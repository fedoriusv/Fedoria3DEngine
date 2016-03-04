#include "RenderPass.h"

#include "Engine.h"
#include "utils/Logger.h"
#include "scene/TargetManager.h"
#include "GeometryTarget.h"

#include "tinyxml2.h"

namespace v3d
{
namespace renderer
{

using namespace scene;

CRenderPass::CRenderPass()
    : m_program(nullptr)
    , m_userShaderData(nullptr)
    , m_defaultShaderData(nullptr)
    , m_renderState(nullptr)
    , m_lods(nullptr)
    , m_advanced(nullptr)
    , m_enable(true)
    , m_name("")
{
    CRenderPass::init();
}

CRenderPass::~CRenderPass()
{
    m_targetList.clear();
}

const ShaderProgramPtr& CRenderPass::getShaderProgram() const
{
    return m_program;
}

void CRenderPass::setShaderProgram(const ShaderProgramPtr& program)
{
    m_program = program;
}

const ShaderDataPtr& CRenderPass::getUserShaderData() const
{
    return m_userShaderData;
}

void CRenderPass::setUserShaderData(const ShaderDataPtr& data)
{
    m_userShaderData = data;
}

const ShaderDataPtr& CRenderPass::getDefaultShaderData() const
{
    return m_defaultShaderData;
}

void CRenderPass::setDefaultShaderData(const ShaderDataPtr& data)
{
    m_defaultShaderData = data;
}

const RenderStatePtr& CRenderPass::getRenderState() const
{
    return m_renderState;
}

void CRenderPass::setRenderState(const RenderStatePtr& state)
{
    m_renderState = state;
}

bool CRenderPass::parse(const tinyxml2::XMLElement* root)
{
    if (!root)
    {
        LOG_ERROR("CRenderPass: Cannot read render pass element");
        return false;
    }

    const std::string passName = root->Attribute("name");
    if (passName.empty())
    {
        LOG_ERROR("CRenderPass: Cannot read render pass name");
        return false;
    }

    m_name = passName;

    //uniforms
    const tinyxml2::XMLElement* uniformsElement = root->FirstChildElement("uniforms");
    if (uniformsElement)
    {
        if (!parseUniforms(uniformsElement))
        {
            return false;
        }
    }

    //attributes
    const tinyxml2::XMLElement* attributesElement = root->FirstChildElement("attributes");
    if (attributesElement)
    {
        if (!parseAttributes(attributesElement))
        {
            return false;
        }
    }

    //fragData
    const tinyxml2::XMLElement* fragdataElement = root->FirstChildElement("fragdata");
    if (fragdataElement)
    {
        if (!parseFragdata(fragdataElement))
        {
            return false;
        }
    }

    //samplers
    const tinyxml2::XMLElement* samplersElement = root->FirstChildElement("samplers");
    if (samplersElement)
    {
        if (!parseSamplers(samplersElement))
        {
            return false;
        }
    }

    //rendertarget
    const tinyxml2::XMLElement*  rendertargetElement = root->FirstChildElement("rendertarget");
    if (rendertargetElement)
    {
        if (!parseRenderTarget(rendertargetElement))
        {
            return false;
        }
    }

    //shaders
    const tinyxml2::XMLElement*  shadersElement = root->FirstChildElement("shaders");
    if (shadersElement)
    {
        if (!parseShaders(shadersElement))
        {
            return false;
        }
    }

    //renderstate
    const tinyxml2::XMLElement*  renderstateElement = root->FirstChildElement("renderstate");
    if (renderstateElement)
    {
        if (!parseRenderState(renderstateElement))
        {
            return false;
        }
    }

    //lod
    const tinyxml2::XMLElement*  renderlodElement = root->FirstChildElement("lod");
    if (renderlodElement)
    {
        if (!parseRenderLOD(renderlodElement))
        {
            return false;
        }
    }

    //advanced
    const tinyxml2::XMLElement*  renderadvancedElement = root->FirstChildElement("advanced");
    if (renderadvancedElement)
    {
        if (!parseRenderAdvanced(renderadvancedElement))
        {
            return false;
        }
    }

    return true;
}

bool CRenderPass::parseUniforms(const tinyxml2::XMLElement* root)
{
    if (!root)
    {
        LOG_ERROR("CRenderPass: Not exist xml uniforms element");
        return false;
    }

    const tinyxml2::XMLElement* varElement = root->FirstChildElement("var");
    while (varElement)
    {
        CShaderUniform* uniform = new CShaderUniform();

        if (!uniform->parse(varElement))
        {
            delete uniform;
            uniform = nullptr;

            LOG_ERROR("CRenderPass: Cannot parse uniform in pass '%s'", m_name.c_str());
            varElement = varElement->NextSiblingElement("var");
            continue;
        }

        bool isDefault = (uniform->getData() != CShaderUniform::eUserUniform);
        if (isDefault)
        {
            m_defaultShaderData->addUniform(uniform);
        }
        else
        {
            m_userShaderData->addUniform(uniform);
        }

        varElement = varElement->NextSiblingElement("var");
    }

    return true;
}

bool CRenderPass::parseAttributes(const tinyxml2::XMLElement* root)
{
    if (!root)
    {
        LOG_ERROR("CRenderPass: Not exist xml attributes element");
        return false;
    }

    const tinyxml2::XMLElement* varElement = root->FirstChildElement("var");
    while (varElement)
    {
        CShaderAttribute* attribute = new CShaderAttribute();
        if (!attribute->parse(varElement))
        {
            delete attribute;
            attribute = nullptr;

            LOG_ERROR("CRenderPass: Parse error attribute element");
            varElement = varElement->NextSiblingElement("var");
            continue;
        }

        bool isDefault = (attribute->getData() != CShaderAttribute::eAttribUser);
        if (isDefault)
        {
            m_defaultShaderData->addAttribute(attribute);
        }
        else
        {
            m_userShaderData->addAttribute(attribute);
        }

        varElement = varElement->NextSiblingElement("var");
    }

    return true;
}

bool CRenderPass::parseFragdata(const tinyxml2::XMLElement* root)
{
    if (!root)
    {
        return true;
    }

    const tinyxml2::XMLElement* varElement = root->FirstChildElement("var");
    while (varElement)
    {
        CShaderAttribute* fragData = new CShaderAttribute();
        if (!fragData->parse(varElement))
        {
            delete fragData;
            fragData = nullptr;

            LOG_ERROR("CRenderPass: Parse error fragData element");
            varElement = varElement->NextSiblingElement("var");
            continue;
        }

        bool isDefault = (fragData->getData() != CShaderAttribute::eAttribUser);
        if (isDefault)
        {
            m_defaultShaderData->addFragData(fragData);
        }
        else
        {
            m_userShaderData->addFragData(fragData);
        }

        varElement = varElement->NextSiblingElement("var");
    }

    return true;
}

bool CRenderPass::parseSamplers(const tinyxml2::XMLElement* root)
{
    if (!root)
    {
        LOG_ERROR("CRenderPass: Not exist xml samplers element");
        return false;
    }

    const tinyxml2::XMLElement* varElement = root->FirstChildElement("var");
    while (varElement)
    {
        SamplerPtr sampler = std::make_shared<CShaderSampler>();
        if (!sampler->parse(varElement))
        {
            LOG_ERROR("CRenderPass: Parse error samplers element");
            varElement = varElement->NextSiblingElement("var");
            continue;
        }

        bool isDefault = sampler->getType() != CShaderSampler::eUserSampler;
        if (isDefault)
        {
            m_defaultShaderData->addSampler(sampler);
        }
        else
        {
            m_userShaderData->addSampler(sampler);
        }

        varElement = varElement->NextSiblingElement("var");
    }

    return true;
}

bool CRenderPass::parseShaders(const tinyxml2::XMLElement* root)
{
    if (!root)
    {
        LOG_ERROR("CRenderPass: Not exist xml shader element");
        return false;
    }

    const tinyxml2::XMLElement*  shaderElement = root->FirstChildElement("var");
    while (shaderElement)
    {
        ShaderPtr shader = RENDERER->makeSharedShader();
        if (!shader->parse(shaderElement))
        {
            LOG_ERROR("CRenderPass: Shader parse error");
            ASSERT(false, "Shader parse error");

            shaderElement = shaderElement->NextSiblingElement("var");
            continue;
        }

        if (!shader->create())
        {
            LOG_ERROR("CRenderPass: Error create shader");
            shaderElement = shaderElement->NextSiblingElement("var");
            continue;
        }
        m_program->attachShader(shader);

        shaderElement = shaderElement->NextSiblingElement("var");
    }

    m_program->addShaderData(m_defaultShaderData);
    m_program->addShaderData(m_userShaderData);

    if (!m_program->create())
    {
        LOG_ERROR("CRenderPass: Error Create Shader Program");
        return false;
    }

    return true;
}

void CRenderPass::init()
{
    m_userShaderData = std::make_shared<CShaderData>();
    m_defaultShaderData = std::make_shared<CShaderData>();
    m_lods = std::make_shared<CRenderLOD>();
    m_advanced = std::make_shared<CRenderAdvanced>();
    m_program = RENDERER->makeSharedProgram();
    m_renderState = RENDERER->makeSharedRenderState();
}

bool CRenderPass::parseRenderTarget(const tinyxml2::XMLElement* root)
{
    if (!root)
    {
        LOG_ERROR("CRenderPass: Not exist xml element");
        return false;
    }

    const tinyxml2::XMLElement* varElement = root->FirstChildElement("var");
    while (varElement)
    {
        bool isDefault = false;
        std::string name;
        if (varElement->Attribute("val"))
        {
            name = varElement->Attribute("val");
            if (name == "default")
            {
                isDefault = true;
            }
        }
        else
        {
            LOG_ERROR("CRenderPass: Render Target have not name");
            varElement = varElement->NextSiblingElement("var");
            continue;
        }

        if (isDefault)
        {
            LOG_INFO("CRenderTarget: Set default setting for render target");
            CRenderPass::addRenderTarget(RENDERER->getDefaultRenderTarget());
        }
        else
        {
            const TargetPtr target = CTargetManager::getInstance()->get(name);
            if (!target)
            {
                LOG_WARNING("CRenderTarget: Render target not found with name '%s', set default", name.c_str());
                CRenderPass::addRenderTarget(RENDERER->getDefaultRenderTarget());
            }
            else
            {
                LOG_INFO("CRenderTarget: Set target with name '%s'", name.c_str());
                CRenderPass::addRenderTarget(target);

                if (target->getTagetType() == ITarget::ETagetType::eGeometryTarget)
                {
                    const GeometryTargetPtr& geomtarget = std::static_pointer_cast<CGeometryTarget>(target);

                    if (geomtarget->getBufferStrings().empty())
                    {
                        LOG_WARNING("CRenderTarget: Varyings list is empty in target '%s'", name.c_str());
                    }
                    else
                    {
                        m_program->addVaryingsAttibutes(geomtarget->getBufferStrings());
                    }
                }
            }
        }

        varElement = varElement->NextSiblingElement("var");
    }

    if (CRenderPass::getRenderTargetCount() == 0)
    {
        LOG_WARNING("CRenderTarget: Render target not found set default");
        CRenderPass::addRenderTarget(RENDERER->getDefaultRenderTarget());
    }

    return true;
}

bool CRenderPass::parseRenderState(const tinyxml2::XMLElement* root)
{
    if (!root)
    {
        LOG_ERROR("CRenderPass: Not exist xml renderstate element");
        return false;
    }

    if (!m_renderState->parse(root))
    {
        LOG_ERROR("CRenderPass: Renderstate parse error");
        return false;
    }

    return true;
}

bool CRenderPass::parseRenderLOD(const tinyxml2::XMLElement* root)
{
    if (!root)
    {
        LOG_ERROR("CRenderPass: Not exist xml lod element");
        return false;
    }

    f32 geometryLod = root->FloatAttribute("geometry");
    if (geometryLod > 0)
    {
        m_lods->setGeometryDistance(geometryLod);
    }

    return true;
}

bool CRenderPass::parseRenderAdvanced(const tinyxml2::XMLElement* root)
{
    if (!root)
    {
        LOG_ERROR("CRenderPass: Not exist xml advanced element");
        return false;
    }

    if (!m_advanced->parse(root))
    {
        LOG_ERROR("CRenderPass: RenderAdvanced parse error");
        return false;
    }

    return true;
}

void CRenderPass::bind(u32 target)
{
    m_renderState->bind();

    if (!m_enable || !m_program->isEnable())
    {
        m_program->unbind();
        return;
    }

    m_program->bind();

    const UniformList& list = m_userShaderData->m_uniformList;
    for (UniformList::const_iterator uniform = list.begin(); uniform != list.end(); ++uniform)
    {
        CShaderUniform::EUniformData data = uniform->second->getData();
        if (data != CShaderUniform::eUserUniform)
        {
            continue;
        }

        m_program->applyUniform(uniform->second);
    }

    /*ASSERT(m_targetList.size() > target, "Invalid target index");
    m_targetList[target]->bind();*/
}

void CRenderPass::unbind(u32 target)
{
    //m_program->unbind();

    /*ASSERT(m_targetList.size() > target, "Invalid target index");
    m_targetList[target]->unbind();*/

}

const std::string CRenderPass::attachIndexToUniform(const std::string& name, s32 idx)
{
    size_t pos = name.find_first_of(".");
    if (pos == std::string::npos)
    {
        std::string idxName(name + "[" + std::to_string(idx) + "]");

        return idxName;
    }

    std::string idxName(name.substr(0, pos) + "[" + std::to_string(idx) + "]" + name.substr(pos, name.size()));

    return idxName;
}

const RenderLODPtr& CRenderPass::getRenderLOD() const
{
    return m_lods;
}

void CRenderPass::setRenderLOD(const RenderLODPtr& lod)
{
    m_lods = lod;
}

const RenderAdvancedPtr& CRenderPass::getRenderAdvanced() const
{
    return m_advanced;
}
void CRenderPass::setRenderAdvanced(const RenderAdvancedPtr& advanced)
{
    m_advanced = advanced;
}

const TargetPtr& CRenderPass::getRenderTarget(u32 index) const
{
    ASSERT(m_targetList.size() > index, "Invalid Render Target index");
    return m_targetList[index];
}

void CRenderPass::addRenderTarget(const TargetPtr& target)
{
    if (target)
    {
        TargetList ::const_iterator iter = std::find(m_targetList.begin(), m_targetList.end(), target);
        if (iter != m_targetList.end())
        {
            LOG_WARNING("CRenderPass: Added target alredy exist '%s'", (*iter)->getName().c_str());
            return;
        }

        m_targetList.push_back(target);
    }
}

void CRenderPass::setRenderTarget(u32 index, const TargetPtr& target)
{
    ASSERT(m_targetList.size() > index, "Invalid Render Target index");
    m_targetList[index] = target;
}

u32 CRenderPass::getRenderTargetCount() const
{
    return (u32)m_targetList.size();
}

} //namespace renderer
} //namespace v3d
