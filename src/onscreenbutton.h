#ifndef ONSCREENBUTTON_H
#define ONSCREENBUTTON_H

#include <QLabel>

class OnScreenButton : public QLabel
{
    Q_OBJECT

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
    OnScreenButton(Role role, QString title = QString(), QWidget* parent = 0)
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

    QString title() const { return mTitle; }
    void setTitle(const QString& title) { mTitle = title; }

    bool showShift() const { return mShowShift; }
    void setShowShift(bool val) { mShowShift = val; }

signals:

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
