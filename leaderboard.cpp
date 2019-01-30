#include "leaderboard.h"

leaderboard::leaderboard(QLabel *parent,QString PATH):QLabel(parent),PATH(PATH)
{
    resize(1920,1080);
    showFullScreen();
    setPixmap(QPixmap(PATH+"scoreboard.png"));

    converter = new QProcess(this);


    int w =600;
    int h = 60;
    int x0 = 388;
    int y0 = 360;

    int x1 = 1100;


    int dy = 10;

    for(int i =0;i<10;i++)
    {
        QLabel *n = new QLabel(this);

        nameLbls.push_back(n);
        n->resize(w,h);
        n->move(x0,y0+i*(h+dy));
        //n->setStyleSheet("border: 1px solid black");
        n->show();

        QLabel *s = new QLabel(this);
        scoreLbls.push_back(s);
        s->resize(w,h);
        s->move(x1,y0+i*(h+dy));
        //s->setStyleSheet("border: 1px solid black");
        s->show();


    }



    updTimer = new QTimer(this);
    connect(updTimer,SIGNAL(timeout()),this,SLOT(reloadScores()));
    updTimer->start(10*1000);
    reloadScores();
}



void leaderboard::reloadScores()
{
    convertScores();

    QFile file(PATH+"scores.csv");
    if(!file.open(QIODevice::ReadOnly)) {
        qDebug()<<"no csv file";

    }
    else
    {

        QTextStream in(&file);

        QString  line;
        QStringList params;

        QString teamName;
        int mins,secs,total;
        bool test1,test2;
        score nuScore;
        scores.clear();

        in.readLine();//disregard first line
        while(!in.atEnd()) {
            line = in.readLine();
            params = line.split(",");
            if(params.size()>=3)
            {
                teamName = params[0];
                teamName.remove("\"");
                mins = params[1].toInt(&test1);
                secs = params[2].toInt(&test2);
                total = mins*60+secs;



                if(test1&&test2&&(mins>=0)&&(secs>=0)&&(teamName!=""))
                {
                    nuScore.name = teamName;
                    nuScore.mins = mins;
                    nuScore.secs = secs;
                    nuScore.total = total;
                    scores.push_back(nuScore);

                }
            }
        }
        file.close();
    }

    findBestScores();

}

void leaderboard::convertScores()
{
    //converter->start((QString)"ssconvert "+PATH+"scores.xlsx "+PATH+"scores.csv");
    QString cmd = (QString)"wget \"https://docs.google.com/spreadsheets/d/1YBktyEQyFQfDaCYwQj_CTzeHGUtb2XXzYDJP56xb028/export?format=csv\" -O "+PATH+"scores.csv";
    // qDebug()<<cmd;
    converter->start(cmd);
    converter->waitForFinished(5000);
}




void leaderboard::findBestScores()
{

    if(scores.size()<1)
        return;


    std::vector<int> scoreOrder;

    scoreOrder.push_back(0);

    if(scores.size()>=2)

    {

        bool test;
        for(int i = 1;i<scores.size();i++)
        {
            test = false;
            for(int j = 0;j<scoreOrder.size();j++)
            {

                if(scores[i].total<scores[scoreOrder[j]].total)
                {
                    scoreOrder.insert(scoreOrder.begin()+j,i);
                    test = true;
                    break;
                }



            }
            if(!test)
                scoreOrder.push_back(i);


        }

    }
    //qDebug()<<scoreOrder;

    int N = qMin(10,(int)scoreOrder.size());


    for (auto l:nameLbls)
        l->hide();
    for (auto l:scoreLbls)
        l->hide();

    for(int n = 0;n<N;n++)
    {
        int k = scoreOrder[n];
        qDebug()<<scores[k].name<<scores[k].mins<<scores[k].secs<<scores[k].total;
        writeToLabel(nameLbls[n],scores[k].name);
        writeToLabel(scoreLbls[n],QString::number(scores[k].mins)+"\""+QString("%1").arg(scores[k].secs, 2, 10, QChar('0')));


    }


}


void leaderboard::writeToLabel(QLabel *l,QString txt)
{
    QPixmap myPix( l->size() );

    myPix.fill(Qt::transparent);

    QPainter painter(&myPix);
    painter.setRenderHint(QPainter::Antialiasing, true);


    painter.setPen(QPen(Qt::white, 0));


    if(txt!="")
    {

        QFont font = QFont(PATH+"font.ttf",40) ;


        QFontMetrics fm(font);

        double W = fm.width(txt);
        double H = fm.height();

        if(W > (double)0.9*l->width())
        {
            float a = (double) W/((double)0.9*l->width());

            font.setPointSizeF((double)font.pointSize()/a);

            QFontMetrics fm2(font);
            W = fm2.width(txt);
            H = fm2.height();
        }


        painter.setFont(font);

        painter.setPen(QPen(Qt::white, 2));
        painter.drawText(QRectF((l->width()-W)/2,(l->height()-H)/2,l->width(),l->height()),txt);

    }


    l->setPixmap(myPix);
    l->show();

}
