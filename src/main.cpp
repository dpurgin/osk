#include "application.h"

//#include <QDesktopWidget>

#include "onscreenkeyboard.h"

int main(int argc, char *argv[])
{
    Application a(argc, argv);

//    a.osk()->move(0, QDesktopWidget().screenGeometry().height() - 330);
//    a.osk()->move(0, 1550);
//    a.osk()->setFixedSize(1080, 300);

    return a.exec();
}
