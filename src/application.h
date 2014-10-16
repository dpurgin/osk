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

#ifndef APPLICATION_H
#define APPLICATION_H

#include <QApplication>
#include <QRect>
#include <QSettings>

#define app (reinterpret_cast< Application* >(qApp))

class OnScreenKeyboard;

class Application : public QApplication
{
    Q_OBJECT

public:
    explicit Application(int& argc, char* argv[]);
    virtual ~Application();

    OnScreenKeyboard* osk() const { return mOsk; }

//    QRect geometry() const { return mGeometry; }

    QString defaultLayout() const { return mDefaultLayout; }
    bool openLastLayout() const { return mOpenLastLayout; }

    QString lastLayout() const { return mLastLayout; }
    void setLastLayout(const QString& lastLayout)
    {
        mLastLayout = lastLayout;

        QScopedPointer< QSettings > settings(openSettings());
        settings->beginGroup("layout");
        settings->setValue("lastLayout", lastLayout);
        settings->endGroup();
    }

signals:

public slots:

private:
    QSettings* openSettings();

private:
    OnScreenKeyboard* mOsk;

    bool mOpenLastLayout;
    QString mDefaultLayout;
    QString mLastLayout;  
};

#endif // APPLICATION_H
