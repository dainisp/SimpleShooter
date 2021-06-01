#ifndef GAMESCENE_H
#define GAMESCENE_H
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>
#include <QRandomGenerator>
#include <QTimer>
#include <cmath>
#include "mainwindow.h"

class GameScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit GameScene(QObject *parent = nullptr);
void showItems();
void showText(QString text=QString());

private slots:
void ontimeout();
void onRotTimeout();
private:
QTimer * timer;
QTimer * rottimer;
QList<int> rotationCounts;
MainWindow * mainWindow=nullptr;
 QList< QGraphicsPixmapItem *> thinkItems;
 QGraphicsTextItem * textItem=nullptr;
 //QList<double> radiuses;
 QList<QPointF> shiftVectors;
  QList<QPointF>  starts;
  QList<QPointF>  ends;
// QList<double> randomAngles;
// QList<QPointF> shiftPoints;
QPointF thinkOrigin;
// double arcDegree = M_PI_2/3;
 int fragmentsCount = 10;
 int timerCounter = 0;
 double precisity=0.3;
 double itemWidth=0;
 double rotAngle = 30.0;
// double currentAngle=0;
  int itemCount=5;
 QRandomGenerator randomGenerator;
QPixmap thinkPixmap;
QPointF findCross(QPointF point, QRectF rect, QPointF way);
QPointF rotatePoint(QPointF point, double angle, bool forward=false);

protected:
void mousePressEvent(QGraphicsSceneMouseEvent *event);

};

#endif // GAMESCENE_H
