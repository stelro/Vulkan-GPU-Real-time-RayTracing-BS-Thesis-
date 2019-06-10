/* =======================================================================
   $File: VulkanEngine.hh
   $Date: 3/6/2018
   $Revision:
   $Creator: Rostislav Orestis Stelmach
   $Notice:  This file is a part of Thesis project ( stracer ) for
                 the Technical Educational Institute of Western Macedonia
                 Supervisor: Dr. George Sisias
   ======================================================================== */

#include "VulkanEngine.hh"

namespace ost {

  VulkanEngine *VulkanEngine::m_VulkanEngineInstance = nullptr;

  VulkanEngine::VulkanEngine(bool enableValidation, u32 width, u32 height) noexcept
    : m_window(nullptr),
      m_submitPipelineStages(VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT),
      m_descriptorPool(VK_NULL_HANDLE),
      m_currentBuffer(0),
      m_prepared(false),
      m_width(width),
      m_height(height),
      m_timer(0.0f),
      m_timerSpeed(0.25f),
      m_paused(false),
      m_zoomSpeed(1.0f),
      m_windowTitle("Stel's Ray-tracing Engine"),
      m_engineName("Ray-tracer Renderer"),
      m_UIOverlay(nullptr),
      m_enabledFeatures({}) {

    m_settings.validation = enableValidation;
    m_settings.overlay = true;

    this->initWindow();
    this->initVulkan();

  }

  VulkanEngine::~VulkanEngine() noexcept {


    // Clean up Vulkan resources
    m_swapChain.destroy();

    glfwDestroyWindow(m_window);

    if ( m_descriptorPool != VK_NULL_HANDLE ) {
      vkDestroyDescriptorPool(m_device, m_descriptorPool, nullptr);
    }

    destroyCommandBuffers();
    vkDestroyRenderPass(m_device, m_renderPass, nullptr);

    for ( auto &i : m_frameBuffers ) {
      vkDestroyFramebuffer(m_device, i, nullptr);
    }

    for ( auto &shaderModule : m_shaderModules ) {
      vkDestroyShaderModule(*m_vulkanDevice, shaderModule, nullptr);
    }

    vkDestroyImageView(m_device, m_depthStencil.view, nullptr);
    vkDestroyImage(m_device, m_depthStencil.image, nullptr);
    vkFreeMemory(m_device, m_depthStencil.mem, nullptr);

    vkDestroyPipelineCache(m_device, m_pipelineCache, nullptr);

    vkDestroyCommandPool(m_device, m_commandPool, nullptr);

    vkDestroySemaphore(m_device, m_semaphores.presentComplete, nullptr);
    vkDestroySemaphore(m_device, m_semaphores.renderComplete, nullptr);
    vkDestroySemaphore(m_device, m_semaphores.overlayComplete, nullptr);

    delete m_UIOverlay;

    delete m_vulkanDevice;

    vkDestroyInstance(m_instance, nullptr);

  }

  std::vector<const char *> VulkanEngine::m_commandLineArgs;

  VkResult VulkanEngine::createVulkanInstance(bool enableValidation) noexcept {

    this->m_settings.validation = enableValidation;

    // Validation can also be forced via a define
#if defined(_VALIDATION)
    this->settings.validation = true;
#endif

    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = m_engineName.c_str();
    appInfo.apiVersion = VK_API_VERSION_1_0;
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    //std::vector<const char *> instanceExtensions = { VK_KHR_SURFACE_EXTENSION_NAME, VK_KHR_SWAPCHAIN_EXTENSION_NAME };
    u32 glfwExtensionsCount = 0;
    const char **glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionsCount);
    std::vector<const char *> extensions(glfwExtensions, glfwExtensions + glfwExtensionsCount);

    if ( !m_enabledInstanceExtensions.empty()) {
      for ( auto enabledExtension : m_enabledInstanceExtensions ) {
        extensions.push_back(enabledExtension);
      }
    }

    VkInstanceCreateInfo instanceCreateInfo = {};
    instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceCreateInfo.pNext = nullptr;
    instanceCreateInfo.pApplicationInfo = &appInfo;

    if ( !extensions.empty()) {

      if ( m_settings.validation ) {
        extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
      }

      instanceCreateInfo.enabledExtensionCount = static_cast<u32>(extensions.size());
      instanceCreateInfo.ppEnabledExtensionNames = extensions.data();
    }

    return vkCreateInstance(&instanceCreateInfo, nullptr, &m_instance);
  }

  bool VulkanEngine::checkCommandBuffers() noexcept {

    for ( auto &cmdBuffer : m_drawCmdBuffers ) {
      if ( cmdBuffer == VK_NULL_HANDLE ) {
        return false;
      }
    }
    return true;
  }

  void VulkanEngine::createCommandBuffers() noexcept {
    // Create one command buffer for each swap chain image and reuse for rendering
    m_drawCmdBuffers.resize(m_swapChain.imageCount);

    VkCommandBufferAllocateInfo cmdBufAllocateInfo =
      ost::initializers::commandBufferAllocateInfo(
        m_commandPool,
        VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        static_cast<u32>(m_drawCmdBuffers.size()));

    VK_CHECK_RESULT(vkAllocateCommandBuffers(m_device, &cmdBufAllocateInfo, m_drawCmdBuffers.data()));
  }

  void VulkanEngine::destroyCommandBuffers() noexcept {

    vkFreeCommandBuffers(m_device, m_commandPool, static_cast<u32>(m_drawCmdBuffers.size()), m_drawCmdBuffers.data());
  }

  VkCommandBuffer VulkanEngine::createCommandBuffer(VkCommandBufferLevel level, bool begin) noexcept {

    VkCommandBuffer cmdBuffer;

    VkCommandBufferAllocateInfo cmdBufAllocateInfo =
      ost::initializers::commandBufferAllocateInfo(
        m_commandPool,
        level,
        1);

    VK_CHECK_RESULT(vkAllocateCommandBuffers(m_device, &cmdBufAllocateInfo, &cmdBuffer));

    // If requested, also start the new command buffer
    if ( begin ) {
      VkCommandBufferBeginInfo cmdBufInfo = ost::initializers::commandBufferBeginInfo();
      VK_CHECK_RESULT(vkBeginCommandBuffer(cmdBuffer, &cmdBufInfo));
    }

    return cmdBuffer;
  }

  void VulkanEngine::flushCommandBuffer(VkCommandBuffer commandBuffer, VkQueue queue, bool free) noexcept {

    if ( commandBuffer == VK_NULL_HANDLE ) {
      return;
    }

    VK_CHECK_RESULT(vkEndCommandBuffer(commandBuffer));

    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    VK_CHECK_RESULT(vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE));
    VK_CHECK_RESULT(vkQueueWaitIdle(queue));

    if ( free ) {
      vkFreeCommandBuffers(m_device, m_commandPool, 1, &commandBuffer);
    }
  }

  void VulkanEngine::createPipelineCache() noexcept {

    VkPipelineCacheCreateInfo pipelineCacheCreateInfo = {};
    pipelineCacheCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
    VK_CHECK_RESULT(vkCreatePipelineCache(m_device, &pipelineCacheCreateInfo, nullptr, &m_pipelineCache));
  }

  void VulkanEngine::prepare() noexcept {

    initSwapchain();
    createCommandPool();
    setupSwapChain();
    createCommandBuffers();
    setupDepthStencil();
    setupRenderPass();
    createPipelineCache();
    setupFrameBuffer();

    if ( m_settings.overlay ) {

      UIOverlayCreateInfo overlayCreateInfo = {};
      // Setup default overlay creation info
      overlayCreateInfo.device = m_vulkanDevice;
      overlayCreateInfo.copyQueue = m_gpuQueue;
      overlayCreateInfo.framebuffers = m_frameBuffers;
      overlayCreateInfo.colorformat = m_swapChain.colorFormat;
      overlayCreateInfo.depthformat = m_depthFormat;
      overlayCreateInfo.width = m_width;
      overlayCreateInfo.height = m_height;
      if ( overlayCreateInfo.shaders.empty()) {
        overlayCreateInfo.shaders = {
          loadShader("./../shaders/gui.vert.spv", VK_SHADER_STAGE_VERTEX_BIT),
          loadShader("./../shaders/gui.frag.spv", VK_SHADER_STAGE_FRAGMENT_BIT),
        };
      }
      m_UIOverlay = new UIOverlay(overlayCreateInfo);
    }
  }

  void VulkanEngine::prepareFrame() noexcept {
    // Acquire the next image from the swap chain
    VkResult err = m_swapChain.acquireNextImage(m_semaphores.presentComplete, &m_currentBuffer);
    VK_CHECK_RESULT(err);
  }

  void VulkanEngine::submitFrame() noexcept {

    bool submitOverlay = m_settings.overlay;

    if ( submitOverlay ) {
      // Wait for color attachment output to finish before rendering the text overlay
      VkPipelineStageFlags stageFlags = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
      m_submitInfo.pWaitDstStageMask = &stageFlags;

      // Set semaphores
      // Wait for render complete semaphore
      m_submitInfo.waitSemaphoreCount = 1;
      m_submitInfo.pWaitSemaphores = &m_semaphores.renderComplete;
      // Signal ready with UI overlay complete semaphpre
      m_submitInfo.signalSemaphoreCount = 1;
      m_submitInfo.pSignalSemaphores = &m_semaphores.overlayComplete;

      // Submit current UI overlay command buffer
      m_submitInfo.commandBufferCount = 1;
      m_submitInfo.pCommandBuffers = &m_UIOverlay->cmdBuffers[m_currentBuffer];
      VK_CHECK_RESULT(vkQueueSubmit(m_gpuQueue, 1, &m_submitInfo, VK_NULL_HANDLE));

      // Reset stage mask
      m_submitInfo.pWaitDstStageMask = &m_submitPipelineStages;
      // Reset wait and signal semaphores for rendering next frame
      // Wait for swap chain presentation to finish
      m_submitInfo.waitSemaphoreCount = 1;
      m_submitInfo.pWaitSemaphores = &m_semaphores.presentComplete;
      // Signal ready with offscreen semaphore
      m_submitInfo.signalSemaphoreCount = 1;
      m_submitInfo.pSignalSemaphores = &m_semaphores.renderComplete;
    }

    VkResult res = m_swapChain.queuePresent(m_gpuQueue,
                                            m_currentBuffer,
                                            submitOverlay ? m_semaphores.overlayComplete : m_semaphores.renderComplete);
    if ( !((res == VK_SUCCESS) || (res == VK_SUBOPTIMAL_KHR))) {
      if ( res == VK_ERROR_OUT_OF_DATE_KHR ) {
        // Swap chain is no longer compatible with the surface and needs to be recreated
        windowResize();
        return;
      } else {
        VK_CHECK_RESULT(res);
      }
    }

    VK_CHECK_RESULT(vkQueueWaitIdle(m_gpuQueue));
  }

  void VulkanEngine::initVulkan() noexcept {

    VkResult err;

    // Vulkan instance
    err = createVulkanInstance(m_settings.validation);
    OST_ASSERT_LOG(!err, "Could not create Vulkan instance");

    // Physical device
    u32 gpuCount = 0;
    // Get number of available physical devices
    VK_CHECK_RESULT(vkEnumeratePhysicalDevices(m_instance, &gpuCount, nullptr));
    OST_ASSERT(gpuCount > 0);
    // Enumerate devices
    std::vector<VkPhysicalDevice> physicalDevices(gpuCount);
    err = vkEnumeratePhysicalDevices(m_instance, &gpuCount, physicalDevices.data());
    OST_ASSERT_LOG(!err, " Could not enumerate physical devices");

    // Defaults to the first device unless specified by command line
    u32 selectedDevice = 0;

    m_physicalDevice = physicalDevices[selectedDevice];

    vkGetPhysicalDeviceProperties(m_physicalDevice, &m_deviceProperties);
    vkGetPhysicalDeviceFeatures(m_physicalDevice, &m_deviceFeatures);
    vkGetPhysicalDeviceMemoryProperties(m_physicalDevice, &m_deviceMemoryProperties);

    getEnabledFeatures();

    // Vulkan device creation
    // This is handled by a separate class that gets a logical device representation
    // and encapsulates functions related to a device
    m_vulkanDevice = new ost::VulkanDevice(m_physicalDevice);
    VkResult res = m_vulkanDevice->createLogicalDevice(m_enabledFeatures, m_enabledDeviceExtensions);
    OST_ASSERT_LOG(res == VK_SUCCESS, "Could not create Vulkan Device");

    m_device = m_vulkanDevice->logicalDevice;

    // Get a graphics queue from the device
    vkGetDeviceQueue(m_device, m_vulkanDevice->queueFamilyIndices.graphics, 0, &m_gpuQueue);

    // Find a suitable depth format
    VkBool32 validDepthFormat = ost::tools::getSupportedDepthFormat(m_physicalDevice, &m_depthFormat);
    OST_ASSERT(validDepthFormat);

    m_swapChain.connect(m_instance, m_physicalDevice, m_device);

    // Create synchronization objects
    VkSemaphoreCreateInfo semaphoreCreateInfo = ost::initializers::semaphoreCreateInfo();

    /** Create a semaphore used to synchronize image presentation
     * Ensures that the image is displayed before we start submitting new commands to the queue
     */
    VK_CHECK_RESULT(vkCreateSemaphore(m_device, &semaphoreCreateInfo, nullptr, &m_semaphores.presentComplete));

    /** Create a semaphore used to synchronize command submission
     * Ensures that the image is not presented until all commands have been submitted and executed
     */
    VK_CHECK_RESULT(vkCreateSemaphore(m_device, &semaphoreCreateInfo, nullptr, &m_semaphores.renderComplete));

    /** Create a semaphore used to synchronize command submission
     * Ensures that the image is not presented until all commands for the UI overlay have been submitted and executed
     * Will be inserted after the render complete semaphore if the UI overlay is enabled
     */
    VK_CHECK_RESULT(vkCreateSemaphore(m_device, &semaphoreCreateInfo, nullptr, &m_semaphores.overlayComplete));

    // Set up submit info structure
    // Semaphores will stay the same during application lifetime
    m_submitInfo = ost::initializers::submitInfo();
    m_submitInfo.pWaitDstStageMask = &m_submitPipelineStages;
    m_submitInfo.waitSemaphoreCount = 1;
    m_submitInfo.pWaitSemaphores = &m_semaphores.presentComplete;
    m_submitInfo.signalSemaphoreCount = 1;
    m_submitInfo.pSignalSemaphores = &m_semaphores.renderComplete;

  }

  void VulkanEngine::viewChanged() noexcept { }

  void VulkanEngine::buildCommandBuffers() noexcept { }

  void VulkanEngine::createCommandPool() noexcept {

    VkCommandPoolCreateInfo cmdPoolInfo = {};
    cmdPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    cmdPoolInfo.queueFamilyIndex = m_swapChain.queueNodeIndex;
    cmdPoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    VK_CHECK_RESULT(vkCreateCommandPool(m_device, &cmdPoolInfo, nullptr, &m_commandPool));
  }

  void VulkanEngine::setupDepthStencil() noexcept {

    VkImageCreateInfo image = {};
    image.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    image.pNext = nullptr;
    image.imageType = VK_IMAGE_TYPE_2D;
    image.format = m_depthFormat;
    image.extent = { m_width, m_height, 1 };
    image.mipLevels = 1;
    image.arrayLayers = 1;
    image.samples = VK_SAMPLE_COUNT_1_BIT;
    image.tiling = VK_IMAGE_TILING_OPTIMAL;
    image.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
    image.flags = 0;

    VkMemoryAllocateInfo mem_alloc = {};
    mem_alloc.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    mem_alloc.pNext = nullptr;
    mem_alloc.allocationSize = 0;
    mem_alloc.memoryTypeIndex = 0;

    VkImageViewCreateInfo depthStencilView = {};
    depthStencilView.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    depthStencilView.pNext = nullptr;
    depthStencilView.viewType = VK_IMAGE_VIEW_TYPE_2D;
    depthStencilView.format = m_depthFormat;
    depthStencilView.flags = 0;
    depthStencilView.subresourceRange = {};
    depthStencilView.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
    depthStencilView.subresourceRange.baseMipLevel = 0;
    depthStencilView.subresourceRange.levelCount = 1;
    depthStencilView.subresourceRange.baseArrayLayer = 0;
    depthStencilView.subresourceRange.layerCount = 1;

    VkMemoryRequirements memReqs;

    VK_CHECK_RESULT(vkCreateImage(m_device, &image, nullptr, &m_depthStencil.image));
    vkGetImageMemoryRequirements(m_device, m_depthStencil.image, &memReqs);
    mem_alloc.allocationSize = memReqs.size;
    mem_alloc.memoryTypeIndex =
      m_vulkanDevice->getMemoryType(memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    VK_CHECK_RESULT(vkAllocateMemory(m_device, &mem_alloc, nullptr, &m_depthStencil.mem));
    VK_CHECK_RESULT(vkBindImageMemory(m_device, m_depthStencil.image, m_depthStencil.mem, 0));

    depthStencilView.image = m_depthStencil.image;
    VK_CHECK_RESULT(vkCreateImageView(m_device, &depthStencilView, nullptr, &m_depthStencil.view));
  }

  void VulkanEngine::setupFrameBuffer() noexcept {

    VkImageView attachments[2];

    // Depth/Stencil attachment is the same for all frame buffers
    attachments[1] = m_depthStencil.view;

    VkFramebufferCreateInfo frameBufferCreateInfo = {};
    frameBufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    frameBufferCreateInfo.pNext = nullptr;
    frameBufferCreateInfo.renderPass = m_renderPass;
    frameBufferCreateInfo.attachmentCount = 2;
    frameBufferCreateInfo.pAttachments = attachments;
    frameBufferCreateInfo.width = m_width;
    frameBufferCreateInfo.height = m_height;
    frameBufferCreateInfo.layers = 1;

    // Create frame buffers for every swap chain image
    m_frameBuffers.resize(m_swapChain.imageCount);
    for ( u32 i = 0; i < m_frameBuffers.size(); i++ ) {
      attachments[0] = m_swapChain.buffers[i].view;
      VK_CHECK_RESULT(vkCreateFramebuffer(m_device, &frameBufferCreateInfo, nullptr, &m_frameBuffers[i]));
    }
  }

  void VulkanEngine::setupRenderPass() noexcept {

    std::array<VkAttachmentDescription, 2> attachments = {};
    // Color attachment
    attachments[0].format = m_swapChain.colorFormat;
    attachments[0].samples = VK_SAMPLE_COUNT_1_BIT;
    attachments[0].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    attachments[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    attachments[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attachments[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attachments[0].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    attachments[0].finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    // Depth attachment
    attachments[1].format = m_depthFormat;
    attachments[1].samples = VK_SAMPLE_COUNT_1_BIT;
    attachments[1].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    attachments[1].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    attachments[1].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    attachments[1].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attachments[1].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    attachments[1].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkAttachmentReference colorReference = {};
    colorReference.attachment = 0;
    colorReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkAttachmentReference depthReference = {};
    depthReference.attachment = 1;
    depthReference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpassDescription = {};
    subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpassDescription.colorAttachmentCount = 1;
    subpassDescription.pColorAttachments = &colorReference;
    subpassDescription.pDepthStencilAttachment = &depthReference;
    subpassDescription.inputAttachmentCount = 0;
    subpassDescription.pInputAttachments = nullptr;
    subpassDescription.preserveAttachmentCount = 0;
    subpassDescription.pPreserveAttachments = nullptr;
    subpassDescription.pResolveAttachments = nullptr;

    // Subpass dependencies for layout transitions
    std::array<VkSubpassDependency, 2> dependencies{};

    dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
    dependencies[0].dstSubpass = 0;
    dependencies[0].srcStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
    dependencies[0].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependencies[0].srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
    dependencies[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    dependencies[0].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

    dependencies[1].srcSubpass = 0;
    dependencies[1].dstSubpass = VK_SUBPASS_EXTERNAL;
    dependencies[1].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependencies[1].dstStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
    dependencies[1].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    dependencies[1].dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
    dependencies[1].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

    VkRenderPassCreateInfo renderPassInfo = {};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = static_cast<u32>(attachments.size());
    renderPassInfo.pAttachments = attachments.data();
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpassDescription;
    renderPassInfo.dependencyCount = static_cast<u32>(dependencies.size());
    renderPassInfo.pDependencies = dependencies.data();

    VK_CHECK_RESULT(vkCreateRenderPass(m_device, &renderPassInfo, nullptr, &m_renderPass));
  }

  void VulkanEngine::getEnabledFeatures() noexcept {
    // Can be overriden in derived class
  }

  void VulkanEngine::windowResize() noexcept {

    if ( !m_prepared ) {
      return;
    }
    m_prepared = false;

    // Ensure all operations on the device have been finished before destroying resources
    vkDeviceWaitIdle(m_device);

    setupSwapChain();

    // Recreate the frame buffers
    vkDestroyImageView(m_device, m_depthStencil.view, nullptr);
    vkDestroyImage(m_device, m_depthStencil.image, nullptr);
    vkFreeMemory(m_device, m_depthStencil.mem, nullptr);

    setupDepthStencil();

    for ( auto &i : m_frameBuffers ) {
      vkDestroyFramebuffer(m_device, i, nullptr);
    }

    setupFrameBuffer();

    // Command buffers need to be recreated as they may store
    // references to the recreated frame buffer
    destroyCommandBuffers();
    createCommandBuffers();
    buildCommandBuffers();

    vkDeviceWaitIdle(m_device);

    // Notify derived class
    windowResized();
    viewChanged();

    m_prepared = true;
  }

  void VulkanEngine::windowResized() noexcept {
    // Can be overriden in derived class
  }

  void VulkanEngine::initSwapchain() noexcept {

    m_swapChain.initSurface(m_window);
  }

  void VulkanEngine::setupSwapChain() noexcept {

    m_swapChain.create(&m_width, &m_height, m_settings.vsync);
  }

  void VulkanEngine::initWindow() noexcept {

    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, false);
    m_window = glfwCreateWindow(m_width, m_height, m_windowTitle.c_str(), nullptr, nullptr);
    OST_ASSERT_LOG(m_window != nullptr, " Could not create GLFW Window!");

  }

  GLFWwindow *VulkanEngine::getWindow() noexcept {

    return m_window;
  }

  VulkanEngine *VulkanEngine::create(bool enableValidation, u32 width, u32 height) noexcept {

    if ( !m_VulkanEngineInstance ) {
      m_VulkanEngineInstance = new VulkanEngine(enableValidation, width, height);
    }

    return m_VulkanEngineInstance;
  }

  VulkanEngine *VulkanEngine::getInstance() noexcept {

    OST_ASSERT_LOG(m_VulkanEngineInstance, "Tried to retrieve uninitialized vulkan engine!");
    return m_VulkanEngineInstance;
  }

  void VulkanEngine::destroy() noexcept {

    OST_ASSERT_LOG(m_VulkanEngineInstance, "You tried to destroy null vulkan instance");
    delete m_VulkanEngineInstance;
    m_VulkanEngineInstance = nullptr;

  }

  VkPhysicalDevice VulkanEngine::getPhysicalDevice() noexcept {

    return m_physicalDevice;
  }

  VkPipelineCache VulkanEngine::getPipelineCache() noexcept {

    return m_pipelineCache;
  }

  VkCommandPool VulkanEngine::getCommandPool() noexcept {

    return m_commandPool;
  }

  VkQueue VulkanEngine::getGpuQueue() noexcept {

    return m_gpuQueue;
  }

  VkDescriptorPool *VulkanEngine::getDescriptorPool() noexcept {

    return &m_descriptorPool;
  }

  VulkanDevice &VulkanEngine::getVulkanDevice() noexcept {

    return *m_vulkanDevice;
  }

  VkRenderPass VulkanEngine::getRenderPass() noexcept {

    return m_renderPass;
  }

  std::vector<VkCommandBuffer> VulkanEngine::getDrawCmdBuffers() const noexcept {

    return m_drawCmdBuffers;
  }

  void VulkanEngine::setDrawCmdBuffers(const std::vector<VkCommandBuffer> &drawCmdBuffers) noexcept {

    m_drawCmdBuffers = drawCmdBuffers;
  }

  std::vector<VkFramebuffer> VulkanEngine::getFrameBuffers() const noexcept {

    return m_frameBuffers;
  }

  void VulkanEngine::setFrameBuffers(const std::vector<VkFramebuffer> &frameBuffers) noexcept {

    m_frameBuffers = frameBuffers;
  }

  u32 VulkanEngine::getWidth() const noexcept {

    return m_width;
  }

  void VulkanEngine::setWidth(u32 width) noexcept {

    m_width = width;
  }

  u32 VulkanEngine::getHeight() const noexcept {

    return m_height;
  }

  void VulkanEngine::setHeight(u32 height) noexcept {

    m_height = height;
  }

  void VulkanEngine::submitCmdBuffer() noexcept {

    // Command buffer to be sumitted to the queue
    m_submitInfo.commandBufferCount = 1;
    m_submitInfo.pCommandBuffers = &m_drawCmdBuffers[m_currentBuffer];
    VK_CHECK_RESULT(vkQueueSubmit(m_gpuQueue, 1, &m_submitInfo, VK_NULL_HANDLE));

  }

  VkDevice VulkanEngine::getDevice() noexcept {

    return m_device;
  }

  void VulkanEngine::engineIsPrepared(bool prep) noexcept {

    m_prepared = prep;
  }

  bool VulkanEngine::isPrepared() const noexcept {

    return m_prepared;
  }

  float VulkanEngine::getTimer() const noexcept {

    return m_timer;
  }

  void VulkanEngine::setWindowTitle(std::string_view name) noexcept {

    m_windowTitle = name;
    glfwSetWindowTitle(m_window, m_windowTitle.c_str());
  }

  VkPhysicalDeviceProperties VulkanEngine::getDeviceProperties() noexcept {

    return m_deviceProperties;
  }

  VkPipelineShaderStageCreateInfo VulkanEngine::loadShader(std::string_view fileName,
                                                           VkShaderStageFlagBits stage) noexcept {

    VkPipelineShaderStageCreateInfo shaderStage = {};
    shaderStage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStage.stage = stage;
    shaderStage.module = tools::loadShader(fileName.data(), *m_vulkanDevice);
    shaderStage.pName = "main";
    OST_ASSERT(shaderStage.module != VK_NULL_HANDLE);
    m_shaderModules.push_back(shaderStage.module);

    return shaderStage;
  }

  UIOverlay *VulkanEngine::getUIOverlay() noexcept {

    return m_UIOverlay;
  }

  bool VulkanEngine::vSycnEnabled() const {

    return m_settings.vsync;
  }

  bool VulkanEngine::validationLayersEnabled() const {

    return m_settings.validation;
  }

}
