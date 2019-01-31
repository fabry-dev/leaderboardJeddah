#ifndef LEADERBOARD_H
#define LEADERBOARD_H

#include <QObject>
#include <QWidget>
#include "qlabel.h"
#include "qprocess.h"
#include "qfile.h"
#include "qdebug.h"
#include "qmath.h"
#include "qpaintdevice.h"
#include "qpainter.h"
#include "qtimer.h"
#include "QStringList"

struct score {
  QString name;
  int mins;
  int secs;
  int total;
} ;


class leaderboard : public QLabel
{
    Q_OBJECT
public:
    leaderboard(QLabel *parent,QString PATH);
private:
    QString PATH;
    QProcess *converter;
    std::vector<score> scores;
    std::vector<QLabel*>nameLbls,scoreLbls;
    QTimer *updTimer;

    void writeToLabel(QLabel *l, QString txt);
private slots:
    void reloadScores();
    void convertScores();
    void findBestScores();
};

#endif // LEADERBOARD_H
