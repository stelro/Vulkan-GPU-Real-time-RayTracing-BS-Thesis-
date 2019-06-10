/* =======================================================================
   $File: ComputeStage.cc
   $Date: 17/6/2018	
   $Revision: 
   $Creator: Rostislav Orestis Stelmach
   $Notice:  This file is a part of Thesis project ( stracer ) for 
                 the Technical Educational Institute of Western Macedonia
                 Supervisor: Dr. George Sisias
   ======================================================================== */

// Engine Includes
#include "ComputeStage.hh"
#include "Vulkan/VulkanInitializers.hh"
#include "Vulkan/VulkanTexture.hh"
#include "Geometry/Sphere.hh"
#include "Geometry/Plane.hh"
#include "Geometry/Rectangle.hh"
#include "Vulkan/VulkanDevice.hh"
#include "VulkanEngine.hh"
#include "Materials.hh"
#include "Utils.hh"
#include "IOManager.hh"
#include "Math/MatrixTransformations.hh"

// C++ Includes
#include <vector>

namespace ost {

  ComputeStage::ComputeStage(Texture *texture) noexcept
    : StageBase(VulkanEngine::getInstance()), m_texture(texture) , m_iomanager(IOManager::getInstance()) {
    // Empty Constructor
  }

  ComputeStage::~ComputeStage() noexcept {

    vkDestroyPipeline(m_VulkanEngine->getVulkanDevice(), m_pipeline, nullptr);
    vkDestroyPipelineLayout(m_VulkanEngine->getVulkanDevice(), m_pipelineLayout, nullptr);
    vkDestroyDescriptorSetLayout(m_VulkanEngine->getVulkanDevice(), m_descriptorSetLayout, nullptr);
    vkDestroyFence(m_VulkanEngine->getVulkanDevice(), m_fence, nullptr);
    vkDestroyCommandPool(m_VulkanEngine->getVulkanDevice(), m_commandPool, nullptr);

    m_boxesBuffer.destroy();
    m_rectanglesBuffer.destroy();
    m_planesBuffer.destroy();
    m_spheresBuffer.destroy();
    m_randomSamples.destroy();
    m_uniformBuffer.destroy();

  }

  void ComputeStage::initialize() noexcept {

    prepareStorageBuffers();
    prepareUniformBuffers();
    setupDescriptorSetLayout();
    setupPipeline();
    setupDescriptorSet();
    buildCommandBuffers();

  }

  void ComputeStage::setupPipeline() noexcept {

//    VkDeviceQueueCreateInfo queueCreateInfo = {};
//    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
//    queueCreateInfo.pNext = nullptr;
//    queueCreateInfo.queueFamilyIndex = m_VulkanEngine->getVulkanDevice().queueFamilyIndices.compute;
//    queueCreateInfo.queueCount = 1;
    vkGetDeviceQueue(m_VulkanEngine->getVulkanDevice(),
                     m_VulkanEngine->getVulkanDevice().queueFamilyIndices.compute,
                     0,
                     &m_queue);

    std::vector<VkDescriptorSetLayoutBinding> setLayoutBindings = {
      // Binding 0: Storage image (raytraced output)
      ost::initializers::descriptorSetLayoutBinding(
        VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,
        VK_SHADER_STAGE_COMPUTE_BIT,
        0),
      // Binding 1: Shader storage buffer for the spheres
      ost::initializers::descriptorSetLayoutBinding(
        VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
        VK_SHADER_STAGE_COMPUTE_BIT,
        1),
      // Binding 2: Shader storage buffer for the planes
      ost::initializers::descriptorSetLayoutBinding(
        VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
        VK_SHADER_STAGE_COMPUTE_BIT,
        2),
      // Biding 3: Uniform Buffer block
      ost::initializers::descriptorSetLayoutBinding(
        VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
        VK_SHADER_STAGE_COMPUTE_BIT,
        3),
      // Binding 4: Shader storage buffer for the Rectangles
      ost::initializers::descriptorSetLayoutBinding(
        VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
        VK_SHADER_STAGE_COMPUTE_BIT,
        4),
      // Binding 5: Shader storage buffer for the Boxes
      ost::initializers::descriptorSetLayoutBinding(
        VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
        VK_SHADER_STAGE_COMPUTE_BIT,
        5),

    };

    VkDescriptorSetLayoutCreateInfo descriptorLayout =
      ost::initializers::descriptorSetLayoutCreateInfo(
        setLayoutBindings.data(),
        static_cast<u32>(setLayoutBindings.size()));

    VK_CHECK_RESULT(vkCreateDescriptorSetLayout(m_VulkanEngine->getVulkanDevice(),
                                                &descriptorLayout,
                                                nullptr,
                                                &m_descriptorSetLayout));

    VkPipelineLayoutCreateInfo pPipelineLayoutCreateInfo =
      ost::initializers::pipelineLayoutCreateInfo(
        &m_descriptorSetLayout,
        1);

    VK_CHECK_RESULT(vkCreatePipelineLayout(m_VulkanEngine->getVulkanDevice(),
                                           &pPipelineLayoutCreateInfo,
                                           nullptr,
                                           &m_pipelineLayout));

    VkDescriptorSetAllocateInfo allocInfo =
      ost::initializers::descriptorSetAllocateInfo(
        *m_VulkanEngine->getDescriptorPool(),
        &m_descriptorSetLayout,
        1);

    VK_CHECK_RESULT(vkAllocateDescriptorSets(m_VulkanEngine->getVulkanDevice(), &allocInfo, &m_descriptorSet));


    std::vector<VkWriteDescriptorSet> computeWriteDescriptorSets = {

          // Binding 0: Output storage image
          ost::initializers::writeDescriptorSet(
          m_descriptorSet,
          VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,
          0,
          &m_texture->descriptor),
          // Binding 1: Shader storage buffer for the spheres
          ost::initializers::writeDescriptorSet(
          m_descriptorSet,
          VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
          1,
          &m_spheresBuffer.descriptor),
          // Biding 2: Shader storage buffer for the Planes
          ost::initializers::writeDescriptorSet(
          m_descriptorSet,
          VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
          2,
          &m_planesBuffer.descriptor),
          // Binding 3: Uniform Buffer Object
          ost::initializers::writeDescriptorSet(
          m_descriptorSet,
          VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
          3,
          &m_uniformBuffer.descriptor),
          // Biding 4: Shader storage buffer for the Rectangles
          ost::initializers::writeDescriptorSet(
          m_descriptorSet,
          VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
          4,
          &m_rectanglesBuffer.descriptor),
          // Biding 5: Shader storage buffer for the Boxes
          ost::initializers::writeDescriptorSet(
          m_descriptorSet,
          VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
          5,
          &m_boxesBuffer.descriptor),

      };

    vkUpdateDescriptorSets(m_VulkanEngine->getVulkanDevice(),
                           static_cast<u32>(computeWriteDescriptorSets.size()),
                           computeWriteDescriptorSets.data(),
                           0,
                           nullptr);

    // Create compute shader pipelines
    VkComputePipelineCreateInfo computePipelineCreateInfo =
      ost::initializers::computePipelineCreateInfo(
        m_pipelineLayout,
        0);

    computePipelineCreateInfo.stage = m_VulkanEngine->loadShader(
      "./../shaders/render.comp.spv", VK_SHADER_STAGE_COMPUTE_BIT );

    VK_CHECK_RESULT(vkCreateComputePipelines(m_VulkanEngine->getVulkanDevice(),
                                             m_VulkanEngine->getPipelineCache(),
                                             1,
                                             &computePipelineCreateInfo,
                                             nullptr,
                                             &m_pipeline));

    // Separate command pool as queue family for compute may be different than graphics
    VkCommandPoolCreateInfo cmdPoolInfo = {};
    cmdPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    cmdPoolInfo.queueFamilyIndex = m_VulkanEngine->getVulkanDevice().queueFamilyIndices.compute;
    cmdPoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    VK_CHECK_RESULT(vkCreateCommandPool(m_VulkanEngine->getVulkanDevice(), &cmdPoolInfo, nullptr, &m_commandPool));

    // Create a command buffer for compute operations
    VkCommandBufferAllocateInfo cmdBufAllocateInfo =
      ost::initializers::commandBufferAllocateInfo(
        m_commandPool,
        VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        1);

    VK_CHECK_RESULT(vkAllocateCommandBuffers(m_VulkanEngine->getVulkanDevice(),
                                             &cmdBufAllocateInfo,
                                             &m_commandBuffer));

    // Fence for compute CB sync
    VkFenceCreateInfo fenceCreateInfo = ost::initializers::fenceCreateInfo(VK_FENCE_CREATE_SIGNALED_BIT);
    VK_CHECK_RESULT(vkCreateFence(m_VulkanEngine->getVulkanDevice(), &fenceCreateInfo, nullptr, &m_fence));

    // Build a single command buffer containing the compute dispatch commands
    buildCommandBuffers();

  }

  void ComputeStage::setupDescriptorSet() noexcept {

  }

  void ComputeStage::setupDescriptorSetLayout() noexcept {

  }

  void ComputeStage::buildCommandBuffers() noexcept {

    VkCommandBufferBeginInfo cmdBufInfo = ost::initializers::commandBufferBeginInfo();

    VK_CHECK_RESULT(vkBeginCommandBuffer(m_commandBuffer, &cmdBufInfo));

    vkCmdBindPipeline(m_commandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, m_pipeline);
    vkCmdBindDescriptorSets(m_commandBuffer,
                            VK_PIPELINE_BIND_POINT_COMPUTE,
                            m_pipelineLayout,
                            0,
                            1,
                            &m_descriptorSet,
                            0,
                            nullptr);

    vkCmdDispatch(m_commandBuffer, ( uint32_t ) ceil( m_scene->getWidth() / float(8)), ( uint32_t ) ceil( m_scene->getHeight() / float(8)), 1);
    vkEndCommandBuffer(m_commandBuffer);
  }

  void ComputeStage::prepareUniformBuffers() noexcept {

    m_VulkanEngine->getVulkanDevice().createBuffer(
      VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
      VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
      &m_uniformBuffer,
      sizeof(m_uniformBuffer));

    updateUniformBuffers();

  }

  void ComputeStage::prepareStorageBuffers() noexcept {


    // Construct Spheres
    std::vector<Sphere> spheres = m_scene->getSpheres();

    VkDeviceSize storageBufferSize = spheres.size() * sizeof(Sphere);


    /*
     * Creation of the stage buffer
     * In order to transfer buffer of spheres or any kind of storage buffer to the
     * GPU, we should transfer first the data to a staging buffer, and then transfer
     * the staging contents to the GPU. We cannot directly access GPU's memory.
     */
    ost::VulkanBuffer stagingBuffer;

    m_VulkanEngine->getVulkanDevice().createBuffer(
      VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
      VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
      &stagingBuffer,
      storageBufferSize,
      spheres.data());

    m_VulkanEngine->getVulkanDevice().createBuffer(
      VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
      VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
      &m_spheresBuffer,
      storageBufferSize);


    // Copy to staging buffer
    VkCommandBuffer copyCommand = m_VulkanEngine->createCommandBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY, true);
    VkBufferCopy copyRegion = {};
    copyRegion.size = storageBufferSize;
    vkCmdCopyBuffer(copyCommand, stagingBuffer.buffer, m_spheresBuffer.buffer, 1, &copyRegion);
    m_VulkanEngine->flushCommandBuffer(copyCommand, m_VulkanEngine->getGpuQueue(), true);

    stagingBuffer.destroy();

    // Construct Planes

    std::vector<Plane> planes = m_scene->getPlanes();

    storageBufferSize = planes.size() * sizeof(Plane);

    m_VulkanEngine->getVulkanDevice().createBuffer(
      VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
      VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
      &stagingBuffer,
      storageBufferSize,
      planes.data());

    m_VulkanEngine->getVulkanDevice().createBuffer(
      VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
      VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
      &m_planesBuffer,
      storageBufferSize);

    copyCommand = m_VulkanEngine->createCommandBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY, true);
    copyRegion = {};
    copyRegion.size = storageBufferSize;
    vkCmdCopyBuffer(copyCommand, stagingBuffer.buffer, m_planesBuffer.buffer, 1, &copyRegion);
    m_VulkanEngine->flushCommandBuffer(copyCommand, m_VulkanEngine->getGpuQueue(), true);

    stagingBuffer.destroy();

    // Construct Rectangles

    std::vector<Rectangle> rectangles = m_scene->getRectangles();


    storageBufferSize = rectangles.size() * sizeof(Rectangle);

    m_VulkanEngine->getVulkanDevice().createBuffer(
      VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
      VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
      &stagingBuffer,
      storageBufferSize,
      rectangles.data());

    m_VulkanEngine->getVulkanDevice().createBuffer(
      VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
      VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
      &m_rectanglesBuffer,
      storageBufferSize);

    copyCommand = m_VulkanEngine->createCommandBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY, true);
    copyRegion = {};
    copyRegion.size = storageBufferSize;
    vkCmdCopyBuffer(copyCommand, stagingBuffer.buffer, m_rectanglesBuffer.buffer, 1, &copyRegion);
    m_VulkanEngine->flushCommandBuffer(copyCommand, m_VulkanEngine->getGpuQueue(), true);

    stagingBuffer.destroy();

    // Construct Rectangles

    std::vector<Box> boxes = m_scene->getBoxes();


    storageBufferSize = boxes.size() * sizeof(Box);

    m_VulkanEngine->getVulkanDevice().createBuffer(
      VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
      VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
      &stagingBuffer,
      storageBufferSize,
      boxes.data());

    m_VulkanEngine->getVulkanDevice().createBuffer(
      VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
      VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
      &m_boxesBuffer,
      storageBufferSize);

    copyCommand = m_VulkanEngine->createCommandBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY, true);
    copyRegion = {};
    copyRegion.size = storageBufferSize;
    vkCmdCopyBuffer(copyCommand, stagingBuffer.buffer, m_boxesBuffer.buffer, 1, &copyRegion);
    m_VulkanEngine->flushCommandBuffer(copyCommand, m_VulkanEngine->getGpuQueue(), true);

    stagingBuffer.destroy();
  }

  void ComputeStage::updateUniformBuffers() noexcept {

    m_ubo.randomSeed = Utils::getElapsedTime();
    m_ubo.ambient = m_scene->getAmbientColor();

    m_ubo.spheres_are_enabled = m_scene->spheresAreEnabled();
    m_ubo.planes_are_enabled = m_scene->planesAreEnabled();
    m_ubo.boxes_are_enabled = m_scene->boxesAreEnabled();
    m_ubo.rects_are_enabled = m_scene->rectsAreEnabled();


    VK_CHECK_RESULT(m_uniformBuffer.map());
    memcpy(m_uniformBuffer.mapped, &m_ubo, sizeof(m_ubo));
    m_uniformBuffer.unmap();
  }

  void ComputeStage::update([[maybe_unused]] float dt) noexcept {
    updateUniformBuffers();
    m_ubo.anyKeyIsPressed = m_iomanager->inputIsActive() ? 1 : 0;
  }

  void ComputeStage::render([[maybe_unused]] float dt) noexcept {
    // Submit compute commands
    // Use a fence to ensure that compute command buffer has finished executing before using it again
    vkWaitForFences(m_VulkanEngine->getVulkanDevice(), 1, &m_fence, VK_TRUE, UINT64_MAX);
    vkResetFences(m_VulkanEngine->getVulkanDevice(), 1, &m_fence);

    VkSubmitInfo computeSubmitInfo = ost::initializers::submitInfo();
    computeSubmitInfo.commandBufferCount = 1;
    computeSubmitInfo.pCommandBuffers = &m_commandBuffer;

    VK_CHECK_RESULT(vkQueueSubmit(m_queue, 1, &computeSubmitInfo, m_fence));
  }

  void ComputeStage::setAAStrength(float value) noexcept {
    m_ubo.aaStrength = value;
  }

  void ComputeStage::setRayTraceDepth(u32 value) noexcept {
    m_ubo.rayDepth = value;
  }

  void ComputeStage::setSmaplesPerPixels(u32 value) noexcept {
    m_ubo.samplesPerPixel = value;
  }

  float ComputeStage::getAAStrength() const noexcept {
    return m_ubo.aaStrength;
  }

  u32 ComputeStage::getRayTraceDepth() const noexcept {
    return m_ubo.rayDepth;
  }

  u32 ComputeStage::getSamplesPerPixel() const noexcept {
    return m_ubo.samplesPerPixel;
  }

  u32 ComputeStage::getNumberOfShapes() const noexcept {
    return m_scene->getTotalObjects() + 2;
  }

  void ComputeStage::setFieldOfView(float fov) noexcept {
    m_ubo.fov = fov;
  }

  float ComputeStage::getFieldOfView() const noexcept {
    return m_ubo.fov;
  }

  std::array<float, 3> ComputeStage::getCameraOrigin() const noexcept {
    return std::array<float, 3> {  m_ubo.origin.x, m_ubo.origin.y, m_ubo.origin.z };
  }

  std::array<float, 3> ComputeStage::getCameraLookAt() const noexcept {
    return std::array<float, 3> {  m_ubo.lookAt.x, m_ubo.lookAt.y, m_ubo.lookAt.z };
  }

  void ComputeStage::setCameraOrigin(std::array<float, 3> arr) noexcept {
    m_ubo.origin = arr;
  }

  void ComputeStage::setCameraLookAt(std::array<float, 3> arr) noexcept {
    m_ubo.lookAt = arr;
  }

  void ComputeStage::setScene(Scene* s) noexcept {
    m_scene = s;
  }

  void ComputeStage::setAmbientColor(const ost::Vec3 &value) noexcept {
    m_ubo.ambient = value;
  }

}
