#include "ShaderProgramGL.h"
#include "ShaderGL.h"
#include "utils/Logger.h"
#include "GL/glew.h"

using namespace f3d;
using namespace f3d::renderer;

CShaderProgramGL::CShaderProgramGL(const ShaderDataPtr& data)
    : CShaderProgram(data)
{
}

CShaderProgramGL::~CShaderProgramGL()
{
}

bool CShaderProgramGL::create()
{
    std::vector<u32> shadersId;
    CShaderProgramGL::getShaderIDArray(shadersId);

    bool status = CShaderProgramGL::initProgram(m_shaderProgID, shadersId);
    shadersId.clear();

    return status;

}

bool CShaderProgramGL::create(const std::string& vShader, const std::string& fShader, u32 arg, ...)
{
    if (vShader.empty() || fShader.empty())
    {
        ASSERT(false && "Empty Shader FileName");
        return false;
    }

    ShaderPtr vshader = std::make_shared<CShaderGL>();
    vshader->create(vShader, EShaderType::eTypeVertex);
    CShaderProgram::addShader(vshader);

    ShaderPtr fshader = std::make_shared<CShaderGL>();
    fshader->create(fShader, EShaderType::eTypeFragment);
    CShaderProgram::addShader(fshader);


    va_list argList;
    va_start(argList, arg);
    for (u32 i = 0; i < arg; i += 2)
    {
        char* strName = va_arg(argList, char*);
        int type = va_arg(argList, int);

        ShaderPtr shader = std::make_shared<CShaderGL>();
        shader->create(strName, (EShaderType)type);

        CShaderProgram::addShader(shader);
    }
    va_end(argList);


    std::vector<u32> shadersId;
    CShaderProgram::getShaderIDArray(shadersId);

    bool status = CShaderProgramGL::initProgram(m_shaderProgID, shadersId);
    shadersId.clear();

    return status;
}

void CShaderProgramGL::destroy()
{
    for (auto shader : m_shaderList)
    {
        CShaderProgramGL::destroyShader(shader);
    }

    CShaderProgramGL::deleteProgram(m_shaderProgID);
}

void CShaderProgramGL::bind()
{
    CShaderProgramGL::useProgram(m_shaderProgID);
}

void CShaderProgramGL::unbind()
{
    CShaderProgramGL::useProgram(0);
}

bool CShaderProgramGL::initProgram(u32& shaderProgram, std::vector<u32>& shaders)
{
    shaderProgram = glCreateProgram();
    ASSERT(glIsProgram(shaderProgram) || "Invalid Index Created Shader program");

    for (u32 i = 0; i < shaders.size(); ++i)
    {
        CShaderProgramGL::attachShader(shaderProgram, shaders[i]);
    }

    const AttributeList& data = m_shaderData->getAttributeList();
    for (auto attribute : data)
    {
        const std::string& name = attribute.second->getAttributeName();
        EShaderAttribute type = attribute.second->getAttributeType();

        CShaderProgramGL::bindAttrib(shaderProgram, type, name);
    }

    glLinkProgram(shaderProgram);
    glValidateProgram(shaderProgram);

    for (auto attribute : data)
    {
        const std::string& name = attribute.second->getAttributeName();
        EShaderAttribute type = attribute.second->getAttributeType();
        
        s32 id = CShaderProgramGL::getAttrib(shaderProgram, name);
        if ((EShaderAttribute)id != type)
        {
            LOG_ERROR("InitShaderProgram: Invalid attribute Index for: %s", name);
        }
    }

    GLint linkStatus;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &linkStatus);
    if (!linkStatus)
    {
        LOG_ERROR("InitShaderProgram: shader program not compiled id: %s", shaderProgram);
#ifdef _DEBUG
        GLint length;
        GLint charsWritten;
        glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &length);

        GLchar* buffer = new GLchar[length];
        glGetProgramInfoLog(shaderProgram, length, &charsWritten, buffer);
        if (strlen(buffer) > 0)
        {
            LOG_ERROR("Shader Program Error: %s", buffer);
        }
#endif
    }

    return (linkStatus == GL_TRUE) ? true : false;
}

void CShaderProgramGL::attachShader(u32 shaderProgram, u32 shader)
{
    ASSERT(glIsProgram(shaderProgram) || "Invalid Index Attachment Shader program");
    ASSERT(glIsShader(shader) || "Invalid Index Attached Shader program");
    glAttachShader(shaderProgram, shader);
}

void CShaderProgramGL::detachShader(u32 shaderProgram, u32 shader)
{
    ASSERT(glIsProgram(shaderProgram) || "Invalid Index Detachment Shader program");
    ASSERT(glIsShader(shader) || "Invalid Index detach Shader program");
    glDetachShader(shaderProgram, shader);
}

void CShaderProgramGL::bindAttrib(u32 shaderProgram, EShaderAttribute type, const std::string& name)
{
    ASSERT(glIsProgram(shaderProgram) || "Invalid Index bindAttrib Shader program");
    glBindAttribLocation(shaderProgram, type, name.c_str());
}

int CShaderProgramGL::getAttrib(u32 shaderProgram, const std::string& name)
{
    ASSERT(glIsProgram(shaderProgram) || "Invalid Index getAttrib Shader program");
    return (s32)glGetAttribLocation(shaderProgram, name.c_str());
}

void CShaderProgramGL::deleteProgram(u32 shaderProgram)
{
    if (shaderProgram > 0)
    {
        ASSERT(glIsProgram(shaderProgram) || "Invalid Index delete Shader program");
        glDeleteProgram(shaderProgram);
    }
}

void CShaderProgramGL::useProgram(u32 shaderProgram)
{
    ASSERT(glIsProgram(shaderProgram) || "Invalid Index bind Shader program");
    glUseProgram(shaderProgram);
}