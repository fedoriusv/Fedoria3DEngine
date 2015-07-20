#include "ShaderManager.h"
#include "utils/Logger.h"
#include "stream/StreamManager.h"

using namespace v3d;
using namespace scene;
using namespace stream;
using namespace resources;

CShaderManager::CShaderManager()
{
    CShaderManager::registerPath("../../../../data/shaders/");
    CShaderManager::registerPath("../../../../../data/shaders/");
    CShaderManager::registerPath("data/shaders/");
}

CShaderManager::~CShaderManager()
{
    unloadAll();
}

void CShaderManager::add(const ShaderSourceDataPtr& shader)
{
    std::string name = shader->getName();
    m_resources.insert(std::map<std::string, ShaderSourceDataPtr>::value_type(name, shader));
}

const ShaderSourceDataPtr CShaderManager::load(const std::string& name, const std::string& alias)
{
    std::string nameStr = name;
    std::transform(name.begin(), name.end(), nameStr.begin(), ::tolower);

    std::string fileExtension;

    const size_t pos = nameStr.find('.');
    if (pos != std::string::npos)
    {
        fileExtension = std::string(nameStr.begin() + pos, nameStr.end());
    }

    for (std::string& path : m_pathes)
    {
        const std::string fullName = path + nameStr;
        const bool isFileExist = stream::FileStream::isFileExist(fullName);
        if (isFileExist)
        {
            const stream::FileStreamPtr stream = stream::CStreamManager::createFileStream(fullName, stream::FileStream::e_in);
            if (stream->isOpen())
            {
                ShaderSourceDataPtr shader = std::make_shared<CShaderSourceData>();

                shader->init(stream);
                shader->setResourseName(fullName);
                const std::string fullPath = fullName.substr(0, fullName.find_last_of("/") + 1);
                shader->setResourseFolder(fullPath);

                if (!shader->load())
                {
                    LOG_ERROR("CShaderManager: Streaming error read file [%s]", nameStr.c_str());
                    stream->close();

                    return nullptr;
                }
                stream->close();

                m_resources.insert(std::map<std::string, ShaderSourceDataPtr>::value_type(nameStr, shader));

                return shader;
            }
        }
    }

    LOG_WARNING("CShaderManager::load: File [%s] not found", name.c_str());
    return nullptr;
}
