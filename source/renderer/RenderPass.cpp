#include "RenderPass.h"
#include "Fedoria3D.h"
#include "renderer/GL/ShaderProgramGL.h"

using namespace f3d;
using namespace f3d::renderer;

CRenderPass::CRenderPass()
    : m_program(nullptr)
{
    CRenderPass::init();
}

CRenderPass::~CRenderPass()
{
}

bool CRenderPass::parse(tinyxml2::XMLElement* root)
{
    if (!root)
    {
        LOG_ERROR("Error parse.  Cannot read render pass element");
        return false;
    }

    const std::string passName = root->Attribute("name");
    if (passName.empty())
    {
        LOG_ERROR("Error parse. Cannot read render pass name");
        return false;
    }

    m_name = passName;

    //uniforms
    tinyxml2::XMLElement*  uniformsElement = root->FirstChildElement("uniforms");
    if (uniformsElement)
    {
        if (!parseUniforms(uniformsElement))
        {
            return false;
        }
    }

    //attributes
    tinyxml2::XMLElement*  attributesElement = root->FirstChildElement("attributes");
    if (attributesElement)
    {
        if (!parseAttributes(attributesElement))
        {
            return false;
        }
    }

    //samplers
    tinyxml2::XMLElement*  samplersElement = root->FirstChildElement("samplers");
    if (samplersElement)
    {
        if (!parseSamplers(samplersElement))
        {
            return false;
        }
    }

    //rendertarget
    tinyxml2::XMLElement*  rendertargetElement = root->FirstChildElement("rendertarget");
    if (rendertargetElement)
    {
        if (!parseRenderTarget(rendertargetElement))
        {
            return false;
        }
    }


    //shaders
    tinyxml2::XMLElement*  shadersElement = root->FirstChildElement("shaders");
    if (shadersElement)
    {
        if (!parseShaders(shadersElement))
        {
            return false;
        }
    }

    return true;
}

bool CRenderPass::parseUniforms(tinyxml2::XMLElement* root)
{
    if (!root)
    {
        LOG_ERROR("Error parse. Not exist xml uniforms element");
        return false;
    }

    tinyxml2::XMLElement* varElement = root;
    while (varElement)
    {
        const std::string varName = varElement->Attribute("name");
        if (varName.empty())
        {
            LOG_ERROR("Cannot find uniform name from pass '%s'", m_name);
            return false;
        }

        const std::string varType = varElement->Attribute("type");
        if (varType.empty())
        {
            LOG_ERROR("Cannot find uniform type from pass '%s' in '%s'", m_name, varName);
            return false;
        }

        const std::string varVal = varElement->Attribute("val");
        if (varVal.empty())
        {
            LOG_ERROR("Cannot find uniform val from pass '%s' in '%s'", m_name, varName);
            return false;
        }

        EDefaultShaderData      uniformName = getShaderUniformValueByName(varName);
        EShaderUniformDataType  uniformType = (uniformName == EDefaultShaderData::eUserUniform) 
            ? EShaderUniformDataType::eUniformNone : getShaderUniformTypeByName(varType);

        m_program->setDefaultUniform(varName, uniformType, uniformName);

        varElement = varElement->NextSiblingElement("uniforms");
    }

    return true;
}

bool CRenderPass::parseAttributes(tinyxml2::XMLElement* root)
{
    if (!root)
    {
        LOG_ERROR("Error parse. Not exist xml attributes element");
        return false;
    }

    tinyxml2::XMLElement* varElement = root;
    while (varElement)
    {
        const std::string varName = varElement->Attribute("name");
        if (varName.empty())
        {
            LOG_ERROR("Cannot find uniform name from pass '%s'", m_name);
            return false;
        }

        const std::string varVal = varElement->Attribute("val");
        if (varVal.empty())
        {
            LOG_ERROR("Cannot find uniform val from pass '%s' in '%s'", m_name, varName);
            return false;
        }

        EDefaultShaderData uniformName = getShaderUniformValueByName(varName);

        m_program->setDefaultUniform(varName, EShaderUniformDataType::eUniformNone, uniformName);

        varElement = varElement->NextSiblingElement("uniforms");
    }

    return true;
}

bool CRenderPass::parseSamplers(tinyxml2::XMLElement* root)
{
    if (!root)
    {
        LOG_ERROR("Error parse. Not exist xml element");
        return false;
    }
}

bool CRenderPass::parseRenderTarget(tinyxml2::XMLElement* root)
{
    if (!root)
    {
        LOG_ERROR("Error parse. Not exist xml element");
        return false;
    }
}

bool CRenderPass::parseShaders(tinyxml2::XMLElement* root)
{
    if (!root)
    {
        LOG_ERROR("Error parse. Not exist xml element");
        return false;
    }
}

void CRenderPass::init()
{
    platform::EDriverType type = f3d::CEngine::getInstance()->getPlatform()->getDriverType();

    switch (type)
    {
        case platform::EDriverType::eDriverOpenGL:
        {
            m_program = std::make_shared<CShaderProgramGL>(CShaderProgramGL());
        }
            break;

        case platform::EDriverType::eDriverDirect3D:
        {
            //m_geometry = std::make_shared<CGeometry>(CGeometryGL());
        }
            break;

        default:
        {
            LOG_ERROR("Can`t Choose driver type");
        }
    }
}