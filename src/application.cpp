#include "application.h"

#include <QFileInfo>
#include <QSettings>

#include "onscreenkeyboard.h"

Application::Application(int& argc, char* argv[])
    : QApplication(argc, argv),
      mOsk(NULL),

      mOpenLastLayout(false)
{
    QScopedPointer< QSettings > settings(openSettings());

    settings->beginGroup("layout");
        mOpenLastLayout = settings->value("openLastLayout", true).toBool();
        mLastLayout = settings->value("lastLayout").toString();
        mDefaultLayout = settings->value("defaultLayout").toString();
    settings->endGroup();

    if (defaultLayout().isEmpty())
        mDefaultLayout = "en_US.json";

    if (openLastLayout() && lastLayout().isEmpty())
        setLastLayout(defaultLayout());


    mOsk = new OnScreenKeyboard();

    if (openLastLayout())
        mOsk->loadLayout(lastLayout());
    else
        mOsk->loadLayout(defaultLayout());

    settings->beginGroup("position");
        mOsk->setKeyboardLeft(settings->value("left", 0).toInt());
        mOsk->setKeyboardTop(settings->value("top", 0).toInt());
        mOsk->setKeyboardWidth(settings->value("width", 1080).toInt());
        mOsk->setKeyboardHeight(settings->value("height", 300).toInt());

        if (settings->value("dockPosition").toString() == "top")
            mOsk->setDockPosition(OnScreenKeyboard::Top);
        else if (settings->value("dockPosition").toString() == "none")
            mOsk->setDockPosition(OnScreenKeyboard::None);
        else
            mOsk->setDockPosition(OnScreenKeyboard::Bottom);
    settings->endGroup();

    mOsk->show();
}

Application::~Application()
{
    delete mOsk;
}

QSettings* Application::openSettings()
{
    QFileInfo file(qApp->applicationFilePath());

    return new QSettings(file.canonicalPath() + "/" + file.baseName() + ".ini", QSettings::IniFormat);
}
