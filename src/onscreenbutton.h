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

#ifndef ONSCREENBUTTON_H
#define ONSCREENBUTTON_H

#include <QLabel>

class OnScreenButton : public QLabel
{
    Q_OBJECT

    Q_PROPERTY(bool mPressed READ pressed WRITE setPressed NOTIFY pressedChanged)

public:
    enum Role {
        Undefined,

        Character,

        Backspace,
        Enter,
        Tab,
        Shift,
        CapsLock,
        LeftArrow,
        RightArrow,
        UpArrow,
        DownArrow,
        Space,

        LoadLayout
    };

public:
    OnScreenButton(Role role, const QString& title = QString(), QWidget* parent = 0)
        : QLabel(parent),
          mRole(role),
          mShowShift(false),
          mPressed(false),
          mTitle(title)
    {
        init();
    }

    OnScreenButton(const QChar& main, const QChar& shift, QWidget* parent = 0)
        : QLabel(parent),
          mRole(Character),
          mMain(main),
          mShift(shift),
          mShowShift(false),
          mPressed(false)
    {
        init();
    }

    OnScreenButton(const QString& layoutFileName, const QString& title, QWidget* parent = 0)
        : QLabel(parent),
          mRole(LoadLayout),
          mShowShift(false),
          mPressed(false),
          mTitle(title),
          mLayoutFileName(layoutFileName)
    {
        init();
    }

    QString layoutFileName() const { return mLayoutFileName; }

    QChar main() const { return mMain; }
    Role role() const { return mRole; }
    QChar shift() const { return mShift; }

    bool pressed() const { return mPressed; }
    void setPressed(bool pressed) { emit pressedChanged(mPressed = pressed); }

    QString title() const { return mTitle; }
    void setTitle(const QString& title) { mTitle = title; }

    bool showShift() const { return mShowShift; }
    void setShowShift(bool val) { mShowShift = val; }

signals:
    void pressedChanged(bool pressed);

public slots:

protected:
    void mouseReleaseEvent(QMouseEvent* event);
    void resizeEvent(QResizeEvent* event);

private:
    void init();

private slots:
    void updateStylesheet();
    void updateText();

    void toggle();

private:
    Role mRole;

    QChar mMain;
    QChar mShift;

    bool mShowShift;

    bool mPressed;

    QString mTitle;

    QString mLayoutFileName;
};

#endif // ONSCREENBUTTON_H
