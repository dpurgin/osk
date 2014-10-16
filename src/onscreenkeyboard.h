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

#ifndef ONSCREENKEYBOARD_H
#define ONSCREENKEYBOARD_H

#include <QDesktopWidget>
#include <QString>
#include <QWidget>

class QGridLayout;
class OnScreenButton;

class OnScreenKeyboard : public QWidget
{
    Q_OBJECT

public:
    enum DockPosition {
        None,
        Top,
        Bottom
    };

public:
    explicit OnScreenKeyboard(QWidget* parent = 0);
    virtual ~OnScreenKeyboard();

    DockPosition dockPosition() const { return mDockPosition; }
    void setDockPosition(DockPosition dockPosition)
    {
        mDockPosition = dockPosition;

        QDesktopWidget dw;

        if (dockPosition == Bottom)
        {
            setGeometry(0, dw.screenGeometry().height() - keyboardHeight(),
                        dw.screenGeometry().width(), keyboardHeight());
        }
        else if (dockPosition == Top)
        {
            setGeometry(0, 0, dw.screenGeometry().width(), keyboardHeight());
        }
        else
            setGeometry(keyboardLeft(), keyboardTop(), keyboardWidth(), keyboardHeight());
    }

    int keyboardHeight() const { return mKeyboardHeight; }
    void setKeyboardHeight(int keyboardHeight) { mKeyboardHeight = keyboardHeight; }

    int keyboardLeft() const { return mKeyboardLeft; }
    void setKeyboardLeft(int keyboardLeft) { mKeyboardLeft = keyboardLeft; }

    int keyboardTop() const { return mKeyboardTop; }
    void setKeyboardTop(int keyboardTop) { mKeyboardTop = keyboardTop; }

    int keyboardWidth() const { return mKeyboardWidth; }
    void setKeyboardWidth(int keyboardWidth) { mKeyboardWidth = keyboardWidth; }

    bool shiftMode() const { return mShiftMode; }
    void toggleShiftMode()
    {
        mShiftMode = !mShiftMode;
        emit shiftModeToggled();
    }

    bool capsLockMode() const { return mCapsLockMode; }
    void toggleCapsLockMode()
    {
        mCapsLockMode = !mCapsLockMode;
        emit capsLockModeToggled();
    }

signals:
    void shiftModeToggled();
    void capsLockModeToggled();

public slots:
    void loadLayout(const QString& fileName);

private:
    QGridLayout* gridLayout;

    bool mCapsLockMode;
    bool mShiftMode;

    DockPosition mDockPosition;    
    int mKeyboardLeft;
    int mKeyboardTop;
    int mKeyboardHeight;
    int mKeyboardWidth;
};

#endif // ONSCREENKEYBOARD_H
