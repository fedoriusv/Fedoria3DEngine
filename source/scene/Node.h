#ifndef _F3D_NODE_H_
#define _F3D_NODE_H_

#include "Object.h"

namespace f3d
{
namespace scene
{
	/////////////////////////////////////////////////////////////////////////////////////////////////////

	class CNode : public CObject
	{
	public:
							CNode();
		virtual				~CNode();
	
		void				setPosition			(const core::Vector3D& position);
		void				setRotation			(const core::Vector3D& rotation);
		void				setScale			(const core::Vector3D& scale);
		void				setTransform		(const core::Matrix4D& transform);
	
		void				setParentNode		(CNode* parent);
		void				attachChildNode		(CNode* child);
		void				dettachChildNode	(CNode* child);
	
		core::Vector3D		getPosition			()                           const;
		core::Vector3D		getRotation			()                           const;
		core::Vector3D		getScale			()                           const;
		core::Matrix4D		getTransform		()                           const;
		core::Matrix4D		getAbsTransform		()                           const;
	
		CNode*				getParentNode		()                           const;
		CNode*				getChildNodeByID	(u32 id)                     const;
		CNode*				getChildNodeByName  (const std::string& name)    const;

	protected:

		CNode*				m_parentNode;
		std::vector<CNode*>	m_childNodes;

		core::Matrix4D		m_transform;
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_F3D_NODE_H_