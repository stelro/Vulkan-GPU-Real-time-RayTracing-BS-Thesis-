/* =======================================================================
   $File: GraphicsStage.hh
   $Date: 17/6/2018	
   $Revision: 
   $Creator: Rostislav Orestis Stelmach
   $Notice:  This file is a part of Thesis project ( stracer ) for 
                 the Technical Educational Institute of Western Macedonia
                 Supervisor: Dr. George Sisias
   ======================================================================== */


#pragma once

#include "StageBase.hh"
#include "core.hh"
#include <vector>
#include <functional>

/*
 * This is the graphics stage part of the raytracing engine.
 * It is used to portray the raytraced image that has been
 * generated from the compute shader, through texture, that's why
 * we are using image sampler in the shaders.
 */

namespace ost {

  class Texture;
  class VulkanEngine;
  class IOManager;

  class GraphicsStage : public StageBase {
  public:

    explicit                     GraphicsStage(Texture *texture)    noexcept;
                                ~GraphicsStage()                    noexcept override;

                                OST_DISABLE_MOVE(GraphicsStage)
                                OST_DISABLE_COPY(GraphicsStage)

    void                        initialize()                        noexcept override;
    void                        setupPipeline()                     noexcept override;
    void                        setupDescriptorSet()                noexcept override;
    void                        setupDescriptorSetLayout()          noexcept override;
    void                        buildCommandBuffers()               noexcept override;
    void                        prepareStorageBuffers()             noexcept override;
    void                        prepareUniformBuffers()             noexcept override;
    void                        update(float dt)                    noexcept override;
    void                        render(float dt)                    noexcept override;

  private:

    struct alignas(8) PushConstants {
      float             currentSample       = 0.0f;
      int               anyKeyPressed       = false;
    } m_pushConstants ;

    /*
     * Descriptor set layout for the raytraced image to display in
     * shader binding layout
     */
    VkDescriptorSetLayout       m_descriptorSetLayout;

    /*
     * Descriptor set for the raytraced image to display before
     * compute shader image manipulation
     */
    VkDescriptorSet             m_descriptorSetPreCompute;

    /*
     * Descriptor set for the raytraced iamge to display after
     * compute shader iamge manimpulation
     */
    VkDescriptorSet             m_descriptorSetPostCompute;

    /*
     * pipeline to handle the actuall graphics that will be
     * displayed on the screen ( in our case the raytraced image that generated
     * from the comptue stage)
     */
    VkPipeline                  m_pipeline;

    /*
     * Layout of the grphics pipeline
     */
    VkPipelineLayout            m_pipelineLayout;

    /*
     * Texture pointer, the raytraced image will be saved here
     */
    Texture*                    m_texture                   = nullptr;
    IOManager*                  m_iomanager                 = nullptr;

  };

}

