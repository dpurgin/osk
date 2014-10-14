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
