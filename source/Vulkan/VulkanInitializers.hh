/* =======================================================================
   $File: VulkanInitializers.hh
   $Date: 3/6/2018
   $Revision:
   $Creator: Rostislav Orestis Stelmach
   $Notice:  This file is a part of Thesis project ( stracer ) for
                 the Technical Educational Institute of Western Macedonia
                 Supervisor: Dr. George Sisias
   ======================================================================== */

#pragma once

// Engine Includes
#include "vulkan/vulkan.h"
#include "../core.hh"
// C++ Includes
#include <vector>

namespace ost {
  namespace initializers {

    inline VkMemoryAllocateInfo memoryAllocateInfo() {

      VkMemoryAllocateInfo memAllocInfo{};
      memAllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
      return memAllocInfo;
    }

    inline VkMappedMemoryRange mappedMemoryRange() {

      VkMappedMemoryRange mappedMemoryRange{};
      mappedMemoryRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
      return mappedMemoryRange;
    }

    inline VkCommandBufferAllocateInfo commandBufferAllocateInfo(
      VkCommandPool commandPool,
      VkCommandBufferLevel level,
      u32 bufferCount) {

      VkCommandBufferAllocateInfo commandBufferAllocateInfo{};
      commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
      commandBufferAllocateInfo.commandPool = commandPool;
      commandBufferAllocateInfo.level = level;
      commandBufferAllocateInfo.commandBufferCount = bufferCount;
      return commandBufferAllocateInfo;
    }

    inline VkCommandBufferBeginInfo commandBufferBeginInfo() {

      VkCommandBufferBeginInfo cmdBufferBeginInfo{};
      cmdBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
      return cmdBufferBeginInfo;
    }

    inline VkRenderPassBeginInfo renderPassBeginInfo() {

      VkRenderPassBeginInfo renderPassBeginInfo{};
      renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
      return renderPassBeginInfo;
    }

    inline VkImageMemoryBarrier imageMemoryBarrier() {

      VkImageMemoryBarrier imageMemoryBarrier{};
      imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
      imageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
      imageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
      return imageMemoryBarrier;
    }

    inline VkImageCreateInfo imageCreateInfo() {

      VkImageCreateInfo imageCreateInfo{};
      imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
      return imageCreateInfo;
    }

    inline VkSamplerCreateInfo samplerCreateInfo() {

      VkSamplerCreateInfo samplerCreateInfo{};
      samplerCreateInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
      samplerCreateInfo.maxAnisotropy = 1.0f;
      return samplerCreateInfo;
    }

    inline VkImageViewCreateInfo imageViewCreateInfo() {

      VkImageViewCreateInfo imageViewCreateInfo{};
      imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
      return imageViewCreateInfo;
    }


    inline VkSemaphoreCreateInfo semaphoreCreateInfo() {

      VkSemaphoreCreateInfo semaphoreCreateInfo{};
      semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
      return semaphoreCreateInfo;
    }

    inline VkFenceCreateInfo fenceCreateInfo(VkFenceCreateFlags flags = 0) {

      VkFenceCreateInfo fenceCreateInfo{};
      fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
      fenceCreateInfo.flags = flags;
      return fenceCreateInfo;
    }


    inline VkSubmitInfo submitInfo() {

      VkSubmitInfo submitInfo{};
      submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
      return submitInfo;
    }

    inline VkViewport viewport(
      float width,
      float height,
      float minDepth,
      float maxDepth) {

      VkViewport viewport{};
      viewport.width = width;
      viewport.height = height;
      viewport.minDepth = minDepth;
      viewport.maxDepth = maxDepth;
      return viewport;
    }

    inline VkRect2D rect2D(
      int32_t width,
      int32_t height,
      int32_t offsetX,
      int32_t offsetY) {

      VkRect2D rect2D{};
      rect2D.extent.width = width;
      rect2D.extent.height = height;
      rect2D.offset.x = offsetX;
      rect2D.offset.y = offsetY;
      return rect2D;
    }

    inline VkBufferCreateInfo bufferCreateInfo(
      VkBufferUsageFlags usage,
      VkDeviceSize size) {

      VkBufferCreateInfo bufCreateInfo{};
      bufCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
      bufCreateInfo.usage = usage;
      bufCreateInfo.size = size;
      return bufCreateInfo;
    }

    inline VkDescriptorPoolCreateInfo descriptorPoolCreateInfo(
      u32 poolSizeCount,
      VkDescriptorPoolSize *pPoolSizes,
      u32 maxSets) {

      VkDescriptorPoolCreateInfo descriptorPoolInfo{};
      descriptorPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
      descriptorPoolInfo.poolSizeCount = poolSizeCount;
      descriptorPoolInfo.pPoolSizes = pPoolSizes;
      descriptorPoolInfo.maxSets = maxSets;
      return descriptorPoolInfo;
    }

    inline VkDescriptorPoolCreateInfo descriptorPoolCreateInfo(
      const std::vector<VkDescriptorPoolSize> &poolSizes,
      u32 maxSets) {

      VkDescriptorPoolCreateInfo descriptorPoolInfo{};
      descriptorPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
      descriptorPoolInfo.poolSizeCount = static_cast<u32>(poolSizes.size());
      descriptorPoolInfo.pPoolSizes = poolSizes.data();
      descriptorPoolInfo.maxSets = maxSets;
      return descriptorPoolInfo;
    }

    inline VkDescriptorPoolSize descriptorPoolSize(
      VkDescriptorType type,
      u32 descriptorCount) {

      VkDescriptorPoolSize descriptorPoolSize{};
      descriptorPoolSize.type = type;
      descriptorPoolSize.descriptorCount = descriptorCount;
      return descriptorPoolSize;
    }

    inline VkDescriptorSetLayoutBinding descriptorSetLayoutBinding(
      VkDescriptorType type,
      VkShaderStageFlags stageFlags,
      u32 binding,
      u32 descriptorCount = 1) {

      VkDescriptorSetLayoutBinding setLayoutBinding{};
      setLayoutBinding.descriptorType = type;
      setLayoutBinding.stageFlags = stageFlags;
      setLayoutBinding.binding = binding;
      setLayoutBinding.descriptorCount = descriptorCount;
      return setLayoutBinding;
    }

    inline VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo(
      const VkDescriptorSetLayoutBinding *pBindings,
      u32 bindingCount) {

      VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo{};
      descriptorSetLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
      descriptorSetLayoutCreateInfo.pBindings = pBindings;
      descriptorSetLayoutCreateInfo.bindingCount = bindingCount;
      return descriptorSetLayoutCreateInfo;
    }

    inline VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo(
      const std::vector<VkDescriptorSetLayoutBinding> &bindings) {

      VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo{};
      descriptorSetLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
      descriptorSetLayoutCreateInfo.pBindings = bindings.data();
      descriptorSetLayoutCreateInfo.bindingCount = static_cast<u32>(bindings.size());
      return descriptorSetLayoutCreateInfo;
    }

    inline VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo(
      const VkDescriptorSetLayout *pSetLayouts,
      u32 setLayoutCount = 1) {

      VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo{};
      pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
      pipelineLayoutCreateInfo.setLayoutCount = setLayoutCount;
      pipelineLayoutCreateInfo.pSetLayouts = pSetLayouts;
      return pipelineLayoutCreateInfo;
    }

    inline VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo(
      u32 setLayoutCount = 1) {

      VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo{};
      pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
      pipelineLayoutCreateInfo.setLayoutCount = setLayoutCount;
      return pipelineLayoutCreateInfo;
    }

    inline VkDescriptorSetAllocateInfo descriptorSetAllocateInfo(
      VkDescriptorPool descriptorPool,
      const VkDescriptorSetLayout *pSetLayouts,
      u32 descriptorSetCount) {

      VkDescriptorSetAllocateInfo descriptorSetAllocateInfo{};
      descriptorSetAllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
      descriptorSetAllocateInfo.descriptorPool = descriptorPool;
      descriptorSetAllocateInfo.pSetLayouts = pSetLayouts;
      descriptorSetAllocateInfo.descriptorSetCount = descriptorSetCount;
      return descriptorSetAllocateInfo;
    }

    inline VkDescriptorImageInfo descriptorImageInfo(VkSampler sampler,
                                                     VkImageView imageView,
                                                     VkImageLayout imageLayout) {

      VkDescriptorImageInfo descriptorImageInfo{};
      descriptorImageInfo.sampler = sampler;
      descriptorImageInfo.imageView = imageView;
      descriptorImageInfo.imageLayout = imageLayout;
      return descriptorImageInfo;
    }

    inline VkWriteDescriptorSet writeDescriptorSet(
      VkDescriptorSet dstSet,
      VkDescriptorType type,
      u32 binding,
      VkDescriptorBufferInfo *bufferInfo,
      u32 descriptorCount = 1) {

      VkWriteDescriptorSet writeDescriptorSet{};
      writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
      writeDescriptorSet.dstSet = dstSet;
      writeDescriptorSet.descriptorType = type;
      writeDescriptorSet.dstBinding = binding;
      writeDescriptorSet.pBufferInfo = bufferInfo;
      writeDescriptorSet.descriptorCount = descriptorCount;
      return writeDescriptorSet;
    }

    inline VkWriteDescriptorSet writeDescriptorSet(
      VkDescriptorSet dstSet,
      VkDescriptorType type,
      u32 binding,
      VkDescriptorImageInfo *imageInfo,
      u32 descriptorCount = 1) {

      VkWriteDescriptorSet writeDescriptorSet{};
      writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
      writeDescriptorSet.dstSet = dstSet;
      writeDescriptorSet.descriptorType = type;
      writeDescriptorSet.dstBinding = binding;
      writeDescriptorSet.pImageInfo = imageInfo;
      writeDescriptorSet.descriptorCount = descriptorCount;
      return writeDescriptorSet;
    }

    inline VkVertexInputBindingDescription vertexInputBindingDescription(
      u32 binding,
      u32 stride,
      VkVertexInputRate inputRate) {

      VkVertexInputBindingDescription vInputBindDescription{};
      vInputBindDescription.binding = binding;
      vInputBindDescription.stride = stride;
      vInputBindDescription.inputRate = inputRate;
      return vInputBindDescription;
    }

    inline VkVertexInputAttributeDescription vertexInputAttributeDescription(
      u32 binding,
      u32 location,
      VkFormat format,
      u32 offset) {

      VkVertexInputAttributeDescription vInputAttribDescription{};
      vInputAttribDescription.location = location;
      vInputAttribDescription.binding = binding;
      vInputAttribDescription.format = format;
      vInputAttribDescription.offset = offset;
      return vInputAttribDescription;
    }

    inline VkPipelineVertexInputStateCreateInfo pipelineVertexInputStateCreateInfo() {

      VkPipelineVertexInputStateCreateInfo pipelineVertexInputStateCreateInfo{};
      pipelineVertexInputStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
      return pipelineVertexInputStateCreateInfo;
    }

    inline VkPipelineInputAssemblyStateCreateInfo pipelineInputAssemblyStateCreateInfo(
      VkPrimitiveTopology topology,
      VkPipelineInputAssemblyStateCreateFlags flags,
      VkBool32 primitiveRestartEnable) {

      VkPipelineInputAssemblyStateCreateInfo pipelineInputAssemblyStateCreateInfo{};
      pipelineInputAssemblyStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
      pipelineInputAssemblyStateCreateInfo.topology = topology;
      pipelineInputAssemblyStateCreateInfo.flags = flags;
      pipelineInputAssemblyStateCreateInfo.primitiveRestartEnable = primitiveRestartEnable;
      return pipelineInputAssemblyStateCreateInfo;
    }

    inline VkPipelineRasterizationStateCreateInfo pipelineRasterizationStateCreateInfo(
      VkPolygonMode polygonMode,
      VkCullModeFlags cullMode,
      VkFrontFace frontFace,
      VkPipelineRasterizationStateCreateFlags flags = 0) {

      VkPipelineRasterizationStateCreateInfo pipelineRasterizationStateCreateInfo{};
      pipelineRasterizationStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
      pipelineRasterizationStateCreateInfo.polygonMode = polygonMode;
      pipelineRasterizationStateCreateInfo.cullMode = cullMode;
      pipelineRasterizationStateCreateInfo.frontFace = frontFace;
      pipelineRasterizationStateCreateInfo.flags = flags;
      pipelineRasterizationStateCreateInfo.depthClampEnable = VK_FALSE;
      pipelineRasterizationStateCreateInfo.lineWidth = 1.0f;
      return pipelineRasterizationStateCreateInfo;
    }

    inline VkPipelineColorBlendStateCreateInfo pipelineColorBlendStateCreateInfo(
      u32 attachmentCount,
      const VkPipelineColorBlendAttachmentState *pAttachments) {

      VkPipelineColorBlendStateCreateInfo pipelineColorBlendStateCreateInfo{};
      pipelineColorBlendStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
      pipelineColorBlendStateCreateInfo.attachmentCount = attachmentCount;
      pipelineColorBlendStateCreateInfo.pAttachments = pAttachments;
      return pipelineColorBlendStateCreateInfo;
    }

    inline VkPipelineDepthStencilStateCreateInfo pipelineDepthStencilStateCreateInfo(
      VkBool32 depthTestEnable,
      VkBool32 depthWriteEnable,
      VkCompareOp depthCompareOp) {

      VkPipelineDepthStencilStateCreateInfo pipelineDepthStencilStateCreateInfo{};
      pipelineDepthStencilStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
      pipelineDepthStencilStateCreateInfo.depthTestEnable = depthTestEnable;
      pipelineDepthStencilStateCreateInfo.depthWriteEnable = depthWriteEnable;
      pipelineDepthStencilStateCreateInfo.depthCompareOp = depthCompareOp;
      pipelineDepthStencilStateCreateInfo.front = pipelineDepthStencilStateCreateInfo.back;
      pipelineDepthStencilStateCreateInfo.back.compareOp = VK_COMPARE_OP_ALWAYS;
      return pipelineDepthStencilStateCreateInfo;
    }

    inline VkPipelineViewportStateCreateInfo pipelineViewportStateCreateInfo(
      u32 viewportCount,
      u32 scissorCount,
      VkPipelineViewportStateCreateFlags flags = 0) {

      VkPipelineViewportStateCreateInfo pipelineViewportStateCreateInfo{};
      pipelineViewportStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
      pipelineViewportStateCreateInfo.viewportCount = viewportCount;
      pipelineViewportStateCreateInfo.scissorCount = scissorCount;
      pipelineViewportStateCreateInfo.flags = flags;
      return pipelineViewportStateCreateInfo;
    }

    inline VkPipelineMultisampleStateCreateInfo pipelineMultisampleStateCreateInfo(
      VkSampleCountFlagBits rasterizationSamples,
      VkPipelineMultisampleStateCreateFlags flags = 0) {

      VkPipelineMultisampleStateCreateInfo pipelineMultisampleStateCreateInfo{};
      pipelineMultisampleStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
      pipelineMultisampleStateCreateInfo.rasterizationSamples = rasterizationSamples;
      pipelineMultisampleStateCreateInfo.flags = flags;
      return pipelineMultisampleStateCreateInfo;
    }

    inline VkPipelineDynamicStateCreateInfo pipelineDynamicStateCreateInfo(
      const VkDynamicState *pDynamicStates,
      u32 dynamicStateCount,
      VkPipelineDynamicStateCreateFlags flags = 0) {

      VkPipelineDynamicStateCreateInfo pipelineDynamicStateCreateInfo{};
      pipelineDynamicStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
      pipelineDynamicStateCreateInfo.pDynamicStates = pDynamicStates;
      pipelineDynamicStateCreateInfo.dynamicStateCount = dynamicStateCount;
      pipelineDynamicStateCreateInfo.flags = flags;
      return pipelineDynamicStateCreateInfo;
    }

    inline VkPipelineDynamicStateCreateInfo pipelineDynamicStateCreateInfo(
      const std::vector<VkDynamicState> &pDynamicStates,
      VkPipelineDynamicStateCreateFlags flags = 0) {

      VkPipelineDynamicStateCreateInfo pipelineDynamicStateCreateInfo{};
      pipelineDynamicStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
      pipelineDynamicStateCreateInfo.pDynamicStates = pDynamicStates.data();
      pipelineDynamicStateCreateInfo.dynamicStateCount = static_cast<u32>(pDynamicStates.size());
      pipelineDynamicStateCreateInfo.flags = flags;
      return pipelineDynamicStateCreateInfo;
    }

    inline VkGraphicsPipelineCreateInfo pipelineCreateInfo(
      VkPipelineLayout layout,
      VkRenderPass renderPass,
      VkPipelineCreateFlags flags = 0) {

      VkGraphicsPipelineCreateInfo pipelineCreateInfo{};
      pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
      pipelineCreateInfo.layout = layout;
      pipelineCreateInfo.renderPass = renderPass;
      pipelineCreateInfo.flags = flags;
      pipelineCreateInfo.basePipelineIndex = -1;
      pipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;
      return pipelineCreateInfo;
    }

    inline VkComputePipelineCreateInfo computePipelineCreateInfo(
      VkPipelineLayout layout,
      VkPipelineCreateFlags flags = 0) {

      VkComputePipelineCreateInfo computePipelineCreateInfo{};
      computePipelineCreateInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
      computePipelineCreateInfo.layout = layout;
      computePipelineCreateInfo.flags = flags;
      return computePipelineCreateInfo;
    }

    inline VkPushConstantRange pushConstantRange(
      VkShaderStageFlags stageFlags,
      u32 size,
      u32 offset) {

      VkPushConstantRange pushConstantRange{};
      pushConstantRange.stageFlags = stageFlags;
      pushConstantRange.offset = offset;
      pushConstantRange.size = size;
      return pushConstantRange;
    }
  }
}