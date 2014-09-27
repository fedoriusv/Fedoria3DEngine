#ifndef _V3D_CAMERA_H_
#define _V3D_CAMERA_H_

#include "Node.h"

namespace v3d
{
namespace scene
{
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////

    class CSceneManager;

    class CCamera : public CNode
    {
    public:

        CCamera();
        virtual             ~CCamera();

        void                setTarget(const core::Vector3D& target);
        void                setUpVector(const core::Vector3D& up);

        core::Vector3D      getTarget() const;
        core::Vector3D      getUpVector() const;

        void                render()        override;
        void                update(f64 time)override;
        void                init()          override;

        bool                isActive() const;

    protected:

        friend              CSceneManager;

        core::Vector3D      m_up;
        core::Vector3D      m_target;

        bool                m_active;

        core::Vector3D      m_max;
        core::Vector3D      m_min;
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_CAMERA_H_