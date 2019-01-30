#include <QApplication>
#include "qwindow.h"
#include "qscreen.h"
#include "qwidget.h"
#include "leaderboard.h"
#include "qdebug.h"
#include "QFile"



#define PATH_DEFAULT (QString)"/home/fred/Dropbox/Taf/Cassiopee/leaderboardJeddah/files/"




int main(int argc, char *argv[])

{
    QApplication a(argc, argv);




    QString PATH;
    QStringList params = a.arguments();
    if(params.size()>1)
        PATH = params[1];
    else
        PATH=PATH_DEFAULT;

    bool HIDE_CURSOR=false;


    QFile file(PATH+"config.cfg");
    if(!file.open(QIODevice::ReadOnly)) {
        qDebug()<<"no config file";

    }
    else
    {

        QTextStream in(&file);

        QString  line;
        QString paramName,paramValue;
        QStringList params;

        while(!in.atEnd()) {
            line = in.readLine();
            line = (line.split("#"))[0];
            params = line.split("=");
            if(params.size()>=2)
            {
                paramName = params[0];
                paramValue = params[1];

               if (paramName.mid(0,6)=="CURSOR")
                    HIDE_CURSOR = (paramValue=="NO");

                else
                    qDebug()<<paramName<<" - "<<paramValue;
            }
        }
        file.close();

    }




    if (HIDE_CURSOR)
    {
        QCursor cursor(Qt::BlankCursor);
        a.setOverrideCursor(cursor);
        a.changeOverrideCursor(cursor);
    }


   leaderboard *lb = new leaderboard(NULL,PATH);


   lb->setGeometry(a.screens()[0]->geometry().x(),a.screens()[0]->geometry().y(),1920,1080);
        lb->show();





    return a.exec();

}
