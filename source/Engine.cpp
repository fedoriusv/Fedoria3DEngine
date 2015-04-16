#include "Engine.h"
#include "platform/Platform.h"
#include "event/InputEventHandler.h"
#include "event/EventManager.h"
#include "scene/RenderTechniqueManager.h"
#include "scene/TextureManager.h"
#include "scene/FontManager.h"
#include "scene/ModelManager.h"
#include "scene/RenderTargetManager.h"
#include "scene/ShaderManager.h"
#include "utils/Timer.h"

using namespace v3d;
using namespace platform;
using namespace event;
using namespace scene;
using namespace renderer;
using namespace utils;

CEngine::CEngine()
    : m_platform(nullptr)
    , m_inputEventHandler(nullptr)
    , m_scene(nullptr)

    , m_frameTime(0)
    , m_lastTime(0)
{
    m_platform = std::make_shared<CPlatform>();
    m_inputEventHandler = std::make_shared<CInputEventHandler>();
    m_scene = std::make_shared<CSceneManager>();

    m_inputEventHandler->setEnableEvents(true);

    m_lastTime = CTimer::getCurrentTime();
}

CEngine::~CEngine()
{
    m_inputEventHandler->setEnableEvents(false);

    CRenderTechniqueManager::freeInstance();
    CTextureManager::freeInstance();
    CFontManager::freeInstance();
    CModelManager::freeInstance();
    CEventManager::freeInstance();
    CRenderTargetManager::freeInstance();
    CShaderManager::freeInstance();
}

bool CEngine::init()
{
    if (m_platform->hasError())
    {
        return false;
    }

    if (!m_platform->init())
    {
        return false;
    }

    return true;
}

const PlatformPtr& CEngine::getPlatform() const
{
    return m_platform;
}

const InputEventHandlerPtr& CEngine::getInputEventHandler() const
{
    return m_inputEventHandler;
}

const SceneManagerPtr& CEngine::getSceneManager() const
{
    return m_scene;
}

bool CEngine::begin()
{
    if (!m_platform)
    {
        return false;
    }

    u64 ticks = CTimer::getCurrentTime();
    m_frameTime = (s32)((ticks - m_lastTime) * m_timeFactor);
    m_lastTime = ticks;

    if (m_frameTime <= 0)
    {
        m_frameTime = 1;
    }

    m_inputEventHandler->update();
    m_scene->draw(m_frameTime);

    return m_platform->begin();
}

bool CEngine::end()
{
    if (!m_platform)
    {
        return false;
    }

    return m_platform->end();
}

const WindowPtr CEngine::getWindow() const
{
    if (!m_platform)
    {
        return nullptr;
    }

    return m_platform->getWindow();
}

const RendererPtr CEngine::getRenderer() const
{
    if (!m_platform)
    {
        return nullptr;
    }

    return m_platform->getRenderer();
}

const renderer::DriverContextPtr CEngine::getContext() const
{
    if (!m_platform)
    {
        return nullptr;
    }

    return m_platform->getRenderer()->getContext();
}