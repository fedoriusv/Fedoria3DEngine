#include "LightAnimator.h"
#include "Valera3D.h"

using namespace v3d;
using namespace core;
using namespace utils;
using namespace scene;
using namespace event;

LightAnimator::LightAnimator(const v3d::scene::SceneManagerPtr & scene)
    : m_light(nullptr)
    , m_scene(scene)
    , m_position(Vector3D(0.0f))
    , m_radius(0.0f)
    , m_speed(0.0f)
    , m_index(0)
    , m_pause(false)
{
    m_timer = new CTimer();
}

LightAnimator::~LightAnimator()
{
    m_scene->dropNode(m_light);

    if (m_timer)
    {
        delete m_timer;
        m_timer = nullptr;
    }
}

void LightAnimator::createLight(const v3d::core::Vector3D& pos, v3d::f32 radius, v3d::f32 speed)
{
    m_position = pos;
    m_radius = radius;
    m_speed = speed;

    m_light = m_scene->addLight();

    CShape* cube = m_scene->addCube(m_light, Vector3D(0, 0, 0));
    cube->setName("debug_light");
    cube->setRenderTechnique("shaders/simple.xml");
    cube->setScale(Vector3D(0.05f));

    LightAnimator::calcAnimatorPoints();

    m_timer->setTime(0);
    m_timer->start();
}

void LightAnimator::update(u32 dt)
{
    if (!m_light)
    {
        return;
    }

    if (m_pause)
    {
        return;
    }

    const u64 ticks = CTimer::getCurrentTime();
    const u32 fpsDeltaTime2 = static_cast<u32>(ticks - m_lastTime2);
    if (fpsDeltaTime2 > 80) //1 sec
    {
        m_lastTime2 = static_cast<u32>(ticks);
        ++m_index;
    }

    const u32 fpsDeltaTime = static_cast<u32>(ticks - m_lastTime);
    if (fpsDeltaTime > 10) //1 sec
    {
        m_lastTime = static_cast<u32>(ticks);

        if (m_index >= m_points.size() || m_index < 0)
        {
            m_index = 0;
        }
        Vector3D& point = m_points[m_index];

        const Vector3D& pos = m_light->getPosition();
        point.y = pos.y;

        Vector3D newpos = core::interpolate(pos, point, 1.0f/(f32)dt);
       m_light->setPosition(newpos);
    }

    //u32 currTime = m_timer->getTime();
    //if (currTime > 1000)
    //{
    //    m_timer->setTime(0);
    //    //m_light->setPosition()
    //}
}

v3d::scene::CLight * LightAnimator::getLight() const
{
    return m_light;
}

void LightAnimator::onKeyboard(const v3d::event::KeyboardInputEventPtr & event)
{
    if (!m_light)
    {
        return;
    }

    if (event->_key == EKeyCode::eKeySpace && event->_event == EKeyboardInputEvent::eKeyboardPressDown)
    {
        m_pause = !m_pause;
    }
}

void LightAnimator::calcAnimatorPoints()
{
    m_points.clear();

    u32 step = 50;
    f32 drho = k_pi / step;
    f32 posX = m_position.x + m_radius;
    f32 posY = m_position.z;

    for (u32 i = 0; i < step; ++i)
    {
        f32 rho = (f32)i * drho;
        posX = m_position.x + (m_radius * cos(rho + drho));
        posY = m_position.z + (m_radius * sin(rho + drho));
        m_points.push_back(Vector3D(posX, 0.0f, posY));
    }


    for (s32 i = step - 1; i >= 0; --i)
    {
        f32 rho = (f32)i * drho;
        posX = m_position.x + (m_radius * cos(rho + drho));
        posY = m_position.z + (m_radius * -sin(rho + drho));
        m_points.push_back(Vector3D(posX, 0.0f, posY));
    }
}
