#ifndef _F3D_RENDERJOB_H_
#define _F3D_RENDERJOB_H_

#include "Material.h"
#include "Geometry.h"

namespace v3d
{
namespace renderer
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    class CRenderJob
    {
    public:

        CRenderJob(const MaterialPtr& material, const GeometryPtr& geomerty);
        ~CRenderJob();

        void                setMaterial(const MaterialPtr& material);
        const MaterialPtr&  getMaterial() const;

        void                setGeomery(const GeometryPtr& geomerty);
        const GeometryPtr&  getGeometry() const;

        void                job();

    private:

        void                updateMaterial(const ShaderDataPtr& data);
        void                updateTransform(const ShaderDataPtr& data);

        MaterialPtr         m_material;
        GeometryPtr         m_geomerty;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<CRenderJob> RenderJobPtr;

    /////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_F3D_RENDERJOB_H_