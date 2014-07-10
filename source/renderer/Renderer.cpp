#include "renderer/Renderer.h"
#include "context/DriverContext.h"
#include "scene/Light.h"

using namespace v3d;
using namespace v3d::renderer;

CRenderer::CRenderer(const DriverContextPtr& context)
    : m_context(context)
    , m_backColor(core::Vector3D(0))
    , m_projectionMatrix(core::Matrix4D())
    , m_viewMatrix(core::Matrix4D())
    , m_viewPosition(core::Vector3D(0.0f))
    , m_updateCamera(true)
#ifdef _DEBUG
    , m_debugMode(false)
#endif
{
    m_viewportSize = context->getWindow()->getSize();
}

CRenderer::~CRenderer()
{
    m_lightList.clear();
}

void CRenderer::addLight(scene::CLight* lights)
{
    m_lightList.push_back(lights);
}

void CRenderer::setBackColor(const core::Vector3D& color)
{
	m_backColor = color;
}

const core::Vector3D& CRenderer::getBackColor() const
{
	return m_backColor;
}

void CRenderer::checkForErrors(const std::string& location)
{
    m_context->checkForErrors(location);
}

void CRenderer::reshape(u32 width, u32 height)
{
    if (height == 0)
    {
        height = 1;
    }
    m_viewportSize.width = width;
    m_viewportSize.height = height;

    f32 aspectRatio = (f32)m_viewportSize.width / (f32)m_viewportSize.height;
    m_projectionMatrix = core::buildProjectionMatrixPerspective(45.0f, aspectRatio, 0.5f, 10.0f);

}

void CRenderer::updateCamera(const core::Vector3D& pos, const core::Vector3D& target, const core::Vector3D& up)
{
    m_viewMatrix = core::buildLookAtMatrix(pos, target, up);
    m_viewMatrix.makeTransposed();
    m_viewPosition = pos;

}

const core::Dimension2D& CRenderer::getViewportSize() const
{
    return m_viewportSize;
}

void CRenderer::draw(const RenderJobPtr& job)
{
    const MaterialPtr& material = job->getMaterial();
    const GeometryPtr& geomerty = job->getGeometry();
    const u32 passCount = material->getRenderTechique()->getRenderPassCount();
    const core::Matrix4D& transform = job->getTransform();

    for (u32 i = 0; i < passCount; ++i)
    {
        const RenderPassPtr& pass = material->getRenderTechique()->getRenderPass(i);

        if (!checkLOD(transform, pass))
        {
            continue;
        }

        pass->bind();

        updateTransform(transform, pass);
        updateMaterial(material, pass);
        updateLight(transform, pass);

        for (u32 layer = 0; layer < material->getTextureCount(); ++layer)
        {
            const TexturePtr& texture = material->getTexture(layer);
            if (pass->getShaderData()->getSamplerList().size() >= layer)
            {
                if (texture->isEnable())
                {
                    texture->bind(layer);
                }
                else
                {
                    texture->unbind(layer);
                }
            }
        }

        geomerty->draw();
    }
}

#ifdef _DEBUG
void CRenderer::setDebugMode(bool active)
{
    m_debugMode = active;
}

bool CRenderer::isDebugMode() const
{
    return m_debugMode;
}
#endif

void CRenderer::updateTransform(const core::Matrix4D& transform, const RenderPassPtr& pass)
{
    const ShaderDataPtr& data = pass->getShaderData();
    const ShaderProgramPtr& program = pass->getShaderProgram();
    const u32 shader = program->getShaderID();

    const UniformList& list = data->getDefaultUniformList();
    for (auto uniform : list)
    {
        const EUniformData type = uniform.second->getUniformData();
        switch (type)
        {
            case EUniformData::eTransformProjectionMatrix:
            {
                program->setUniformMatrix4(shader, "transform.projectionMatrix", m_projectionMatrix);
            }
            break;

            case EUniformData::eTransformModelMatrix:
            {
                core::Matrix4D modelMatrix(transform);
                modelMatrix.makeTransposed();

                program->setUniformMatrix4(shader, "transform.modelMatrix", modelMatrix);
            }
            break;

            case EUniformData::eTransformViewMatrix:
            {
                program->setUniformMatrix4(shader, "transform.viewMatrix", m_viewMatrix);
            }
            break;

            case EUniformData::eTransformViewPosition:
            {
                program->setUniformVector3(shader, "transform.viewPosition", m_viewPosition);
            }
            break;

            case EUniformData::eTransformNormalMatrix:
            {
                core::Matrix4D normalMatrix;
                transform.getInverse(normalMatrix);

                program->setUniformMatrix4(shader, "transform.normalMatrix", normalMatrix);
            }
            break;
        }
    }
}

bool CRenderer::checkLOD(const core::Matrix4D& transform, const RenderPassPtr& pass)
{
    const RenderLODPtr& lod = pass->getRenderLOD();

    if (lod->getGeometryDistance() <= 0)
    {
        return true;
    }

    if (lod->getGeometryDistance() < (transform.getTranslation() - m_viewPosition).length())
    {
        return false;
    }

    return true;
}

void CRenderer::updateMaterial(const MaterialPtr& material, const RenderPassPtr& pass)
{
    const ShaderDataPtr& data = pass->getShaderData();
    const ShaderProgramPtr& program = pass->getShaderProgram();
    const u32 shader = program->getShaderID();

    const UniformList& list = data->getDefaultUniformList();
    for (auto uniform : list)
    {
        const EUniformData type = uniform.second->getUniformData();
        switch (type)
        {
        case EUniformData::eMaterialAmbient:
    
            program->setUniformVector4(shader, "material.ambient", material->getAmbientColor());
            break;

        case EUniformData::eMaterialDiffuse:

            program->setUniformVector4(shader, "material.specular", material->getSpecularColor());
            break;

        case EUniformData::eMaterialSpecular:

            program->setUniformVector4(shader, "material.diffuse", material->getDiffuseColor());
            break;

        case EUniformData::eMaterialEmission:

            program->setUniformVector4(shader, "material.emission", material->getEmissionColor());
            break;

        case EUniformData::eMaterialShininess:

            program->setUniformFloat(shader, "material.shininess", material->getShininess());
            break;

        case EUniformData::eMaterialTransparency:

            program->setUniformFloat(shader, "material.transparency", material->getTransparency());
            break;
        }
    }
}

void CRenderer::updateLight(const core::Matrix4D& transform, const RenderPassPtr& pass)
{
    if (m_lightList.empty())
    {
        return;
    }

    std::vector<scene::CLight*> lights = m_lightList;
    const Vector3D& pos = transform.getTranslation();

    std::remove_if(lights.begin(), lights.end(), [pos](scene::CLight* light) -> bool
    {
        if (!light->isVisible())
        {
            return true;
        }

        f32 distance = (light->getPosition() - pos).length();
        if (light->getRadius() < distance)
        {
            return true;
        }

        return true;
    });

    const ShaderDataPtr& data = pass->getShaderData();
    const ShaderProgramPtr& program = pass->getShaderProgram();
    const u32 shader = program->getShaderID();

    if (data->isExistUniform(eLightsCount))
    {
        program->setUniformInt(shader, "lights.count", lights.size());
    }

    if (lights.empty())
    {
        return;
    }

    s32 index = 0;
    for (std::vector<scene::CLight*>::iterator light = lights.begin(); light < lights.end(); ++light)
    {
        const UniformList& list = data->getDefaultUniformList();
        for (auto uniform : list)
        {
            const EUniformData type = uniform.second->getUniformData();
            switch (type)
            {
            case EUniformData::eLightPosition:

                program->setUniformVector4(shader, pass->attachIndexToUniform("light.position", index), Vector4D((*light)->getPosition(), 0.0f));
                break;

            case EUniformData::eLightAmbient:

                program->setUniformVector4(shader, pass->attachIndexToUniform("light.ambient", index), (*light)->getAmbient());
                break;

            case EUniformData::eLightDiffuse:

                program->setUniformVector4(shader, pass->attachIndexToUniform("light.diffuse", index), (*light)->getDiffuse());
                break;

            case EUniformData::eLightSpecular:

                program->setUniformVector4(shader, pass->attachIndexToUniform("light.specular", index), (*light)->getSpecular());
                break;

            case EUniformData::eLightDirection:

                program->setUniformVector3(shader, pass->attachIndexToUniform("light.direction", index), (*light)->getDirection());
                break;

            case EUniformData::eLightAttenuation:

                program->setUniformVector3(shader, pass->attachIndexToUniform("light.attenuation", index), (*light)->getAttenuation());
                break;


            case EUniformData::eLightRadius:

                program->setUniformFloat(shader, pass->attachIndexToUniform("light.radius", index), (*light)->getRadius());
                break;
            }
        }

        ++index;
    }
}
