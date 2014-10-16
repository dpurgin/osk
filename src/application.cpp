/*
   Name: On-Screen Keyboard for Windows
   Author: Dmitriy Purgin <dmitriy.purgin@ibsproject.kz>
   Organization: IBS Project LLC

   Copyright (c) 2014 IBS Project LLC

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

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
