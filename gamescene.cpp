#include <QGraphicsPixmapItem>

#include "gamescene.h"

GameScene::GameScene(QObject *parent) : QGraphicsScene(parent)
{
mainWindow = static_cast<MainWindow *>(parent);
thinkPixmap.load(":/images/thing.svg");
 thinkPixmap = thinkPixmap.transformed(QTransform().scale(0.1,0.1));
 itemWidth = thinkPixmap.width()/2.0;
 thinkOrigin = QPointF(itemWidth,itemWidth);
 timer = new QTimer();

 connect(timer,&QTimer::timeout,this,&GameScene::ontimeout);

 rottimer = new QTimer();
 connect(rottimer,&QTimer::timeout,this,&GameScene::onRotTimeout);

}


void GameScene::showText(QString text)
{
    if(text.isEmpty())
    {
        if(textItem!=nullptr)
        {
          removeItem(textItem);
          delete textItem;
        }
    }
    else
    {
        if(textItem==nullptr)
         {
          textItem = new QGraphicsTextItem(text);
        textItem->setPos(10,10);
        addItem(textItem);
        textItem->show();
    }else
        {
            textItem->setPlainText(text);
        }
    }

}

void GameScene::showItems()
{

 QRectF srect =   sceneRect();
  if(thinkItems.count()>0)
  {
      while (thinkItems.count()>0) {
          removeItem(thinkItems.last());
          QGraphicsPixmapItem * thinkItem = thinkItems.takeLast();
          delete thinkItem;

      }

  }
//angles.clear();
shiftVectors.clear();
rotationCounts.clear();
starts.clear();
ends.clear();
//currentAngle  =  arcDegree;
QPointF center(0,0);

while (shiftVectors.count()<itemCount) {
   // double x = 750;
  //  double y = 196;
  double x =  randomGenerator.bounded( width())+sceneRect().left();// -width()/2.0 ;
  double y = randomGenerator.bounded( height() )+sceneRect().top();// -height()/2.0 ;
  QPointF endpoint(x,y);
starts.append(endpoint);
  center +=endpoint;
shiftVectors.append(endpoint);
rotationCounts.append(randomGenerator.bounded(10));
}
center = center/itemCount;

for (int itemIndex=0;itemIndex<shiftVectors.count() ;itemIndex++ ) {
      QGraphicsPixmapItem * thinkItem = new QGraphicsPixmapItem(thinkPixmap);
      thinkItem->setTransformOriginPoint(thinkOrigin);

    QPointF way = (shiftVectors[itemIndex]-center)/fragmentsCount;
    if(way.x()==0 && way.y()==0)
   {
        thinkItem->setPos(shiftVectors[itemIndex]-thinkOrigin);
        shiftVectors[itemIndex]=way;
    }else
    {
      QPointF outCoord = findCross(shiftVectors[itemIndex],QRectF(QPointF(sceneRect().topLeft()-thinkOrigin*2  ),QSizeF(sceneRect().size()+QSizeF(itemWidth*4,itemWidth*4))),way);
    ends.append(outCoord);
      way = (shiftVectors[itemIndex]-outCoord)/fragmentsCount;

      if(sceneRect().contains( outCoord +  way*fragmentsCount))
        {
           shiftVectors[itemIndex]=way;
           thinkItem->setPos(outCoord-thinkOrigin);
        }else {
 thinkItem->setPos(shiftVectors[itemIndex]-thinkOrigin);
 shiftVectors[itemIndex] = QPointF(0,0);
}
    }
    thinkItems.append(thinkItem);
    addItem(thinkItem);
    thinkItem->show();
}

 // ontimeout();

update();
timerCounter = 0;
timer->start(20);
rottimer->start(100);


}

QPointF GameScene::rotatePoint(QPointF point,double angle,bool forward)
{
    if(forward)
     {
    double x = point.x()*cos(angle) - point.y()*sin(angle);
    double y = point.x()*sin(angle) + point.y()*cos(angle);
    return QPointF(x,y);
    }
    else
    {
        double x = point.x()*cos(angle) + point.y()*sin(angle);
        double y = -point.x()*sin(angle) + point.y()*cos(angle);
        return QPointF(x,y);

    }


    }

QPointF GameScene::findCross(QPointF point,QRectF rect,QPointF way)
{
    double x,y;

 if(way.x()==0)
 {
     x=point.x();
     if(way.y()>0)
         y=rect.top();
     else
         y=rect.bottom();
 }else if(way.y()==0)
 {
  y=point.y();
     if(way.x()>0)
         x=rect.right();
     else
         x=rect.left();
 }else if(way.x()>0)

  {
     double testy = (rect.right() - point.x())*way.y();
     if( testy < rect.top()*way.x()   )
     {
        y= rect.top();
        x = (point.y()-rect.top())*way.x()/way.y();
     }else if(testy > rect.bottom()*way.x() )
     {
         y=rect.bottom();
       x =  (rect.bottom()-point.y())*way.x()/way.y();
     }else
     {
        x=rect.right();
        y=testy/way.x();
     }

 }else
 {
     double testy = ( point.x()-rect.left())*way.y();
     if( testy < rect.top()*way.x()   )
     {
        y= rect.top();
        x = (point.y()-rect.top())*way.x()/way.y();
     }else if(testy > rect.bottom()*way.x() )
     {
         y=rect.bottom();
       x =  (rect.bottom()-point.y())*way.x()/way.y();
     }else
     {
        x=rect.left();
        y=testy/way.x();
     }

 }







   //if(angle < atan2((point.y()-rect.top())/(rect.right()-point)) )
    return QPointF(x,y);
}

void GameScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::LeftButton )
       {
       QRectF srect =   sceneRect();
        QPointF pos = event->scenePos();
          for (int itemIndex=0;itemIndex <thinkItems.count() ;itemIndex++ ) {
       // QPointF itpos = thinkItems[0]->scenePos();
        QRectF itRect = thinkItems[itemIndex]->sceneBoundingRect();
        double width = itRect.width();
        QPointF itemCenter = itRect.topLeft()+QPointF(width/2,width/2);
        QPointF  distance = itemCenter - pos;
        if(distance.manhattanLength()<width*precisity)
        {
          QGraphicsPixmapItem * item = thinkItems.takeAt(itemIndex);
            removeItem(item);
          if(thinkItems.count()==0)
          {
             mainWindow->shootingDone();
             rottimer->stop();
          }
          //  thinkItems.removeLast();
         //  update();
        }
          }
       // itRect.setSize(itRect.size()*precisity);
      //  itRect.moveTo()
        mainWindow->playClip();
    }
    /*
   if(event->button() == Qt::RightButton )
       item->setPos(-100,-100);
*/
}



void GameScene::ontimeout()
{

    for (int itemIndex=0;itemIndex<thinkItems.count();itemIndex++ ) {
          if(!shiftVectors[itemIndex].isNull() )
       {
         QPointF pos =  thinkItems[itemIndex]->pos();
            pos += shiftVectors[itemIndex];

              thinkItems[itemIndex]->setPos(pos);
          }
    }



update();
timerCounter++;
if(timerCounter>=fragmentsCount)
{
    timer->stop();
mainWindow->sceneReady();
}

}

void GameScene::onRotTimeout()
{
    for (int itemIndex=0;itemIndex<thinkItems.count() ;itemIndex++ ) {

        if(rotationCounts[itemIndex]==0)
        {
            double angle =  thinkItems[itemIndex]->rotation();
            angle += rotAngle;
            if(angle>=360)
                angle -= 360;
           thinkItems[itemIndex]->setRotation(angle);
           rotationCounts[itemIndex] = randomGenerator.bounded(10);
        }else
          rotationCounts[itemIndex]--;
    }
}
