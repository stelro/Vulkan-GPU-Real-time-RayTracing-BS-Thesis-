/* =======================================================================
   $File: RayEngine.cc
   $Date: 3/6/2018	
   $Revision: 
   $Creator: Rostislav Orestis Stelmach
   $Notice:  This file is a part of Thesis project ( stracer ) for 
                 the Technical Educational Institute of Western Macedonia
                 Supervisor: Dr. George Sisias
   ======================================================================== */

#include "RayEngine.hh"
#include "VulkanEngine.hh"
#include "GraphicsStage.hh"
#include "ComputeStage.hh"
#include "IOManager.hh"
#include "json/Scene.hh"


#include <chrono>

namespace ost {

  RayEngine::RayEngine(const std::string& filename) noexcept
  : m_scene( new Scene(filename) )
  , m_width( m_scene->getWidth() )
  , m_height( m_scene->getHeight() )
  , m_iomanager(IOManager::getInstance())
  , m_fov(m_scene->getCameraFov())
  , m_cameraOrigin(m_scene->getCameraOrigin())
  , m_cameraLookAt(m_scene->getCameraLookAt())
  {

    m_vulkanEngine = VulkanEngine::create(ENABLE_VALIDATION, m_width, m_height);
    OST_ASSERT_LOG(m_vulkanEngine, "Failed to create VulkanEngine");

    m_iomanager->setWindow(m_vulkanEngine->getWindow());
  }

  RayEngine::~RayEngine() noexcept {

    delete m_scene;

    delete m_computeStage;
    delete m_graphicsStage;
    m_textureComputeTarget.destroy();

    VulkanEngine::destroy();
  }

  void RayEngine::prepareTextureTarget(ost::Texture *tex, u32 width, u32 height, VkFormat format) noexcept {
    // Get device properties for the requested texture format
    VkFormatProperties formatProperties;
    vkGetPhysicalDeviceFormatProperties(m_vulkanEngine->getPhysicalDevice(), format, &formatProperties);
    // Check if requested image format supports image storage operations
    OST_ASSERT(formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT);

    // Prepare blit target texture
    tex->width = width;
    tex->height = height;

    VkImageCreateInfo imageCreateInfo = ost::initializers::imageCreateInfo();
    imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
    imageCreateInfo.format = format;
    imageCreateInfo.extent = { width, height, 1 };
    imageCreateInfo.mipLevels = 1;
    imageCreateInfo.arrayLayers = 1;
    imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    // Image will be sampled in the fragment shader and used as storage target in the compute shader
    imageCreateInfo.usage = VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_STORAGE_BIT;
    imageCreateInfo.flags = 0;

    VkMemoryAllocateInfo memAllocInfo = ost::initializers::memoryAllocateInfo();
    VkMemoryRequirements memReqs;

    VK_CHECK_RESULT(vkCreateImage(m_vulkanEngine->getDevice(), &imageCreateInfo, nullptr, &tex->image));
    vkGetImageMemoryRequirements(m_vulkanEngine->getDevice(), tex->image, &memReqs);
    memAllocInfo.allocationSize = memReqs.size;
    memAllocInfo.memoryTypeIndex =
      m_vulkanEngine->getVulkanDevice().getMemoryType(memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    VK_CHECK_RESULT(vkAllocateMemory(m_vulkanEngine->getDevice(), &memAllocInfo, nullptr, &tex->deviceMemory));
    VK_CHECK_RESULT(vkBindImageMemory(m_vulkanEngine->getDevice(), tex->image, tex->deviceMemory, 0));

    VkCommandBuffer layoutCmd = m_vulkanEngine->createCommandBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY, true);

    tex->imageLayout = VK_IMAGE_LAYOUT_GENERAL;
    ost::tools::setImageLayout(
      layoutCmd,
      tex->image,
      VK_IMAGE_ASPECT_COLOR_BIT,
      VK_IMAGE_LAYOUT_UNDEFINED,
      tex->imageLayout);

    m_vulkanEngine->flushCommandBuffer(layoutCmd, m_vulkanEngine->getGpuQueue(), true);

    // Create sampler
    VkSamplerCreateInfo sampler = ost::initializers::samplerCreateInfo();
    sampler.magFilter = VK_FILTER_LINEAR;
    sampler.minFilter = VK_FILTER_LINEAR;
    sampler.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    sampler.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
    sampler.addressModeV = sampler.addressModeU;
    sampler.addressModeW = sampler.addressModeU;
    sampler.mipLodBias = 0.0f;
    sampler.maxAnisotropy = 1.0f;
    sampler.compareOp = VK_COMPARE_OP_NEVER;
    sampler.minLod = 0.0f;
    sampler.maxLod = 0.0f;
    sampler.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
    VK_CHECK_RESULT(vkCreateSampler(m_vulkanEngine->getDevice(), &sampler, nullptr, &tex->sampler));

    // Create image view
    VkImageViewCreateInfo view = ost::initializers::imageViewCreateInfo();
    view.viewType = VK_IMAGE_VIEW_TYPE_2D;
    view.format = format;
    view.components =
      { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A };
    view.subresourceRange = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };
    view.image = tex->image;
    VK_CHECK_RESULT(vkCreateImageView(m_vulkanEngine->getDevice(), &view, nullptr, &tex->view));

    // Initialize a descriptor for later use
    tex->descriptor.imageLayout = tex->imageLayout;
    tex->descriptor.imageView = tex->view;
    tex->descriptor.sampler = tex->sampler;
    tex->device = &m_vulkanEngine->getVulkanDevice();
  }

  void RayEngine::setupDescriptorPool() noexcept {

    std::vector<VkDescriptorPoolSize> poolSizes =
      {
        // Compute UBO
        ost::initializers::descriptorPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 2),
        // Graphics image samplers
        ost::initializers::descriptorPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,4),
        // Storage image for ray traced image output
        ost::initializers::descriptorPoolSize(VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,1),
        // Storage buffer for the scene primitives
        ost::initializers::descriptorPoolSize(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,4),
      };

    VkDescriptorPoolCreateInfo descriptorPoolInfo =
      ost::initializers::descriptorPoolCreateInfo(
        static_cast<u32>(poolSizes.size()),
        poolSizes.data(),
        3);

    VK_CHECK_RESULT(vkCreateDescriptorPool(m_vulkanEngine->getDevice(),
                                           &descriptorPoolInfo,
                                           nullptr,
                                           m_vulkanEngine->getDescriptorPool()));
  }

  void RayEngine::prepare() noexcept {

    m_vulkanEngine->prepare();

    prepareTextureTarget(&m_textureComputeTarget, m_width, m_height, VK_FORMAT_R32G32B32A32_SFLOAT);

    m_graphicsStage = new GraphicsStage(&m_textureComputeTarget);
    m_computeStage = new ComputeStage(&m_textureComputeTarget);

    // Pass scene ( which is constructed only in Rayengine class )
    m_computeStage->setScene(m_scene);

    m_aaStrength = m_computeStage->getAAStrength();
    m_samplesPerPixel = m_computeStage->getSamplesPerPixel();
    m_rayDepth = m_computeStage->getRayTraceDepth();

    setupDescriptorPool();

    m_computeStage->initialize();
    m_graphicsStage->initialize();

    // Destroy command buffers if already present
    if ( !m_vulkanEngine->checkCommandBuffers()) {
      m_vulkanEngine->destroyCommandBuffers();
      m_vulkanEngine->createCommandBuffers();
    }

    updateUIOverlay();

    m_vulkanEngine->engineIsPrepared(true);

  }

  void RayEngine::engineLoop() noexcept {

    if ( !m_vulkanEngine->isPrepared())
      return;

    while ( !glfwWindowShouldClose(m_vulkanEngine->getWindow())) {

      auto tStart = std::chrono::high_resolution_clock::now();

      // Update Input - Output manager every frame
      // Keyboard - Mouse
      m_iomanager->update(m_helperTimer);

      /*
       * Vulkan render passes, submit command buffers and render frames
       */
      m_vulkanEngine->prepareFrame();
      m_vulkanEngine->submitCmdBuffer();
      m_vulkanEngine->submitFrame();

      /*
       * Stages render methods
       */
      m_computeStage->render(m_helperTimer);
      m_graphicsStage->render(m_helperTimer);

      updateUIOverlay();
      /*
     * Stages update methods
     */
      m_graphicsStage->update(m_helperTimer);
      m_computeStage->update(m_helperTimer);


      m_computeStage->setAAStrength(m_aaStrength);
      m_computeStage->setRayTraceDepth(static_cast<u32>(m_rayDepth));
      m_computeStage->setSmaplesPerPixels(static_cast<u32>(m_samplesPerPixel));
      m_computeStage->setFieldOfView(m_fov);
      m_computeStage->setCameraOrigin(m_cameraOrigin);
      m_computeStage->setCameraLookAt(m_cameraLookAt);


      m_frameCounter++;
      auto tEnd = std::chrono::high_resolution_clock::now();
      auto tDiff = std::chrono::duration<double, std::milli>(tEnd - tStart).count();
      m_deltaTime = static_cast<float>(tDiff / 1000.0f);

      if ( !m_paused ) {
        m_helperTimer += m_timerSpeed * m_deltaTime;
        if ( m_helperTimer > 1.0 ) {
          m_helperTimer -= 1.0f;
        }
      }


      m_FPSTimer += ( float ) tDiff;
      if ( m_FPSTimer > 1000.0f ) {

        m_lastFPS = m_frameCounter * (1000.0f / m_FPSTimer);
        m_FPSTimer = 0.0f;
        m_frameCounter = 0;
      }
    }

    vkDeviceWaitIdle(m_vulkanEngine->getDevice());
  }

  void RayEngine::updateUIOverlay() noexcept {

    ImGuiIO &io = ImGui::GetIO();

    auto mousePosX = m_iomanager->getMousePosX();
    auto mousePosY = m_iomanager->getMousePosY();

    io.DisplaySize = ImVec2(( float ) m_vulkanEngine->getWidth(), ( float ) m_vulkanEngine->getHeight());
    io.DeltaTime = m_deltaTime;

    io.MousePos = ImVec2(mousePosX, mousePosY);
    io.MouseDown[0] = m_iomanager->isLeftMousePressed();
    io.MouseDown[1] = m_iomanager->isRightMousePressed();

    ImGui::NewFrame();

    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 1.0f);
    ImGui::SetNextWindowPos(ImVec2(10, 10));
    ImGui::SetNextWindowSize(ImVec2(0, 0), ImGuiSetCond_FirstUseEver);
    ImGui::Begin("Stelmach Ro Thesis",
                 nullptr,
                 ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove );
    ImGui::TextColored(ImVec4(0.804, 0.863, 0.224, 1),"Ray-tracer Renderer");
    ImGui::Text("vSync: %s", m_vulkanEngine->vSycnEnabled() ? "True" : "False");
    ImGui::Text("No. of Shapes: %d", m_computeStage->getNumberOfShapes());
    ImGui::Text("vK Validation: %s", m_vulkanEngine->validationLayersEnabled() ? "True" : "False");
    ImGui::TextUnformatted(m_vulkanEngine->getDeviceProperties().deviceName);
    ImGui::Text("Mouse X: %.1lf", mousePosX);
    ImGui::Text("Mouse Y: %.1lf", mousePosY);
    ImGui::Text("%.2f ms/frame (%.1f fps)", (1000.0f / m_lastFPS), m_lastFPS);
    ImGui::Separator();
    ImGui::SliderFloat("AA_Strength", &m_aaStrength, 0.0f, 1.0f);
    ImGui::SliderInt("Samples Per Pixel", &m_samplesPerPixel, 1, 64);
    ImGui::SliderInt("Ray Depth", &m_rayDepth, 1, 64);
    ImGui::InputFloat("Field Of View", &m_fov, 5.0f, .1);
    ImGui::SliderFloat3("Camera Origin", m_cameraOrigin.data(), -30.0f, 30.f);
    ImGui::SliderFloat3("Camera lookAt", m_cameraLookAt.data(), -30.0f, 30.f);

    ImGui::PushItemWidth(110.0f * m_vulkanEngine->getUIOverlay()->scale);
    ImGui::PopItemWidth();

    ImGui::End();
    ImGui::PopStyleVar();
    ImGui::Render();

    m_vulkanEngine->getUIOverlay()->update();

  }

}
