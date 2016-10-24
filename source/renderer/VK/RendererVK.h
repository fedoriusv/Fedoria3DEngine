#pragma once

#include "renderer/Renderer.h"

#ifdef _VULKAN_RENDER_
#include "vulkan/vulkan.h"
#include "context/DeviceContextVK.h"

namespace v3d
{
namespace renderer
{
namespace vk
{

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    class MemoryManagerVK;
    class CommandBufferVK;
    class CommandPoolVK;

    /**
    * Inherited class for general render management. Vulkan render only.
    */
    class RendererVK final : public IRenderer
    {
    public:

        explicit RendererVK(const ContextPtr context);
        ~RendererVK();

        platform::ERenderType   getRenderType() const override;

        const ContextVKPtr      getVulkanContext() const;
        MemoryManagerVK*        getMemoryManager();
        CommandBufferVK*        getCurrentCommandBuffer() const;

    private:

        void                    immediateInit() override;

        void                    immediaterBeginFrame() override;
        void                    immediateEndFrame() override;
        void                    immediatePresentFrame() override;

        void                    immediateDraw() override;

        MemoryManagerVK*        m_memoryMamager;

        CommandPoolVK*          m_commandPool;

        VkDevice                m_device;
        u32                     m_queueFamilyIndex;

    public:

        void                preRender(bool clear = false)                                           override;
        void                postRender()                                                            override;



        ShaderPtr           makeSharedShader()                                                      override;
        ShaderProgramPtr    makeSharedProgram()                                                     override;
        GeometryPtr         makeSharedGeometry(const CRenderTechnique* technique)                   override;
        RenderStatePtr      makeSharedRenderState()                                                 override;

        RenderTargetPtr     makeSharedRenderTarget()                                                override;
        GeometryTargetPtr   makeSharedGeometryTarget()                                              override;
        void                resetTextures()                                                         override;

    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace vk
} //namespace renderer
} //namespace v3d

#endif //_VULKAN_RENDER_
