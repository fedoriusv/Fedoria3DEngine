#ifndef _V3D_SHADER_SAMPLER_H_
#define _V3D_SHADER_SAMPLER_H_

#include "common.h"
#include "RenderTarget.h"
#include "Texture.h"

namespace tinyxml2
{
    class XMLElement;
}

namespace v3d
{
namespace renderer
{
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    class CShaderSampler
    {
    public:

        enum ESamplerType
        {
            eInvalidSampler = -1,
            eUserSampler,
            eTextureSampler,
            eRenderTargetSampler
        };

        CShaderSampler();
        ~CShaderSampler();

        CShaderSampler&         operator=(const CShaderSampler& other);

        void                    setAttribute(const std::string& attribute);
        const std::string&      getAttribute() const;

        ESamplerType            getType() const;

        bool                    parse(const tinyxml2::XMLElement* root);

    protected:

        ESamplerType            m_type;
        std::string             m_attribute;

        RenderTargetWPtr        m_target;
        TextureWPtr             m_texture;
    };

    //////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<CShaderSampler>    SamplerPtr;
    typedef std::vector<SamplerPtr>            SamplerList;

    //////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_SHADER_SAMPLER_H_
