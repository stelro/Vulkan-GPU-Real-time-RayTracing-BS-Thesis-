/* =======================================================================
   $File: VulkanDevice.hh
   $Date: 3/6/2018
   $Revision:
   $Creator: Rostislav Orestis Stelmach
   $Notice:  This file is a part of Thesis project ( stracer ) for
                 the Technical Educational Institute of Western Macedonia
                 Supervisor: Dr. George Sisias
   ======================================================================== */

#pragma once


//Engine includes
#include "vulkan/vulkan.h"
#include "VulkanTools.hh"
#include "VulkanBuffer.hh"
#include "../core.hh"
//C++ includes
#include <algorithm>

namespace ost {

  struct VulkanDevice {


    explicit VulkanDevice(VkPhysicalDevice physicalDevice) {

      OST_ASSERT(physicalDevice);
      this->physicalDevice = physicalDevice;

      // Store Properties features, limits and properties of the physical device for later use
      // Device properties also contain limits and sparse properties
      vkGetPhysicalDeviceProperties(physicalDevice, &properties);

      vkGetPhysicalDeviceFeatures(physicalDevice, &features);

      // Memory properties are used regularly for creating all kinds of buffers
      vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memoryProperties);

      // Queue family properties, used for setting up requested queues upon device creation
      u32 queueFamilyCount;

      vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);
      OST_ASSERT(queueFamilyCount > 0);
      queueFamilyProperties.resize(queueFamilyCount);
      vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilyProperties.data());

      // Get list of supported extensions
      u32 extCount = 0;
      vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extCount, nullptr);
      if ( extCount > 0 ) {
        std::vector<VkExtensionProperties> extensions(extCount);
        if ( vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extCount, &extensions.front())
          == VK_SUCCESS ) {
          for ( auto ext : extensions ) {
            supportedExtensions.push_back(ext.extensionName);
          }
        }
      }
    }

    // Typecast to VkDevice
    operator VkDevice() { return logicalDevice; };

    ~VulkanDevice() {

      if ( commandPool ) {
        vkDestroyCommandPool(logicalDevice, commandPool, nullptr);
      }

      if ( logicalDevice ) {
        vkDestroyDevice(logicalDevice, nullptr);
      }
    }


    // Get the index of a memory type that has all the requested property bits set
    u32 getMemoryType(u32 typeBits, VkMemoryPropertyFlags properties, VkBool32 *memTypeFound = nullptr) {

      for ( u32 i = 0; i < memoryProperties.memoryTypeCount; i++ ) {
        if ((typeBits & 1) == 1 ) {
          if ((memoryProperties.memoryTypes[i].propertyFlags & properties) == properties ) {
            if ( memTypeFound ) {
              *memTypeFound = true;
            }
            return i;
          }
        }
        typeBits >>= 1;
      }

      OST_ASSERT_LOG(memTypeFound, "Could not fine a matching memory type");
      return 0;
    }

    // Get the index of a queue family that supports the requested queue flags
    u32 getQueueFamilyIndex(VkQueueFlagBits queueFlags) {

      // Dedicated queue for compute
      // Try to find a queue family index that supports compute but not graphics
      if ( queueFlags & VK_QUEUE_COMPUTE_BIT ) {
        for ( u32 i = 0; i < static_cast<u32>(queueFamilyProperties.size()); i++ ) {
          if ((queueFamilyProperties[i].queueFlags & queueFlags)
            && ((queueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) == 0)) {
            return i;
          }
        }
      }

      // Dedicated queue for transfer
      // Try to find a queue family index that supports transfer but not graphics and compute
      if ( queueFlags & VK_QUEUE_TRANSFER_BIT ) {
        for ( u32 i = 0; i < static_cast<u32>(queueFamilyProperties.size()); i++ ) {
          if ((queueFamilyProperties[i].queueFlags & queueFlags)
            && ((queueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) == 0)
            && ((queueFamilyProperties[i].queueFlags & VK_QUEUE_COMPUTE_BIT) == 0)) {
            return i;
          }
        }
      }

      // For other queue types or if no separate compute queue is present, return the first one to support the requested flags
      for ( u32 i = 0; i < static_cast<u32>(queueFamilyProperties.size()); i++ ) {
        if ( queueFamilyProperties[i].queueFlags & queueFlags ) {
          return i;
        }
      }

      OST_ASSERT_LOG(false, "Could not find a matching queue family index");
    }

    // Create the logical device based on the assigned physical device, also gets default queue family indices
    VkResult createLogicalDevice(VkPhysicalDeviceFeatures enabledFeatures,
                                 std::vector<const char *> enabledExtensions,
                                 bool useSwapChain = true,
                                 VkQueueFlags requestedQueueTypes = VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT) {

      std::vector<VkDeviceQueueCreateInfo> queueCreateInfos {};

      const float defaultQueuePriority(0.0f);

      // Graphics queue
      if ( requestedQueueTypes & VK_QUEUE_GRAPHICS_BIT ) {
        queueFamilyIndices.graphics = getQueueFamilyIndex(VK_QUEUE_GRAPHICS_BIT);
        VkDeviceQueueCreateInfo queueInfo{};
        queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueInfo.queueFamilyIndex = queueFamilyIndices.graphics;
        queueInfo.queueCount = 1;
        queueInfo.pQueuePriorities = &defaultQueuePriority;
        queueCreateInfos.push_back(queueInfo);
      } else {
        queueFamilyIndices.graphics = VK_NULL_HANDLE;
      }

      // Dedicated compute queue
      if ( requestedQueueTypes & VK_QUEUE_COMPUTE_BIT ) {
        queueFamilyIndices.compute = getQueueFamilyIndex(VK_QUEUE_COMPUTE_BIT);
        if ( queueFamilyIndices.compute != queueFamilyIndices.graphics ) {
          // If compute family index differs, we need an additional queue create info for the compute queue
          VkDeviceQueueCreateInfo queueInfo{};
          queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
          queueInfo.queueFamilyIndex = queueFamilyIndices.compute;
          queueInfo.queueCount = 1;
          queueInfo.pQueuePriorities = &defaultQueuePriority;
          queueCreateInfos.push_back(queueInfo);
        }
      } else {
        // Else we use the same queue
        queueFamilyIndices.compute = queueFamilyIndices.graphics;
      }

      // Dedicated transfer queue
      if ( requestedQueueTypes & VK_QUEUE_TRANSFER_BIT ) {
        queueFamilyIndices.transfer = getQueueFamilyIndex(VK_QUEUE_TRANSFER_BIT);
        if ((queueFamilyIndices.transfer != queueFamilyIndices.graphics)
          && (queueFamilyIndices.transfer != queueFamilyIndices.compute)) {
          // If compute family index differs, we need an additional queue create info for the compute queue
          VkDeviceQueueCreateInfo queueInfo{};
          queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
          queueInfo.queueFamilyIndex = queueFamilyIndices.transfer;
          queueInfo.queueCount = 1;
          queueInfo.pQueuePriorities = &defaultQueuePriority;
          queueCreateInfos.push_back(queueInfo);
        }
      } else {
        // Else we use the same queue
        queueFamilyIndices.transfer = queueFamilyIndices.graphics;
      }

      // Create the logical device representation
      std::vector<const char *> deviceExtensions(std::move(enabledExtensions));
      if ( useSwapChain ) {
        // If the device will be used for presenting to a display via a swapchain we need to request the swapchain extension
        deviceExtensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
      }

      VkDeviceCreateInfo deviceCreateInfo = {};
      deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
      deviceCreateInfo.queueCreateInfoCount = static_cast<u32>(queueCreateInfos.size());;
      deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();
      deviceCreateInfo.pEnabledFeatures = &enabledFeatures;


      if ( !deviceExtensions.empty() ) {
        deviceCreateInfo.enabledExtensionCount = ( u32 ) deviceExtensions.size();
        deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions.data();
      }

      VkResult result = vkCreateDevice(physicalDevice, &deviceCreateInfo, nullptr, &logicalDevice);

      if ( result == VK_SUCCESS ) {
        // Create a default command pool for graphics command buffers
        commandPool = createCommandPool(queueFamilyIndices.graphics);
      }

      this->enabledFeatures = enabledFeatures;

      return result;
    }

    // Create a buffer on the device
    VkResult createBuffer(VkBufferUsageFlags usageFlags,
                          VkMemoryPropertyFlags memoryPropertyFlags,
                          VkDeviceSize size,
                          VkBuffer *buffer,
                          VkDeviceMemory *memory,
                          void *data = nullptr) {

      // Create the buffer handle
      VkBufferCreateInfo bufferCreateInfo = ost::initializers::bufferCreateInfo(usageFlags, size);
      bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
      VK_CHECK_RESULT(vkCreateBuffer(logicalDevice, &bufferCreateInfo, nullptr, buffer));

      // Create the memory backing up the buffer handle
      VkMemoryRequirements memReqs;
      VkMemoryAllocateInfo memAlloc = ost::initializers::memoryAllocateInfo();
      vkGetBufferMemoryRequirements(logicalDevice, *buffer, &memReqs);
      memAlloc.allocationSize = memReqs.size;
      // Find a memory type index that fits the properties of the buffer
      memAlloc.memoryTypeIndex = getMemoryType(memReqs.memoryTypeBits, memoryPropertyFlags);
      VK_CHECK_RESULT(vkAllocateMemory(logicalDevice, &memAlloc, nullptr, memory));

      // If a pointer to the buffer data has been passed, map the buffer and copy over the data
      if ( data != nullptr ) {
        void *mapped;
        VK_CHECK_RESULT(vkMapMemory(logicalDevice, *memory, 0, size, 0, &mapped));
        memcpy(mapped, data, size);
        // If host coherency hasn't been requested, do a manual flush to make writes visible
        if ((memoryPropertyFlags & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) == 0 ) {
          VkMappedMemoryRange mappedRange = ost::initializers::mappedMemoryRange();
          mappedRange.memory = *memory;
          mappedRange.offset = 0;
          mappedRange.size = size;
          vkFlushMappedMemoryRanges(logicalDevice, 1, &mappedRange);
        }
        vkUnmapMemory(logicalDevice, *memory);
      }

      // Attach the memory to the buffer object
      VK_CHECK_RESULT(vkBindBufferMemory(logicalDevice, *buffer, *memory, 0));
      return VK_SUCCESS;
    }

    // Create a buffer on the device ( Overloaded function to take VulkanBuffer )
    VkResult createBuffer(VkBufferUsageFlags usageFlags,
                          VkMemoryPropertyFlags memoryPropertyFlags,
                          ost::VulkanBuffer *buffer,
                          VkDeviceSize size,
                          void *data = nullptr) {

      buffer->device = logicalDevice;

      // Create the buffer handle
      VkBufferCreateInfo bufferCreateInfo = ost::initializers::bufferCreateInfo(usageFlags, size);
      VK_CHECK_RESULT(vkCreateBuffer(logicalDevice, &bufferCreateInfo, nullptr, &buffer->buffer));

      // Create the memory backing up the buffer handle
      VkMemoryRequirements memReqs;
      VkMemoryAllocateInfo memAlloc = ost::initializers::memoryAllocateInfo();
      vkGetBufferMemoryRequirements(logicalDevice, buffer->buffer, &memReqs);
      memAlloc.allocationSize = memReqs.size;
      // Find a memory type index that fits the properties of the buffer
      memAlloc.memoryTypeIndex = getMemoryType(memReqs.memoryTypeBits, memoryPropertyFlags);
      VK_CHECK_RESULT(vkAllocateMemory(logicalDevice, &memAlloc, nullptr, &buffer->memory));

      buffer->alignment = memReqs.alignment;
      buffer->size = memAlloc.allocationSize;
      buffer->usageFlags = usageFlags;
      buffer->memoryPropertyFlags = memoryPropertyFlags;

      // If a pointer to the buffer data has been passed, map the buffer and copy over the data
      if ( data != nullptr ) {
        VK_CHECK_RESULT(buffer->map());
        memcpy(buffer->mapped, data, size);
        buffer->unmap();
      }

      // Initialize a default descriptor that covers the whole buffer size
      buffer->setupDescriptor();

      // Attach the memory to the buffer object
      return buffer->bind();
    }

    // Copy buffer data from src to dst using VkCmdCopyBuffer
    void copyBuffer(ost::VulkanBuffer *src, ost::VulkanBuffer *dst, VkQueue queue, VkBufferCopy *copyRegion = nullptr) {

      OST_ASSERT(dst->size <= src->size);
      OST_ASSERT(src->buffer);
      VkCommandBuffer copyCmd = createCommandBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY, true);
      VkBufferCopy bufferCopy{};
      if ( copyRegion == nullptr ) {
        bufferCopy.size = src->size;
      } else {
        bufferCopy = *copyRegion;
      }

      vkCmdCopyBuffer(copyCmd, src->buffer, dst->buffer, 1, &bufferCopy);

      flushCommandBuffer(copyCmd, queue);
    }

    // Create a command pool for allocation command buffers from
    VkCommandPool createCommandPool(u32 queueFamilyIndex,
                                    VkCommandPoolCreateFlags createFlags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT) {

      VkCommandPoolCreateInfo cmdPoolInfo = {};
      cmdPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
      cmdPoolInfo.queueFamilyIndex = queueFamilyIndex;
      cmdPoolInfo.flags = createFlags;
      VkCommandPool cmdPool;
      VK_CHECK_RESULT(vkCreateCommandPool(logicalDevice, &cmdPoolInfo, nullptr, &cmdPool));
      return cmdPool;
    }

    // Allocate a command buffer from the command pool
    VkCommandBuffer createCommandBuffer(VkCommandBufferLevel level, bool begin = false) {

      VkCommandBufferAllocateInfo
        cmdBufAllocateInfo = ost::initializers::commandBufferAllocateInfo(commandPool, level, 1);

      VkCommandBuffer cmdBuffer;
      VK_CHECK_RESULT(vkAllocateCommandBuffers(logicalDevice, &cmdBufAllocateInfo, &cmdBuffer));

      // If requested, also start recording for the new command buffer
      if ( begin ) {
        VkCommandBufferBeginInfo cmdBufInfo = ost::initializers::commandBufferBeginInfo();
        VK_CHECK_RESULT(vkBeginCommandBuffer(cmdBuffer, &cmdBufInfo));
      }

      return cmdBuffer;
    }

    // Finish command buffer recording and submit it to a queue
    void flushCommandBuffer(VkCommandBuffer commandBuffer, VkQueue queue, bool free = true) {

      if ( commandBuffer == VK_NULL_HANDLE ) {
        return;
      }

      VK_CHECK_RESULT(vkEndCommandBuffer(commandBuffer));

      VkSubmitInfo submitInfo = ost::initializers::submitInfo();
      submitInfo.commandBufferCount = 1;
      submitInfo.pCommandBuffers = &commandBuffer;

      // Create fence to ensure that the command buffer has finished executing
      VkFenceCreateInfo fenceInfo = ost::initializers::fenceCreateInfo(VK_FLAGS_NONE);
      VkFence fence;
      VK_CHECK_RESULT(vkCreateFence(logicalDevice, &fenceInfo, nullptr, &fence));

      // Submit to the queue
      VK_CHECK_RESULT(vkQueueSubmit(queue, 1, &submitInfo, fence));
      // Wait for the fence to signal that command buffer has finished executing
      VK_CHECK_RESULT(vkWaitForFences(logicalDevice, 1, &fence, VK_TRUE, DEFAULT_FENCE_TIMEOUT));

      vkDestroyFence(logicalDevice, fence, nullptr);

      if ( free ) {
        vkFreeCommandBuffers(logicalDevice, commandPool, 1, &commandBuffer);
      }
    }

    // Check if an extension is supported by the (physical device)
    bool extensionSupported(const std::string& extension) {

      return (std::find(supportedExtensions.begin(), supportedExtensions.end(), extension)
        != supportedExtensions.end());
    }

    VkPhysicalDevice                                physicalDevice;
    VkDevice                                        logicalDevice;
    VkPhysicalDeviceProperties                      properties;
    VkPhysicalDeviceFeatures                        features;
    VkPhysicalDeviceFeatures                        enabledFeatures;
    VkPhysicalDeviceMemoryProperties                memoryProperties;
    std::vector<VkQueueFamilyProperties>            queueFamilyProperties;
    std::vector<std::string>                        supportedExtensions;

    VkCommandPool                                   commandPool = VK_NULL_HANDLE;

    struct {
      u32 graphics;
      u32 compute;
      u32 transfer;
    } queueFamilyIndices;

  };
}
