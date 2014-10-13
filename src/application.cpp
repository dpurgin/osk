#include "application.h"

#include "onscreenkeyboard.h"

Application::Application(int& argc, char* argv[])
    : QApplication(argc, argv),
      mOsk(NULL)
{
    mOsk = new OnScreenKeyboard();

    mOsk->loadLayout("en_US.json");

    mOsk->show();
}

Application::~Application()
{
    delete mOsk;
}
