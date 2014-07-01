#include "Material.h"
#include "stream/StreamManager.h"
#include "utils/Logger.h"
#include "stream/FileStream.h"
#include "scene/RenderTechniqueManager.h"
#include "scene/TextureManager.h"

using namespace v3d;
using namespace v3d::renderer;

CMaterial::CMaterial()
    : m_needUpdate(true)
{
	m_type = EObjectType::eTypeMaterial;

	m_materialData._ambient      = core::Vector3D(0.2f, 0.2f, 0.2f);
	m_materialData._diffuse      = core::Vector3D(0.2f, 0.2f, 0.2f);
	m_materialData._specular     = core::Vector3D(1.0f, 1.0f, 1.0f);
	m_materialData._emission     = core::Vector3D(0.0f, 0.0f, 0.0f);
	m_materialData._shininess    = 128U;
	m_materialData._transparency = 1.0f;
}

CMaterial::~CMaterial()
{
}

void CMaterial::setAmbientColor(const core::Vector3D& color)
{
	m_materialData._ambient = color;
    m_needUpdate = true;
}
	
void CMaterial::setDiffuseColor(const core::Vector3D& color)
{
	m_materialData._diffuse = color;
    m_needUpdate = true;
}

void CMaterial::setSpecularColor(const core::Vector3D& color)
{
	m_materialData._specular = color;
    m_needUpdate = true;
}

void CMaterial::setEmissionColor(const core::Vector3D& color)
{
	m_materialData._emission = color;
    m_needUpdate = true;
}

void CMaterial::setShininess(const f32 value)
{
	m_materialData._shininess = value;
    m_needUpdate = true;
}
	
const core::Vector3D& CMaterial::getAmbientColor() const
{
	return m_materialData._ambient;
}

const core::Vector3D& CMaterial::getDiffuseColor() const
{
	return m_materialData._diffuse;
}

const core::Vector3D& CMaterial::getSpecularColor() const
{
	return m_materialData._specular;
}

const core::Vector3D& CMaterial::getEmissionColor() const
{
	return m_materialData._emission;
}

f32 CMaterial::getShininess() const
{
	return m_materialData._shininess;
}
	
const TexturePtr CMaterial::getTexture(const u32 layer) const
{
	if (layer >= ETextureLayer::eLayerMax)
	{
		ASSERT(false && "invalid texture layer") ;
		return nullptr;
	}

	return m_texture.at(layer);
}
	
u32 CMaterial::getTextureCount() const
{
	return m_texture.size();
}
	
bool CMaterial::setTexture(const u32 layer, const std::string& file)
{
    if (layer >= ETextureLayer::eLayerMax)
	{
        ASSERT(false && "Texture Layer range out");
		return false;
	}

    TexturePtr texture = scene::CTextureManager::getInstance()->load(file);
    if (!texture)
    {
        LOG_ERROR("Error read file [%s]", file.c_str());
        return false;
    }

    m_texture[layer] = texture;

    return true;
}

bool CMaterial::setTexture(const u32 layer, const std::string* files[6])
{
    if (layer >= ETextureLayer::eLayerMax)
	{
        ASSERT(false && "Texture Layer range out");
		return false;
	}

    TexturePtr texture = scene::CTextureManager::getInstance()->load(files);
    if (!texture)
    {
        LOG_ERROR("Error read cubemap files");
        return false;
    }

    m_texture[layer] = texture;

    return true;
}

void CMaterial::destroyTexture(u32 layer)
{
    if (layer >= ETextureLayer::eLayerMax)
	{
        ASSERT(false && "Texture Layer range out");
		return;
	}

	m_texture[layer] = nullptr;

}

void CMaterial::setTexture( const u32 layer, TexturePtr texture )
{
    if (layer >= ETextureLayer::eLayerMax)
	{
        ASSERT(false && "Texture Layer range out");
		return;
	}

	m_texture[layer] = texture;

}

void CMaterial::setTransparency(const f32 value)
{
	m_materialData._transparency = (value > 1.0f) ? 1.0f : value;
}

float CMaterial::getTransparency() const
{
	return m_materialData._transparency;
}

const RenderTechniquePtr& CMaterial::getRenderTechique() const
{
    return m_renderTechnique;
}

bool CMaterial::setRenderTechnique(const std::string& file)
{
    RenderTechniquePtr technique = scene::CRenderTechniqueManager::getInstance()->load(file);
    if (!technique)
    {
        LOG_ERROR("Error read file [%s]", file.c_str());
        return false;
    }

    m_renderTechnique = technique;

    return true;
}

bool CMaterial::setRenderTechnique(stream::IStream* stream)
{
    RenderTechniquePtr technique = std::make_shared<CRenderTechnique>();
    technique->init(stream);
    if (technique->load())
    {
        LOG_ERROR("Streaming error read file [%s]", stream->getName().c_str());
        return false;
    }

    scene::CRenderTechniqueManager::getInstance()->add(technique);
    m_renderTechnique = technique;

    return true;
}

void CMaterial::setRenderTechnique(const RenderTechniquePtr& technique)
{
    scene::CRenderTechniqueManager::getInstance()->add(technique);
    m_renderTechnique = technique;
}