#ifndef _F3D_RENDERER_H_
#define _F3D_RENDERER_H_

#include "Singleton.h"
#include "context/DriverContext.h"
#include "renderer/ShaderUniform.h"
#include "renderer/Shader.h"

namespace f3d
{
namespace renderer
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    class CRenderPass;

    class CRenderer : public Singleton<CRenderer>
    {
    public:

        CRenderer(const DriverContextPtr& context);
        virtual                 ~CRenderer();

        virtual void            init()                         = 0;
        
        virtual void            preRender()                    = 0;
        virtual void            postRender()                   = 0;

        virtual void            reshape(u32 width, u32 height) = 0;

        void                    setBackColor(const core::Vector3D& color);
        const core::Vector3D&   getBackColor() const;

        void                    checkForErrors(const std::string& location = "");

        //virtual GeometryPtr   createGeometry() = 0;

    protected:

        friend                  CRenderPass;

        virtual ShaderPtr       makeSharedShader() const    = 0;;

        DriverContextPtr        m_context;

        core::Vector3D          m_backColor;
        core::Dimension2D       m_viewportSize;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<CRenderer>	RendererPtr;

    /////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_F3D_RENDERER_H_