#ifndef _V3D_MESH_H_
#define _V3D_MESH_H_

#include "stream/Resource.h"
#include "Material.h"
#include "Geometry.h"
#include "RenderTechnique.h"

namespace v3d
{
namespace scene
{
    class CModel;
}
namespace renderer
{
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////

    class CMesh : public stream::CResource
    {
    public:

        CMesh(const RenderTechniquePtr& technique);
        virtual             ~CMesh();

        void                init(const stream::IStreamPtr& stream)  override;
        bool                load()                                  override;

        void                setMaterial(const MaterialPtr& material);
        const MaterialPtr&  getMaterial() const;

        const GeometryPtr&  getGeomerty() const;

        void                update(f64 time);
        void                render();

    private:

        friend              scene::CModel;

        MaterialPtr         m_material;
        GeometryPtr         m_geometry;
    };

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<CMesh> MeshPtr;

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_MESH_H_
