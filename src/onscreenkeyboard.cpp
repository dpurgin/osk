#include "onscreenkeyboard.h"

#include <QDesktopWidget>
#include <QDir>
#include <QFile>
#include <QGridLayout>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonParseError>
#include <QMap>
#include <QMessageBox>

#include <Windows.h>

#include "application.h"
#include "onscreenbutton.h"

OnScreenKeyboard::OnScreenKeyboard(QWidget* parent)
    : QWidget(parent),
      mShiftMode(false),
      mCapsLockMode(false),
      gridLayout(NULL)
{
    setStyleSheet("background:black");

    QDesktopWidget dw;

    setGeometry(0, dw.screenGeometry().height() - 320,
                dw.screenGeometry().width(), 300);

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

    QDir dir(qApp->applicationDirPath());

    QFile inputFile(dir.absoluteFilePath(fileName));

    if (!inputFile.open(QFile::ReadOnly))
    {
        QMessageBox::critical(this,
                              tr("Error"),
                              QString(tr("Unable to open keyboard layout: %1")).arg(fileName));

        return;
    }

    QJsonParseError jsonParseError;

    QJsonDocument doc = QJsonDocument::fromJson(inputFile.readAll(), &jsonParseError);

    inputFile.close();

    if (jsonParseError.error != QJsonParseError::NoError)
    {
        QMessageBox::critical(this,
                              tr("Error"),
                              QString(tr("Unable to parse keyboard layout: %1")).arg(jsonParseError.errorString()));

        return;
    }

    if (!doc.isObject())
    {
        QMessageBox::critical(this,
                              tr("Error"),
                              tr("Keyboard layout root must be of type Object"));
        return;
    }

    QJsonObject root = doc.object();

    if (!root.contains("rows") || !root.value("rows").isArray())
    {
        QMessageBox::critical(this,
                              tr("Error"),
                              tr("Keyboard layout must contain 'rows' of type Array"));
        return;
    }

/*    qDeleteAll(buttons);
    buttons.clear();*/

    while (gridLayout->count() > 0)
    {
        QLayoutItem* item = gridLayout->takeAt(0);
        delete item->widget();
        delete item;
    }

    int rowIdx = 0;

    int defaultRowSpan = root.value("rowSpan").toInt(1);
    int defaultColSpan = root.value("colSpan").toInt(1);
    bool defaultShowShift = root.value("showShift").toBool();

    foreach (QJsonValue rowValue, root.value("rows").toArray())
    {
        if (!rowValue.isObject())
        {
            QMessageBox::critical(this,
                                  tr("Error"),
                                  QString(tr("rows[%1] must be of type Object")).arg(rowIdx));
            return;
        }

        QJsonObject rowObject = rowValue.toObject();

        if (!rowObject.value("items").isArray())
        {
            QMessageBox::critical(this,
                                  tr("Error"),
                                  QString(tr("rows[%1] must contain 'items' of type Array")).arg(rowIdx));
            return;
        }

        QJsonArray itemsArray = rowObject.value("items").toArray();

        int defaultRowSpanAtRow = rowObject.value("rowSpan").toInt(defaultRowSpan);
        int defaultColSpanAtRow = rowObject.value("colSpan").toInt(defaultColSpan);
        bool defaultShowShiftAtRow = rowObject.value("showShift").toBool(defaultShowShift);

        int itemIdx = 0;
        int colIdx = 0;

        foreach (QJsonValue itemValue, itemsArray)
        {
            if (!itemValue.isObject())
            {
                QMessageBox::critical(this,
                                      tr("Error"),
                                      QString(tr("rows[%1].items[%2] must be of type Object")).arg(rowIdx).arg(itemIdx));
                return;
            }

            QJsonObject itemObject = itemValue.toObject();

            OnScreenButton* button = NULL;


            if (itemObject.value("role").toString("char") == QLatin1String("char"))
            {
                QChar main;
                QChar alternative;

                if (itemObject.value("main").isDouble())
                    main = QChar(itemObject.value("main").toInt());
                else if (itemObject.value("main").isString())
                    main = QChar(itemObject.value("main").toString().at(0));

                if (itemObject.value("shift").isDouble())
                    alternative = QChar(itemObject.value("shift").toInt());
                else if (itemObject.value("shift").isString())
                    alternative = QChar(itemObject.value("shift").toString().at(0));

                button = new OnScreenButton(main, alternative);
            }
            else if (itemObject.value("role").toString() == QLatin1String("loadlayout"))
            {
                button = new OnScreenButton(itemObject.value("layout").toString(), itemObject.value("title").toString());
            }
            else
            {
                button = new OnScreenButton(rolesMap.value(itemObject.value("role").toString(), OnScreenButton::Undefined));
            }

            if (button)
            {
                int rowSpan = itemObject.value("rowSpan").toInt(defaultRowSpanAtRow);
                int colSpan = itemObject.value("colSpan").toInt(defaultColSpanAtRow);
                bool showShift = itemObject.value("showShift").toBool(defaultShowShiftAtRow);

                button->setTitle(itemObject.value("title").toString());
                button->setShowShift(showShift);

//                if (itemObject.value("title").isString())
//                    button->setTitle(itemObject.value("title").toString());


                gridLayout->addWidget(button, rowIdx, itemObject.value("col").toInt(colIdx), rowSpan, colSpan);

                colIdx += colSpan;
            }

            itemIdx++;
        }

        rowIdx++;
    }
}
