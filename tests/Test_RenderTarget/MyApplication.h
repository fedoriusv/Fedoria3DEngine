#ifndef _MY_APPLICATION_H_
#define _MY_APPLICATION_H_

#include "BaseApplication.h"

class MyApplication : public v3d::BaseApplication
{
public:

    MyApplication(int& argc, char** argv);
    virtual ~MyApplication();

    void    init() override;
    void    run()  override;

    void    onKeyboard(const v3d::event::KeyboardInputEventPtr& event);
    void    onMouse(const v3d::event::MouseInputEventPtr& event);
    void    onGamepad(const v3d::event::GamepadInputEventPtr& event);

private:

    bool    m_activeTarget;
};

#endif //_MY_APPLICATION_H_