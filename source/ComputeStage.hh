/* =======================================================================
   $File: ComputeStage.hh
   $Date: 17/6/2018	
   $Revision: 
   $Creator: Rostislav Orestis Stelmach
   $Notice:  This file is a part of Thesis project ( stracer ) for 
                 the Technical Educational Institute of Western Macedonia
                 Supervisor: Dr. George Sisias
   ======================================================================== */


#pragma once

// Engine Includes
#include "StageBase.hh"
#include "Vulkan/VulkanBuffer.hh"
#include "core.hh"
#include "Math/Vector.hh"
#include "Math/Matrix.hh"
#include "Geometry/Sphere.hh"
#include "json/Scene.hh"
// C++ Includes
#include <array>
#include <memory>

/*
 * This is the compute stage of the ray-tracing engine.
 * We are using compute shader to generate the raytraced image
 * and then we save it a texture ( actually image sampler in shader )
 * we cannot portray directly image from the compute shader into the screen,
 * thus we need to transfer the generated image to the graphics stage
 */

namespace ost {

  class Texture;
  class IOManager;

  class ComputeStage : public StageBase {
  private:

    //Uniform Buffer struct layout, used by the compute shader
    struct UniformBufferObject {

      alignas(4) int        anyKeyIsPressed         = false;
      alignas(4) int        spheres_are_enabled     = false;
      alignas(4) int        planes_are_enabled      = false;
      alignas(4) int        rects_are_enabled       = false;
      alignas(4) int        boxes_are_enabled       = false;

      float                 randomSeed              = 0.0f;
      float                 aaStrength              = 0.10f;
      u32                   samplesPerPixel         = 8;
      u32                   rayDepth                = 8;

      alignas(16) ost::Vec3 ambient                 = ost::Vec3{1.0f};

      alignas(16) ost::Vec3 origin                  = { -2.0f, 2.0f, 1.0f };
      alignas(16) ost::Vec3 lookAt                  = { 0.0f, 1.0f, 0.0f };
      float                 fov                     = 60.0f;

    } m_ubo;

    // Because the queue may differ from the one used for the graphics stage
    // we separate the queues for compute commands and graphics commands
    VkQueue                         m_queue;

    // As with the queue, commandpools may be differ for the graphics stage
    // so we use a separated command pool for the compute stage
    VkCommandPool                   m_commandPool;

    //Command Buffer used for storing the dispatch commands and barriers
    VkCommandBuffer                 m_commandBuffer;

    //Synchronization fence to avoid rewriting compute command buffer
    //if still in use
    VkFence                         m_fence;

    // Compute shader binding layout
    VkDescriptorSetLayout           m_descriptorSetLayout;

    // Compute shader bindings
    VkDescriptorSet                 m_descriptorSet;

    // Layout of the compute pipeline
    // ( compute pipleine cannot be displayed to the screen, thus we write all the
    // results from the raytraced image to a texture, and then we use graphics layout
    // to display the resulting image )
    VkPipelineLayout                m_pipelineLayout;

    // Compute raytracing pipeline
    VkPipeline                      m_pipeline;

    // Texture pointer, the raytraced image will be saved here
    Texture*                        m_texture                                   = nullptr;

    //Spheres storage buffer, for the compute shader stage
    ost::VulkanBuffer               m_spheresBuffer;
    ost::VulkanBuffer               m_planesBuffer;
    ost::VulkanBuffer               m_rectanglesBuffer;
    ost::VulkanBuffer               m_boxesBuffer;
    ost::VulkanBuffer               m_randomSamples;
    ost::VulkanBuffer               m_uniformBuffer;

    IOManager*                      m_iomanager                                 = nullptr;
    Scene*                          m_scene                                     = nullptr;

  public:

    explicit                        ComputeStage(Texture *texture)              noexcept;
                                    ~ComputeStage()                             noexcept override;

    void                            initialize()                                noexcept override;
    void                            setupPipeline()                             noexcept override;
    void                            setupDescriptorSet()                        noexcept override;
    void                            setupDescriptorSetLayout()                  noexcept override;
    void                            buildCommandBuffers()                       noexcept override;
    void                            updateUniformBuffers()                      noexcept;
    void                            prepareStorageBuffers()                     noexcept override;
    void                            prepareUniformBuffers()                     noexcept override;

    void                            render(float dt)                            noexcept override;
    void                            update(float dt)                            noexcept override;

    float                           getAAStrength()                             const noexcept;
    u32                             getRayTraceDepth()                          const noexcept;
    u32                             getSamplesPerPixel()                        const noexcept;
    u32                             getNumberOfShapes()                         const noexcept;
    float                           getFieldOfView()                            const noexcept;
    std::array<float, 3>            getCameraOrigin()                           const noexcept;
    std::array<float, 3>            getCameraLookAt()                           const noexcept;

    void                            setAAStrength(float value)                  noexcept;
    void                            setRayTraceDepth(u32 value)                 noexcept;
    void                            setSmaplesPerPixels(u32 value)              noexcept;
    void                            setFieldOfView(float fov)                   noexcept;
    void                            setCameraOrigin(std::array<float, 3> arr)   noexcept;
    void                            setCameraLookAt(std::array<float, 3> arr)   noexcept;
    void                            setAmbientColor(const ost::Vec3& value)     noexcept;

    void                            setScene(Scene* s)                          noexcept;

  };

}

