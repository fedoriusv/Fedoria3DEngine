#include "Renderer.h"
#include "Material.h"
#include "context/DriverContext.h"
#include "scene/Light.h"
#include "scene/Camera.h"

using namespace v3d;
using namespace core;
using namespace renderer;
using namespace scene;

CRenderer::CRenderer(const DriverContextPtr& context)
    : m_context(context)
    , m_frameIndex(0U)

    , m_defaultRenderTarget(nullptr)
    , m_currentRenderTarget(nullptr)
#ifdef _DEBUG
    , m_debugMode(false)
#endif
    , m_camera(nullptr)
{
}

CRenderer::~CRenderer()
{
    m_context->destroyContext();
    m_lightList.clear();
}

const DriverContextPtr& CRenderer::getContext() const
{
    return m_context;
}

void CRenderer::addLight(scene::CLight* lights)
{
    m_lightList.push_back(lights);
}

void CRenderer::checkForErrors(const std::string& location)
{
    m_context->checkForErrors(location);
}

void CRenderer::updateCamera(CCamera* camera)
{
    m_camera = camera;
}

const core::Rect32& CRenderer::getViewportSize() const
{
    ASSERT(m_currentRenderTarget && "CRenderer: Current Render Target in null");
    if (m_currentRenderTarget)
    {
        return m_currentRenderTarget->getViewport();
    }
    else
    {
        return m_defaultRenderTarget->getViewport();
    }
}

Vector2D CRenderer::convertPosScreenToCanvas(const Point2D& pos)
{
    const core::Dimension2D& size = m_context->getWindowSize();
    
    Vector2D newpos(0.0f, 0.0f);
    f32 koefW = pos.x / (f32)size.width;
    f32 koefH = (size.height - pos.y) / (f32)size.height;
    newpos.x = -1.0f + 2.0f * koefW;
    newpos.y = -1.0f + 2.0f * koefH;

    return newpos;
}

Point2D CRenderer::convertPosCanvasToScreen(const Vector2D& pos)
{
    const core::Dimension2D& size = m_context->getWindowSize();

    Point2D newpos(0, 0);
    f32 koefW = (pos.x + 1.0f) / 2.0f;
    f32 koefH = (pos.y + 1.0f) / 2.0f;
    newpos.x = (u32)(koefW * size.width);
    newpos.y = size.height - (u32)(koefH * size.height);

    return newpos;
}

u32 CRenderer::getFrameIndex() const
{
    return m_frameIndex;
}

const RenderTargetPtr& CRenderer::getDefaultRenderTarget() const
{
    return m_defaultRenderTarget;
}

const RenderTargetPtr& CRenderer::getCurrentRenderTarget() const
{
    if (m_currentRenderTarget)
    {
        return m_currentRenderTarget;
    }
    else
    {
        return m_defaultRenderTarget;
    }
}

void CRenderer::setCurrentRenderTarget(const RenderTargetPtr& target)
{
    m_currentRenderTarget = target;
}

void CRenderer::draw(const RenderJobPtr& job)
{
    CMaterial* material = job->getMaterial();
    const GeometryPtr& geometry = job->getGeometry();
    const core::Matrix4D& transform = job->getTransform();
    u32 targetIndex = job->getRenderTarget();
    CRenderTechnique* techique = material->getRenderTechique();
    const u32 passCount = techique->getRenderPassCount();

    for (u32 passIdx = 0; passIdx < passCount; ++passIdx)
    {
        const RenderPassPtr& pass = techique->getRenderPass(passIdx);
        pass->bind(targetIndex);

        if (pass->isCurrent())
        {
            techique->setCurrentPass(passIdx);
        }

        CRenderer::updateTransform(transform, pass);
        CRenderer::updateMaterial(material, pass);
        CRenderer::updateTexture(material, pass);
        CRenderer::updateLight(transform, pass);

        //Draw Geometry
        if (geometry->updated())
        {
            geometry->refresh();
        }
        geometry->draw();

        pass->unbind(targetIndex);
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
    const ShaderDataPtr& data = pass->getDefaultShaderData();
    const ShaderProgramPtr& program = pass->getShaderProgram();

    const UniformList& list = data->getUniformList();
    for (auto& uniform : list)
    {
        const CShaderUniform::EUniformData type = uniform.second->getData();
        s32 id = uniform.second->getID();
        switch (type)
        {
            case CShaderUniform::eTransformProjectionMatrix:

                if (!m_camera)
                {
                    ASSERT("Camera not exist" && false);
                    break;
                }
                program->setUniformMatrix4(id, m_camera->getProjectionMatrix());

                break;

            case CShaderUniform::eTransformModelMatrix:
            {
                core::Matrix4D modelMatrix(transform);
                modelMatrix.makeTransposed();

                program->setUniformMatrix4(id, modelMatrix);
            }
                break;

            case CShaderUniform::eTransformViewMatrix:

                if (!m_camera)
                {
                    ASSERT("Camera not exist" && false);
                    break;
                }
                program->setUniformMatrix4(id, m_camera->getViewMatrix());

                break;

            case CShaderUniform::eTransformViewPosition:

                if (!m_camera)
                {
                    ASSERT("Camera not exist" && false);
                    break;
                }
                program->setUniformVector3(id, m_camera->getPosition());
                break;

            case CShaderUniform::eTransformViewUpVector:

                if (!m_camera)
                {
                    ASSERT("Camera not exist" && false);
                    break;
                }
                program->setUniformVector3(id, m_camera->getUpVector());
                break;

            case CShaderUniform::eTransformNormalMatrix:
            {
                core::Matrix4D normalMatrix;
                transform.getInverse(normalMatrix);

                program->setUniformMatrix4(id, normalMatrix);
            }
                break;

            default:
                break;
        }
    }
}

void CRenderer::updateMaterial(const CMaterial* material, const RenderPassPtr& pass)
{
    const ShaderDataPtr& data = pass->getDefaultShaderData();
    const ShaderProgramPtr& program = pass->getShaderProgram();

    const UniformList& list = data->getUniformList();
    for (auto& uniform : list)
    {
        const CShaderUniform::EUniformData type = uniform.second->getData();
        const s32 id = uniform.second->getID();
        switch (type)
        {
        case CShaderUniform::eMaterialAmbient:
    
            program->setUniformVector4(id, material->getAmbientColor());
            break;

        case CShaderUniform::eMaterialDiffuse:

            program->setUniformVector4(id, material->getDiffuseColor());
            break;

        case CShaderUniform::eMaterialSpecular:

            program->setUniformVector4(id, material->getSpecularColor());
            break;

        case CShaderUniform::eMaterialEmission:

            program->setUniformVector4(id, material->getEmissionColor());
            break;

        case CShaderUniform::eMaterialShininess:

            program->setUniformFloat(id, material->getShininess());
            break;

        case CShaderUniform::eMaterialTransparency:

            program->setUniformFloat(id, material->getTransparency());
            break;

        default:
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

    std::remove_if(lights.begin(), lights.end(), [&pos](scene::CLight* light) -> bool
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

    if (lights.empty())
    {
        return;
    }

    const ShaderDataPtr& data = pass->getDefaultShaderData();
    const ShaderProgramPtr& program = pass->getShaderProgram();

    for (std::vector<scene::CLight*>::iterator light = lights.begin(); light < lights.end(); ++light)
    {
        const UniformList& list = data->getUniformList();
        for (auto& uniform : list)
        {
            const CShaderUniform::EUniformData type = uniform.second->getData();
            const s32 id = uniform.second->getID();
            switch (type)
            {
            case CShaderUniform::eLightsCount:

                program->setUniformInt(id, (s32)lights.size());
                break;

            case CShaderUniform::eLightPosition:

                program->setUniformVector4(id, Vector4D((*light)->getPosition(), 0.0f));
                break;

            case CShaderUniform::eLightAmbient:

                program->setUniformVector4(id, (*light)->getAmbient());
                break;

            case CShaderUniform::eLightDiffuse:

                program->setUniformVector4(id, (*light)->getDiffuse());
                break;

            case CShaderUniform::eLightSpecular:

                program->setUniformVector4(id, (*light)->getSpecular());
                break;

            case CShaderUniform::eLightDirection:

                program->setUniformVector3(id, (*light)->getDirection());
                break;

            case CShaderUniform::eLightAttenuation:

                program->setUniformVector3(id, (*light)->getAttenuation());
                break;

            case CShaderUniform::eLightRadius:

                program->setUniformFloat(id, (*light)->getRadius());
                break;

            default:
                break;
            }
        }
    }
}

void CRenderer::updateTexture(CMaterial* material, const RenderPassPtr& pass)
{
    const ShaderDataPtr& defaultData = pass->getDefaultShaderData();
    const SamplerList& samplerList = defaultData->getSamplerList();

    bool isUserSamplers = (material->getTextureCount() > 0) ? true : false;
    u32 layersCount = 0;
    for (auto& sampler : samplerList)
    {
        if (sampler->getID() < 0)
        {
            continue;
        }

        CShaderSampler::ESamplerType type = sampler->getType();
        switch (type)
        {
            case CShaderSampler::eUserSampler:
            {
                isUserSamplers = true;
                break;
            }
            break;

            case CShaderSampler::eTextureSampler:
            case CShaderSampler::eRenderTargetSampler:
            {
                CTexture* texture = sampler->getTexture();
                if (texture)
                {
                    texture->bind(layersCount, sampler->getID());
                    ++layersCount;
                }
            }
            break;
        }
    }

    if (isUserSamplers)
    {
        layersCount = material->getTextureCount();
        u32 samplerID = 0;
        for (u32 layer = 0; layer < layersCount; ++layer)
        {
            CTexture* texture = material->getTexture(layer);
            if (texture->isEnable())
            {
                texture->bind(layer, samplerID);
            }
            else
            {
                texture->unbind(layer, samplerID);
            }
            samplerID++;
        }

        if (layersCount == 0)
        {
            resetTexture();
        }
    }
}
