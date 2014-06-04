#include "StreamManager.h"
#include "FileStream.h"

using namespace v3d;
using namespace v3d::stream;

CStreamManager::CStreamManager()
{
}

CStreamManager::~CStreamManager()
{
}

FileStream* CStreamManager::createFileStream(const std::string& file)
{
    return new FileStream(file);
}