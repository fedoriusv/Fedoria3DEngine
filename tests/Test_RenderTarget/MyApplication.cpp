#include "MyApplication.h"
#include "Valera3D.h"

#include "scene/TargetManager.h"

using namespace v3d;
using namespace core;
using namespace scene;
using namespace stream;
using namespace event;
using namespace renderer;

MyApplication::MyApplication(int& argc, char** argv)
    : BaseApplication(argc, argv)
    , m_activeTarget(true)
{
    BaseApplication::getPlatform()->createWindowWithContext(Dimension2D(1024, 768));
}

MyApplication::~MyApplication()
{
}

void MyApplication::init()
{
    if (!m_activeTarget)
    {
        CRectangleShape* screen = BaseApplication::getSceneManager()->addRectangle(0, Rect32(0, 0, 1024, 768));
        screen->setName("screen");
        screen->setRenderTechnique("shaders/screen2D.xml");
    }
    else
    {
        CRectangleShape* screen1 = BaseApplication::getSceneManager()->addRectangle(0, Rect32(0, 0, 512, 384));
        screen1->setName("screen1");
        screen1->setRenderTechnique("shaders/screen2DNFAA.xml");
        screen1->getRenderTechique()->getRenderPass(0)->getShaderProgram()->setDefine("USE_NFAA");

        CRectangleShape* screen2 = BaseApplication::getSceneManager()->addRectangle(0, Rect32(512, 0, 1024, 384));
        screen2->setName("screen2");
        screen2->setRenderTechnique("shaders/screen2DFXAA.xml");
        screen2->getRenderTechique()->getRenderPass(0)->getShaderProgram()->setDefine("USE_FXAA");

        CRectangleShape* screen3 = BaseApplication::getSceneManager()->addRectangle(0, Rect32(512, 384, 1024, 768));
        screen3->setName("screen3");
        screen3->setRenderTechnique("shaders/screen2DMSAA.xml");
    }

    CShape* cube = BaseApplication::getSceneManager()->addCube(0, Vector3D(0, 1, -3));
    cube->setName("cube");
    cube->setRenderTechnique("shaders/texture_target.xml");

    CShape* cube2 = BaseApplication::getSceneManager()->addCube(0, Vector3D(2, 0, -5));
    cube2->setName("cube2");
    cube2->setRenderTechnique("shaders/texture_target.xml");
    cube2->getMaterial()->setTexture(0, "textures/wall.jpg");

    CNode* fpsCamera = BaseApplication::getSceneManager()->addFPSCamera(0, Vector3D(0, 0, 0), Vector3D(0.7f, 0, 0.7f));
    fpsCamera->setName("fpsCamera");
    CNode* camera = BaseApplication::getSceneManager()->addCamera(0, Vector3D(0, 0, 1), Vector3D(0.0f, 0, -1.0f));
    camera->setName("camera");

    BaseApplication::getInputEventHandler()->connectKeyboardEvent(std::bind(&MyApplication::onKeyboard, this, std::placeholders::_1));
    BaseApplication::getInputEventHandler()->connectMouseEvent(std::bind(&MyApplication::onMouse, this, std::placeholders::_1));
    BaseApplication::getInputEventHandler()->connectGamepadEvent(std::bind(&MyApplication::onGamepad, this, std::placeholders::_1));
}

void MyApplication::run()
{
    //Main loop
    /*const RenderTargetPtr target = std::static_pointer_cast<CRenderTarget>(CTargetManager::getInstance()->get("targetTestRight"));
    CTexture* texture = target ? target->getColorTexture(2) : nullptr;
    if (texture)
    {
        using typeId = f32;
        u32 size = texture->getSize().getArea();
        void* data = malloc(size * sizeof(typeId));
        texture->readData(data);

        typeId* castData = (typeId*)data;
        typeId min = castData[0];
        typeId max = castData[0];
        for (u32 i = 0; i < size; ++i)
        {
            max = std::max(max, castData[i]);
            min = std::min(min, castData[i]);
        }

        free(data);
    }*/
}

void MyApplication::onMouse(const event::MouseInputEventPtr& event)
{
}

void MyApplication::onGamepad(const GamepadInputEventPtr& event)
{
}

void MyApplication::onKeyboard(const KeyboardInputEventPtr& event)
{
    f32 step = 5.5f;
    f32 angle = 10.0f;
    static bool debug = false;

    if (event->_event == event::EKeyboardInputEvent::eKeyboardPressDown)
    {

        if (event->_key == EKeyCode::eKeyEscape)
        {
            getPlatform()->closeWindow();
        }
        if (event->_key == EKeyCode::eKeyKey_N)
        {
            debug = !debug;
#ifdef _DEBUG
            BaseApplication::getSceneManager()->setDebugMode(debug);
#endif
        }
        if (event->_key == EKeyCode::eKeyKey_V)
        {
            CCamera* fpsCamera = static_cast<CCamera*>(getSceneManager()->getObjectByName("fpsCamera"));
            CCamera* camera = static_cast<CCamera*>(getSceneManager()->getObjectByName("camera"));
            if (!fpsCamera || !camera)
            {
                return;
            }

            if (getSceneManager()->isActiveCamera(fpsCamera))
            {
                camera->setPosition(fpsCamera->getPosition());
                camera->setTarget(fpsCamera->getTarget());
                getSceneManager()->setActiveCamera(camera);
            }
            else if (getSceneManager()->isActiveCamera(camera))
            {
                fpsCamera->setPosition(camera->getPosition());
                fpsCamera->setTarget(camera->getTarget());
                getSceneManager()->setActiveCamera(fpsCamera);
            }

        }

        if (event->_key == EKeyCode::eKeyKey_F)
        {
            static int enable = 1;
            enable = (enable == 1) ? 0 : 1;

            CNode* screen1 = getSceneManager()->getObjectByName("screen1");
            if (screen1)
            {
                if (enable)
                {
                    static_cast<CRectangleShape*>(screen1)->getRenderTechique()->getRenderPass(0)->getShaderProgram()->setDefine("USE_NFAA");
                }
                else
                {
                    static_cast<CRectangleShape*>(screen1)->getRenderTechique()->getRenderPass(0)->getShaderProgram()->setUndefine("USE_NFAA");
                }
            }

            CNode* screen2 = getSceneManager()->getObjectByName("screen2");
            if (screen2)
            {
                if (enable)
                {
                    static_cast<CRectangleShape*>(screen2)->getRenderTechique()->getRenderPass(0)->getShaderProgram()->setDefine("USE_FXAA");
                }
                else
                {
                    static_cast<CRectangleShape*>(screen2)->getRenderTechique()->getRenderPass(0)->getShaderProgram()->setUndefine("USE_FXAA");
                }
            }

            CNode* screen3 = getSceneManager()->getObjectByName("screen3");
            if (screen3)
            {
                static_cast<CRectangleShape*>(screen3)->getRenderTechique()->getRenderPass(0)->getShaderProgram()->setDefine("USE_MSAA", enable ? "1" : "0");
            }
        }

        ///
        CNode* node = getSceneManager()->getObjectByName("cube");
        if (!node)
        {
            return;
        }

        if (event->_key == EKeyCode::eKeyUp && event->_event == EKeyboardInputEvent::eKeyboardPressDown)
        {
            node->setRotation(Vector3D(node->getRotation().x + angle, node->getRotation().y, node->getRotation().z));
        }
        if (event->_key == EKeyCode::eKeyDown && event->_event == EKeyboardInputEvent::eKeyboardPressDown)
        {
            node->setRotation(Vector3D(node->getRotation().x - angle, node->getRotation().y, node->getRotation().z));
        }
        if (event->_key == EKeyCode::eKeyLeft && event->_event == EKeyboardInputEvent::eKeyboardPressDown)
        {
            node->setRotation(Vector3D(node->getRotation().x, node->getRotation().y + angle, node->getRotation().z));
        }
        if (event->_key == EKeyCode::eKeyRight && event->_event == EKeyboardInputEvent::eKeyboardPressDown)
        {
            node->setRotation(Vector3D(node->getRotation().x, node->getRotation().y - angle, node->getRotation().z));
        }

        getPlatform()->getWindow()->setCaption("x= " + std::to_string(node->getRotation().x) + "; y = " + std::to_string(node->getRotation().y) + "; z = " + std::to_string(node->getRotation().z));
    }
}
