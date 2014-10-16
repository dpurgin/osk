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

#include "onscreenbutton.h"

#include <QMessageBox>
#include <QMouseEvent>

#include <Windows.h>

#include "application.h"
#include "onscreenkeyboard.h"

void OnScreenButton::mouseReleaseEvent(QMouseEvent* event)
{
    QLabel::mouseReleaseEvent(event);

    static QMap< Role, int > vkMap;

    if (vkMap.isEmpty())
    {
        vkMap.insert(Backspace, VK_BACK);
        vkMap.insert(Enter, VK_RETURN);
        vkMap.insert(Tab, VK_TAB);
        vkMap.insert(LeftArrow, VK_LEFT);
        vkMap.insert(RightArrow, VK_RIGHT);
        vkMap.insert(UpArrow, VK_UP);
        vkMap.insert(DownArrow, VK_DOWN);
        vkMap.insert(Space, VK_SPACE);
    }

    if (role() == Character)
    {
        WORD scanCode;

        if ((app->osk()->capsLockMode() && !app->osk()->shiftMode()) ||
                (app->osk()->shiftMode() && !app->osk()->capsLockMode()))
        {
            scanCode = shift().unicode();
        }
        else
            scanCode = main().unicode();

        INPUT input[2];
        input[0].type = INPUT_KEYBOARD;
        input[0].ki.wVk = 0;
        input[0].ki.wScan = scanCode;
        input[0].ki.dwFlags = KEYEVENTF_UNICODE;
        input[0].ki.time = 0;
        input[0].ki.dwExtraInfo = 0;

        input[1].type = INPUT_KEYBOARD;
        input[1].ki.wVk = 0;
        input[1].ki.wScan = scanCode;
        input[1].ki.dwFlags = KEYEVENTF_UNICODE | KEYEVENTF_KEYUP;
        input[1].ki.time = 0;
        input[1].ki.dwExtraInfo = 0;

        SendInput(2, &input[0], sizeof(INPUT));

        if (app->osk()->shiftMode())
            app->osk()->toggleShiftMode();
    }
    else if (vkMap.contains(role()))
    {
        INPUT input[2];
        input[0].type = INPUT_KEYBOARD;
        input[0].ki.wVk = vkMap.value(role());
        input[0].ki.wScan = 0;
        input[0].ki.dwFlags = 0;
        input[0].ki.time = 0;
        input[0].ki.dwExtraInfo = 0;

        input[1].type = INPUT_KEYBOARD;
        input[1].ki.wVk = vkMap.value(role());
        input[1].ki.wScan = 0;
        input[1].ki.dwFlags = KEYEVENTF_KEYUP;
        input[1].ki.time = 0;
        input[1].ki.dwExtraInfo = 0;

        SendInput(2, &input[0], sizeof(INPUT));
    }
    else if (role() == Shift)
    {
        app->osk()->toggleShiftMode();
    }
    else if (role() == CapsLock)
    {
        app->osk()->toggleCapsLockMode();
    }
    else if (role() == LoadLayout)
    {
        app->osk()->loadLayout(layoutFileName());
    }
}

void OnScreenButton::resizeEvent(QResizeEvent* event)
{
    QLabel::resizeEvent(event);

    updateText();
}

void OnScreenButton::init()
{
    setMinimumWidth(40);
    setMinimumHeight(40);

    updateStylesheet();
    updateText();

    if (role() == CapsLock)
        connect(app->osk(), SIGNAL(capsLockModeToggled()),
                this, SLOT(toggle()));
    else if (role() == Shift)
        connect(app->osk(), SIGNAL(shiftModeToggled()),
                this, SLOT(toggle()));
    else
    {
        connect(app->osk(), SIGNAL(capsLockModeToggled()),
                this, SLOT(updateText()));
        connect(app->osk(), SIGNAL(shiftModeToggled()),
                this, SLOT(updateText()));
    }
}

void OnScreenButton::toggle()
{
    mPressed = !mPressed;

    updateStylesheet();
    updateText();
}

void OnScreenButton::updateStylesheet()
{
    QStringList style =
            QStringList() << QString("border: 2px solid;") <<
                             QString("border-radius: 3px;") <<
                             QString("border-top-color: rgb(230, 230, 230);") <<
                             QString("border-right-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(230, 230, 230, 255), stop:1 rgba(100, 100, 100, 255));") <<
                             QString("border-left-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(230, 230, 230, 255), stop:1 rgba(100, 100, 100, 255));");

    if (pressed())
        style.append("background-color: rgb(230, 230, 230)");
    else
        style.append("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(147, 147, 147, 255), stop:1 rgba(0, 0, 0, 255));");

    setStyleSheet(style.join(QChar(';')));

    setAlignment(Qt::AlignCenter);
}

void OnScreenButton::updateText()
{
    static QMap< Role, QString > textMap;

    if (textMap.isEmpty())
    {
        textMap.insert(Backspace, "Bksp");
        textMap.insert(Enter, QChar(0x23ce));
        textMap.insert(Tab, "Tab");
        textMap.insert(CapsLock, "Caps");
        textMap.insert(Shift, "Shift");
        textMap.insert(UpArrow, QChar(0x2191));
        textMap.insert(DownArrow, QChar(0x2193));
        textMap.insert(LeftArrow, QChar(0x2190));
        textMap.insert(RightArrow, QChar(0x2192));
    }

    int mainFontSize = height() * 0.6;
    int shiftFontSize = height() * 0.3;

    QString primary;
    QString secondary;

    if (role() == Character)
    {
        if ((app->osk()->shiftMode() && !app->osk()->capsLockMode()) ||
                (app->osk()->capsLockMode() && !app->osk()->shiftMode()))
        {
            primary.append(shift());
            secondary.append(main());
        }
        else
        {
            primary.append(main());
            secondary.append(shift());
        }
    }
    else
        primary = textMap.value(role(), title());

    QString t;

    t = QString::fromUtf8("<span style=\"font-face: 'Segoe UI Light'; color: %1; font-size: %2px\">%3</span>");
    t = t.arg(pressed()? "black": "white").arg(mainFontSize).arg(primary);

    if (showShift())
    {
        secondary = QString::fromUtf8("<span style=\"font-face: 'Segoe UI Light'; color: grey; font-size: %1px\">&nbsp;%2</span>")
                .arg(shiftFontSize)
                .arg(secondary);

        t.append(secondary);
    }

    setText(t);
}
