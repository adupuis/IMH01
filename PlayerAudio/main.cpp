#include <QtCore/QCoreApplication>
#include "Testapp.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    TestApp *testApp = new TestApp();
    testApp->load();
    testApp->play();
    return a.exec();
}
