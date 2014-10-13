#ifndef APPLICATION_H
#define APPLICATION_H

#include <QApplication>

#define app (reinterpret_cast< Application* >(qApp))

class OnScreenKeyboard;

class Application : public QApplication
{
    Q_OBJECT

public:
    explicit Application(int& argc, char* argv[]);
    virtual ~Application();

    OnScreenKeyboard* osk() const { return mOsk; }

signals:

public slots:

private:
    OnScreenKeyboard* mOsk;
};

#endif // APPLICATION_H
