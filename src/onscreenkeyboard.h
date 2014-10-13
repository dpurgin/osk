#ifndef ONSCREENKEYBOARD_H
#define ONSCREENKEYBOARD_H

#include <QString>
#include <QWidget>

class QGridLayout;
class OnScreenButton;

class OnScreenKeyboard : public QWidget
{
    Q_OBJECT
public:
    explicit OnScreenKeyboard(QWidget* parent = 0);
    virtual ~OnScreenKeyboard();

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
};

#endif // ONSCREENKEYBOARD_H
