#include "application.h"

#include <QTextCodec>

#include "onscreenkeyboard.h"

int main(int argc, char *argv[])
{
    Application a(argc, argv);

    return a.exec();
}
