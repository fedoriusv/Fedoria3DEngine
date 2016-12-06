#pragma once

#include "renderer/Renderer.h"

#ifdef _VULKAN_RENDER_
#include "vulkan/vulkan.h"
#include "context/SwapChainVK.h"

namespace v3d
{
namespace renderer
{
namespace vk
{

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    class RenderStateVK;
    class MemoryManagerVK;
    class CommandBufferVK;
    class CommandPoolVK;
    class FramebufferVK;
    class GeometryVK;
    class ShaderProgramVK;
    class FenceVK;

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    const std::array<VkDynamicState, 3> k_dynamicStateVK =
    {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR,
        VK_DYNAMIC_STATE_STENCIL_REFERENCE,
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    /**
    * Inherited class for general render management. Vulkan render only.
    */
    class RendererVK final : public IRenderer
    {
    public:

        explicit RendererVK(const ContextPtr context);
        ~RendererVK();

        ERenderType                 getRenderType() const override;

        const ContextVKPtr          getVulkanContext() const;
        MemoryManagerVK*            getMemoryManager();
        CommandBufferVK*            getCurrentCommandBuffer() const;

        template <class V, typename T>
        static bool                 checkPresentState(const V& container, T value)
        {
            /*auto iter = std::find(container.cbegin(), container.cend(), T);
            if (iter != container.cend())
            {
                return true;
            }*/

            return false;
        }

    private:

        void                        immediateInit() override;
        void                        immediateTerminate() override;

        void                        immediaterBeginFrame() override;
        void                        immediateEndFrame() override;
        void                        immediatePresentFrame() override;

        void                        immediateDraw() override;

        VkPipeline                  createGraphicPipeline(const RenderStateVK* renderState, const FramebufferVK* framebuffer, const GeometryVK* geometry, const ShaderProgramVK* program);
        VkPipeline                  getGraphicPipeline(const RenderStateVK* renderState, const FramebufferVK* framebuffer, const ShaderProgramVK* program);
        void                        destroyGraphicPipelines();

        MemoryManagerVK*            m_memoryManager;

        CommandPoolVK*              m_commandPool;

        u32                         m_queueFamilyIndex;

        u32                         m_backbufferIndex;
        SwapChainVK*                m_swapChain;
        std::vector<FenceVK*>       m_fences;
        VkQueue                     m_renderQueue;


        std::map<u64, VkPipeline>   m_pipelineList;

    public:

        void                preRender(bool clear = false)                                           override;
        void                postRender()                                                            override;

        RenderTargetPtr     makeSharedRenderTarget()                                                override;
        GeometryTargetPtr   makeSharedGeometryTarget()                                              override;
        void                resetTextures()                                                         override;

    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace vk
} //namespace renderer
} //namespace v3d

#endif //_VULKAN_RENDER_
