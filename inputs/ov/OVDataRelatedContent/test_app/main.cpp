/*
* Author: Arnaud Dupuis (a.dupuis@infinityperl.org)
* Copyright: Orange Vallee
*/

#include "OVDataRelatedContent.h"
#include <QApplication>
#include <QStringList>
#include "RCTest.h"

int main(int argc, char **argv){
        QApplication app(argc,argv);
//	OVDataRelatedContent *rc = new OVDataRelatedContent( QStringList() << "../plugins/build/" );
        RCTest *ui = new RCTest;
        ui->show();
//        rc->loadPlugins();
//        rc->getRelatedContent( QStringList() << "Assuma" << "3.1" );
        app.exec();
}
