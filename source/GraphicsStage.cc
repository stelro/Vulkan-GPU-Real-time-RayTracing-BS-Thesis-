/* =======================================================================
   $File: GraphicsStage.cc
   $Date: 17/6/2018	
   $Revision: 
   $Creator: Rostislav Orestis Stelmach
   $Notice:  This file is a part of Thesis project ( stracer ) for 
                 the Technical Educational Institute of Western Macedonia
                 Supervisor: Dr. George Sisias
   ======================================================================== */

#include "GraphicsStage.hh"
#include "Vulkan/VulkanInitializers.hh"
#include "Vulkan/VulkanTexture.hh"
#include "VulkanEngine.hh"
#include "IOManager.hh"

namespace ost {

  GraphicsStage::GraphicsStage(Texture *texture) noexcept
    : StageBase(VulkanEngine::getInstance()), m_texture(texture), m_iomanager(IOManager::getInstance()) {
    // Empty Constructor
  }

  GraphicsStage::~GraphicsStage() noexcept {

    vkDestroyPipeline(m_VulkanEngine->getVulkanDevice(), m_pipeline, nullptr);
    vkDestroyPipelineLayout(m_VulkanEngine->getVulkanDevice(), m_pipelineLayout, nullptr);
    vkDestroyDescriptorSetLayout(m_VulkanEngine->getVulkanDevice(), m_descriptorSetLayout, nullptr);
  }

  void GraphicsStage::initialize() noexcept {

    prepareStorageBuffers();
    prepareUniformBuffers();
    setupDescriptorSetLayout();
    setupPipeline();
    setupDescriptorSet();
    buildCommandBuffers();
  }

  void GraphicsStage::setupPipeline() noexcept {

    VkPipelineInputAssemblyStateCreateInfo inputAssemblyState =
      ost::initializers::pipelineInputAssemblyStateCreateInfo(
        VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
        0,
        VK_FALSE);

    /*
     * Disabling culling
     */
    VkPipelineRasterizationStateCreateInfo rasterizationState =
      ost::initializers::pipelineRasterizationStateCreateInfo(
        VK_POLYGON_MODE_FILL,
        VK_CULL_MODE_NONE,
        VK_FRONT_FACE_COUNTER_CLOCKWISE,
        0);

    /*
     * Current blendAttachamentState and colorBlendState
     * is enabling Alpha Blending in fragment shader.
     */

    VkPipelineColorBlendAttachmentState blendAttachmentState = {};
    blendAttachmentState.colorWriteMask =
      VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    blendAttachmentState.blendEnable = VK_TRUE;
    blendAttachmentState.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
    blendAttachmentState.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    blendAttachmentState.colorBlendOp = VK_BLEND_OP_ADD;
    blendAttachmentState.srcAlphaBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
    blendAttachmentState.dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    blendAttachmentState.alphaBlendOp = VK_BLEND_OP_ADD;

    VkPipelineColorBlendStateCreateInfo colorBlendState = {};
    colorBlendState.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlendState.logicOpEnable = VK_FALSE;
    colorBlendState.logicOp = VK_LOGIC_OP_COPY;
    colorBlendState.attachmentCount = 1;
    colorBlendState.pAttachments = &blendAttachmentState;
    colorBlendState.blendConstants[0] = 0.0f; // Optional
    colorBlendState.blendConstants[1] = 0.0f; // Optional
    colorBlendState.blendConstants[2] = 0.0f; // Optional
    colorBlendState.blendConstants[3] = 0.0f; // Optional

    VkPipelineDepthStencilStateCreateInfo depthStencilState =
      ost::initializers::pipelineDepthStencilStateCreateInfo(
        // Don't test depth buffer
        VK_FALSE,
        // Don't write to depth buffer
        VK_FALSE,
        VK_COMPARE_OP_LESS_OR_EQUAL);

    VkPipelineViewportStateCreateInfo viewportState =
      ost::initializers::pipelineViewportStateCreateInfo(1, 1, 0);

    VkPipelineMultisampleStateCreateInfo multisampleState =
      ost::initializers::pipelineMultisampleStateCreateInfo(
        VK_SAMPLE_COUNT_1_BIT,
        0);

    std::vector<VkDynamicState> dynamicStateEnables = {
      VK_DYNAMIC_STATE_VIEWPORT,
      VK_DYNAMIC_STATE_SCISSOR };

    VkPipelineDynamicStateCreateInfo dynamicState =
      ost::initializers::pipelineDynamicStateCreateInfo(
        dynamicStateEnables.data(),
        static_cast<u32>(dynamicStateEnables.size()),
        0);

    // Display pipeline
    std::array<VkPipelineShaderStageCreateInfo, 2> shaderStages{};

    shaderStages[0] =
      m_VulkanEngine->loadShader("./../shaders/texture.vert.spv", VK_SHADER_STAGE_VERTEX_BIT);
    shaderStages[1] =
      m_VulkanEngine->loadShader("./../shaders/texture.frag.spv", VK_SHADER_STAGE_FRAGMENT_BIT);

    VkGraphicsPipelineCreateInfo pipelineCreateInfo =
      ost::initializers::pipelineCreateInfo(
        m_pipelineLayout,
        m_VulkanEngine->getRenderPass(),
        0);

    VkPipelineVertexInputStateCreateInfo emptyInputState{};
    emptyInputState.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

    emptyInputState.vertexAttributeDescriptionCount = 0;
    emptyInputState.pVertexAttributeDescriptions = nullptr;
    emptyInputState.vertexBindingDescriptionCount = 0;
    emptyInputState.pVertexBindingDescriptions = nullptr;
    pipelineCreateInfo.pVertexInputState = &emptyInputState;

    pipelineCreateInfo.pInputAssemblyState = &inputAssemblyState;
    pipelineCreateInfo.pRasterizationState = &rasterizationState;
    pipelineCreateInfo.pColorBlendState = &colorBlendState;
    pipelineCreateInfo.pMultisampleState = &multisampleState;
    pipelineCreateInfo.pViewportState = &viewportState;
    pipelineCreateInfo.pDepthStencilState = &depthStencilState;
    pipelineCreateInfo.pDynamicState = &dynamicState;
    pipelineCreateInfo.stageCount = shaderStages.size();
    pipelineCreateInfo.pStages = shaderStages.data();
    pipelineCreateInfo.renderPass = m_VulkanEngine->getRenderPass();

    VK_CHECK_RESULT(vkCreateGraphicsPipelines(m_VulkanEngine->getVulkanDevice(),
                                              m_VulkanEngine->getPipelineCache(),
                                              1,
                                              &pipelineCreateInfo,
                                              nullptr,
                                              &m_pipeline));
  }

  void GraphicsStage::setupDescriptorSet() noexcept {

    VkDescriptorSetAllocateInfo allocInfo =
      ost::initializers::descriptorSetAllocateInfo(
        *m_VulkanEngine->getDescriptorPool(),
        &m_descriptorSetLayout,
        1);

    VK_CHECK_RESULT(vkAllocateDescriptorSets(m_VulkanEngine->getVulkanDevice(),
                                             &allocInfo,
                                             &m_descriptorSetPostCompute));

    std::vector<VkWriteDescriptorSet> writeDescriptorSets =
      {
        // Binding 0 : Fragment shader texture sampler
        ost::initializers::writeDescriptorSet(
          m_descriptorSetPostCompute,
          VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
          0,
          &m_texture->descriptor) };

    vkUpdateDescriptorSets(m_VulkanEngine->getVulkanDevice(),
                           static_cast<u32>(writeDescriptorSets.size()),
                           writeDescriptorSets.data(),
                           0,
                           nullptr);
  }

  void GraphicsStage::setupDescriptorSetLayout() noexcept {

    std::vector<VkDescriptorSetLayoutBinding> setLayoutBindings =
      {
        // Binding 0 : Fragment shader image sampler
        ost::initializers::descriptorSetLayoutBinding(
          VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
          VK_SHADER_STAGE_FRAGMENT_BIT,
          0) };

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

    // push constants
    VkPushConstantRange pushConstantRange = ost::initializers::pushConstantRange(
      VK_SHADER_STAGE_FRAGMENT_BIT,
      sizeof(PushConstants), 0);

    pPipelineLayoutCreateInfo.pushConstantRangeCount = 1;
    pPipelineLayoutCreateInfo.pPushConstantRanges = &pushConstantRange;

    VK_CHECK_RESULT(vkCreatePipelineLayout(m_VulkanEngine->getVulkanDevice(),
                                           &pPipelineLayoutCreateInfo,
                                           nullptr,
                                           &m_pipelineLayout));
  }

  void GraphicsStage::buildCommandBuffers() noexcept {

    int divider = 1;

    VkCommandBufferBeginInfo cmdBufInfo = initializers::commandBufferBeginInfo();

    VkClearValue clearValues[2];
    clearValues[0].color = {{ 0.025f, 0.025f, 0.025f, 1.0f }};
    clearValues[1].depthStencil = { 1.0f, 0 };

    VkRenderPassBeginInfo renderPassBeginInfo = ost::initializers::renderPassBeginInfo();

    renderPassBeginInfo.renderPass = m_VulkanEngine->getRenderPass();
    renderPassBeginInfo.renderArea.offset.x = 0;
    renderPassBeginInfo.renderArea.offset.y = 0;
    renderPassBeginInfo.renderArea.extent.width = m_VulkanEngine->getWidth() / divider;
    renderPassBeginInfo.renderArea.extent.height = m_VulkanEngine->getHeight() / divider;
    renderPassBeginInfo.clearValueCount = 2;
    renderPassBeginInfo.pClearValues = clearValues;

    auto drawCmdBuffers = m_VulkanEngine->getDrawCmdBuffers();
    auto frameBuffers = m_VulkanEngine->getFrameBuffers();

    for ( u32 i = 0; i < drawCmdBuffers.size(); ++i ) {
      // Set target frame buffer
      renderPassBeginInfo.framebuffer = frameBuffers[i];

      VK_CHECK_RESULT(vkBeginCommandBuffer(drawCmdBuffers[i], &cmdBufInfo));

      // Image memory barrier to make sure that compute shader writes are finished before sampling from the texture
      VkImageMemoryBarrier imageMemoryBarrier = {};
      imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
      imageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_GENERAL;
      imageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_GENERAL;
      imageMemoryBarrier.image = m_texture->image;
      imageMemoryBarrier.subresourceRange = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };
      imageMemoryBarrier.srcAccessMask = VK_ACCESS_SHADER_WRITE_BIT;
      imageMemoryBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
      vkCmdPipelineBarrier(
        drawCmdBuffers[i],
        VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
        VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
        VK_FLAGS_NONE,
        0, nullptr,
        0, nullptr,
        1, &imageMemoryBarrier);

      vkCmdBeginRenderPass(drawCmdBuffers[i], &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

      VkViewport viewport = ost::initializers::viewport(( float ) m_VulkanEngine->getWidth() / divider,
                                                        ( float ) m_VulkanEngine->getHeight() / divider,
                                                        0.0f,
                                                        1.0f);
      vkCmdSetViewport(drawCmdBuffers[i], 0, 1, &viewport);

      VkRect2D scissor =
        ost::initializers::rect2D(m_VulkanEngine->getWidth() / divider, m_VulkanEngine->getHeight() / divider, 0, 0);
      vkCmdSetScissor(drawCmdBuffers[i], 0, 1, &scissor);

      // Display ray traced image generated by compute shader as a full screen quad
      // Quad vertices are generated in the vertex shader
      vkCmdBindDescriptorSets(drawCmdBuffers[i],
                              VK_PIPELINE_BIND_POINT_GRAPHICS,
                              m_pipelineLayout,
                              0,
                              1,
                              &m_descriptorSetPostCompute,
                              0,
                              nullptr);

      // Submit randomSeed via push_constant
      vkCmdPushConstants(
        drawCmdBuffers[i],
        m_pipelineLayout,
        VK_SHADER_STAGE_FRAGMENT_BIT,
        0,
        sizeof(PushConstants),
        &m_pushConstants);

      vkCmdBindPipeline(drawCmdBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline);
      vkCmdDraw(drawCmdBuffers[i], 3, 1, 0, 0);

      vkCmdEndRenderPass(drawCmdBuffers[i]);

      VK_CHECK_RESULT(vkEndCommandBuffer(drawCmdBuffers[i]));

    }
  }

  void GraphicsStage::prepareStorageBuffers() noexcept {
    // Empty
    // We don't use any storage buffers in graphics stage
  }

  void GraphicsStage::prepareUniformBuffers() noexcept {
    // Empty
    // We don't use any uniform buffer in graphics stage
  }

  void GraphicsStage::update([[maybe_unused]] float dt) noexcept {

    if ( m_iomanager->inputIsActive() ) {
      m_pushConstants.anyKeyPressed = 1;
      m_pushConstants.currentSample = 0.0f;
    } else {
      m_pushConstants.anyKeyPressed = 0;
    }


  }

  void GraphicsStage::render([[maybe_unused]] float dt) noexcept {

    if ( !m_VulkanEngine->checkCommandBuffers()) {
      m_VulkanEngine->destroyCommandBuffers();
      m_VulkanEngine->createCommandBuffers();

    }

    m_pushConstants.currentSample++;
    buildCommandBuffers();
  }

} // namespace ost
