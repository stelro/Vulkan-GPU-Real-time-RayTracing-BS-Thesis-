/* =======================================================================
   $File: sfs.hh
   $Date: 3/6/2018
   $Revision:
   $Creator: Rostislav Orestis Stelmach
   $Notice:  This file is a part of Thesis project ( stracer ) for
                 the Technical Educational Institute of Western Macedonia
                 Supervisor: Dr. George Sisias
   ======================================================================== */

#pragma once

//Engine Includes
#include "VulkanTools.hh"
#include "VulkanBuffer.hh"
#include "VulkanDevice.hh"
#include "../Math/Vector.hh"

//C++ Includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <sstream>
#include <iomanip>
//Vulkan Includes
#include <vulkan/vulkan.h>
// ImGUI ( external ) includes
#include "../../external/imgui/imgui.h"

namespace ost {
  struct UIOverlayCreateInfo {
    ost::VulkanDevice *device;
    VkQueue copyQueue;
    VkRenderPass renderPass;
    std::vector<VkFramebuffer> framebuffers;
    VkFormat colorformat;
    VkFormat depthformat;
    u32 width;
    u32 height;
    std::vector<VkPipelineShaderStageCreateInfo> shaders;
    VkSampleCountFlagBits rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    u32 subpassCount = 1;
    std::vector<VkClearValue> clearValues = {};
    u32 attachmentCount = 1;
  };

  class UIOverlay {
  private:
    ost::VulkanBuffer vertexBuffer;
    ost::VulkanBuffer indexBuffer;
    int32_t vertexCount = 0;
    int32_t indexCount = 0;

    VkDescriptorPool descriptorPool;
    VkDescriptorSetLayout descriptorSetLayout;
    VkDescriptorSet descriptorSet;
    VkPipelineLayout pipelineLayout;
    VkPipelineCache pipelineCache;
    VkPipeline pipeline;
    VkRenderPass renderPass;
    VkCommandPool commandPool;
    VkFence fence;

    VkDeviceMemory fontMemory = VK_NULL_HANDLE;
    VkImage fontImage = VK_NULL_HANDLE;
    VkImageView fontView = VK_NULL_HANDLE;
    VkSampler sampler;

    struct PushConstBlock {
      ost::Vec2 scale;
      ost::Vec2 translate;
    } pushConstBlock;

    UIOverlayCreateInfo createInfo = {};

    void prepareResources() noexcept;
    void preparePipeline() noexcept;
    void prepareRenderPass() noexcept;
    void updateCommandBuffers() noexcept;
  public:
    bool visible = true;
    float scale = 1.0f;

    std::vector<VkCommandBuffer> cmdBuffers;

    explicit UIOverlay(ost::UIOverlayCreateInfo createInfo) noexcept;
    ~UIOverlay() noexcept;

    OST_DISABLE_COPY(UIOverlay)

    void update() noexcept;
    void resize(u32 width, u32 height,const std::vector<VkFramebuffer>& framebuffers) noexcept;

    void submit(VkQueue queue, u32 bufferindex, VkSubmitInfo submitInfo) noexcept;

    bool header(const char *caption) noexcept;
    bool button(const char *caption) noexcept;
    void text(const char *formatstr, ...) noexcept;
  };
}