#ifndef _V3D_RENDERER_D3D_H_
#define _V3D_RENDERER_D3D_H_

#include "renderer/Renderer.h"

namespace v3d
{
namespace renderer
{
namespace d3d
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////
    
    class CRendererD3D : public CRenderer
    {
    public:
    
        CRendererD3D(const DriverContextPtr& context);
        ~CRendererD3D();
    
        void                init()                                                                  override;
    
        void                preRender(bool clear = false)                                           override;
        void                postRender()                                                            override;

        ShaderPtr           makeSharedShader()                                                      override;
        ShaderProgramPtr    makeSharedProgram()                                                     override;
        GeometryPtr         makeSharedGeometry(const CRenderTechnique* technique)                   override;
        RenderStatePtr      makeSharedRenderState()                                                 override;

        RenderTargetPtr     makeSharedRenderTarget()                                                override;
        GeometryTargetPtr   makeSharedGeometryTarget()                                              override;

        bool                isLocked() const;

    private:

        void                resetTextures()                                                         override;

        bool                m_isLocked;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace d3d
} //namespace renderer
} //namespace v3d

#endif //_V3D_RENDERER_D3D_H_
