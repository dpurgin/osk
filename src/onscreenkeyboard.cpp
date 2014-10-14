#include "onscreenkeyboard.h"

#include <QDesktopWidget>
#include <QDir>
#include <QFile>
#include <QGridLayout>
#include <QMap>
#include <QMessageBox>

#include <Windows.h>

#include <jansson.h>

#include "application.h"
#include "onscreenbutton.h"

OnScreenKeyboard::OnScreenKeyboard(QWidget* parent)
    : QWidget(parent),
      mShiftMode(false),
      mCapsLockMode(false),
      gridLayout(NULL),
      mDockPosition(Bottom),
      mKeyboardLeft(0),
      mKeyboardTop(0),
      mKeyboardWidth(1080),
      mKeyboardHeight(300)
{
    setStyleSheet("background:black");

    setDockPosition(Bottom);

    setWindowFlags(Qt::FramelessWindowHint);

    HWND hWnd = (HWND)winId();

    if (hWnd)
    {
        LONG_PTR exStyle = GetWindowLongPtr(hWnd, GWL_EXSTYLE);
        SetWindowLongPtr(hWnd, GWL_EXSTYLE, exStyle | WS_EX_NOACTIVATE);
    }

    gridLayout = new QGridLayout(this);
}

OnScreenKeyboard::~OnScreenKeyboard()
{

}

void OnScreenKeyboard::loadLayout(const QString& fileName)
{
    static QMap< QString, OnScreenButton::Role > rolesMap;

    if (rolesMap.empty())
    {
        rolesMap.insert("backspace", OnScreenButton::Backspace);
        rolesMap.insert("enter", OnScreenButton::Enter);
        rolesMap.insert("tab", OnScreenButton::Tab);
        rolesMap.insert("shift", OnScreenButton::Shift);
        rolesMap.insert("capslock", OnScreenButton::CapsLock);
        rolesMap.insert("left", OnScreenButton::LeftArrow);
        rolesMap.insert("right", OnScreenButton::RightArrow);
        rolesMap.insert("up", OnScreenButton::UpArrow);
        rolesMap.insert("down", OnScreenButton::DownArrow);
        rolesMap.insert("space", OnScreenButton::Space);
    }

    QDir dir(qApp->applicationDirPath() + "/layouts");

    QFile inputFile(dir.absoluteFilePath(fileName));

    if (!inputFile.exists())
    {
        QMessageBox::critical(NULL,
                              tr("Error"),
                              QString(tr("Keyboard layout does not exist: %1")).arg(fileName));

        return;
    }

    json_error_t jsonParseError;

    json_t* root = json_load_file(inputFile.fileName().toAscii().data(), 0, &jsonParseError);


    if (!root)
    {
        QMessageBox::critical(NULL,
                              tr("Error"),
                              QString(tr("Unable to parse keyboard layout: %1")).arg(jsonParseError.text));

        return;
    }

    if (!json_is_object(root))
    {
        QMessageBox::critical(NULL,
                              tr("Error"),
                              tr("Keyboard layout root must be of type Object"));
        json_decref(root);
        return;
    }

    while (gridLayout->count() > 0)
    {
        QLayoutItem* item = gridLayout->takeAt(0);
        delete item->widget();
        delete item;
    }

    int defaultRowSpan =
            json_is_integer(json_object_get(root, "rowSpan"))?
                json_integer_value(json_object_get(root, "rowSpan")):
                1;

    int defaultColSpan =
            json_is_integer(json_object_get(root, "colSpan"))?
                json_integer_value(json_object_get(root, "colSpan")):
                1;

    bool defaultShowShift =
            json_is_boolean(json_object_get(root, "showShift"))?
                json_boolean_value(json_object_get(root, "showShift")):
                false;

    json_t* rowsArray = json_object_get(root, "rows");

    if (!json_is_array(rowsArray))
    {
        QMessageBox::critical(this,
                              tr("Error"),
                              tr("Keyboard layout must contain 'rows' of type Array"));

        json_decref(root);

        return;
    }

    size_t rowIdx = 0;
    json_t* rowObject = NULL;

    json_array_foreach(rowsArray, rowIdx, rowObject)
    {
        if (!json_is_object(rowObject))
        {
            QMessageBox::critical(this,
                                  tr("Error"),
                                  QString(tr("rows[%1] must be of type Object")).arg(rowIdx));

            json_decref(root);

            return;
        }

        json_t* itemsArray = json_object_get(rowObject, "items");

        if (!json_is_array(itemsArray))
        {
            QMessageBox::critical(this,
                                  tr("Error"),
                                  QString(tr("rows[%1] must contain 'items' of type Array")).arg(rowIdx));

            json_decref(root);

            return;
        }

        int defaultRowSpanAtRow =
                json_is_integer(json_object_get(rowObject, "rowSpan"))?
                    json_integer_value(json_object_get(rowObject, "rowSpan")):
                    defaultRowSpan;

        int defaultColSpanAtRow =
                json_is_integer(json_object_get(rowObject, "colSpan"))?
                    json_integer_value(json_object_get(rowObject, "colSpan")):
                    defaultColSpan;

        bool defaultShowShiftAtRow =
                json_is_boolean(json_object_get(rowObject, "showShift"))?
                    json_boolean_value(json_object_get(rowObject, "showShift")):
                    defaultShowShift;

        size_t itemIdx = 0;
        size_t colIdx = 0;

        json_t* itemObject = NULL;

        json_array_foreach(itemsArray, itemIdx, itemObject)
        {
            if (!json_is_object(itemObject))
            {
                QMessageBox::critical(NULL,
                                      tr("Error"),
                                      QString(tr("rows[%1].items[%2] must be of type Object")).arg(rowIdx).arg(itemIdx));

                json_decref(root);

                return;

            }

            OnScreenButton* button = NULL;

            json_t* roleValue = json_object_get(itemObject, "role");

            if (json_is_string(roleValue) && QLatin1String(json_string_value(roleValue)) == QLatin1String("loadlayout"))
            {
                QString layout = QString::fromUtf8(json_string_value(json_object_get(itemObject, "layout")));
                QString title = QString::fromUtf8(json_string_value(json_object_get(itemObject, "title")));

                button = new OnScreenButton(layout, title);
            }
            // role may be empty. If so, considered to be equal to 'char'
            else if (!json_is_string(roleValue) || QLatin1String(json_string_value(roleValue)) == QLatin1String("char"))
            {
                QChar main;
                QChar shift;

                json_t* mainValue = json_object_get(itemObject, "main");
                json_t* shiftValue = json_object_get(itemObject, "shift");

                if (json_is_integer(mainValue))
                    main = QChar(static_cast< int >(json_integer_value(mainValue)));
                else if (json_is_string(mainValue))
                    main = QString::fromUtf8(json_string_value(mainValue)).at(0);

                if (json_is_integer(shiftValue))
                    shift = QChar(static_cast< int >(json_integer_value(shiftValue)));
                else if (json_is_string(shiftValue))
                    shift = QString::fromUtf8(json_string_value(shiftValue)).at(0);

                button = new OnScreenButton(main, shift);
            }
            else
            {
                button = new OnScreenButton(
                            rolesMap.value(QLatin1String(json_is_string(roleValue)? json_string_value(roleValue): ""),
                            OnScreenButton::Undefined));
            }

            if (button)
            {
                int rowSpan =
                        json_is_integer(json_object_get(itemObject, "rowSpan"))?
                            json_integer_value(json_object_get(itemObject, "rowSpan")):
                            defaultRowSpanAtRow;

                int colSpan =
                        json_is_integer(json_object_get(itemObject, "colSpan"))?
                            json_integer_value(json_object_get(itemObject, "colSpan")):
                            defaultColSpanAtRow;

                bool showShift =
                        json_is_boolean(json_object_get(itemObject, "showShift"))?
                            json_boolean_value(json_object_get(itemObject, "showShift")):
                            defaultShowShiftAtRow;

                if (json_is_string(json_object_get(itemObject, "title")))
                    button->setTitle(QString::fromUtf8(json_string_value(json_object_get(itemObject, "title"))));

                button->setShowShift(showShift);

                int col = colIdx;

                if (json_is_integer(json_object_get(itemObject, "col")))
                    col = json_integer_value(json_object_get(itemObject, "col"));

                gridLayout->addWidget(button, rowIdx, col, rowSpan, colSpan);

                colIdx += colSpan;
            }
        }
    }

    json_decref(root);

    app->setLastLayout(fileName);
}
