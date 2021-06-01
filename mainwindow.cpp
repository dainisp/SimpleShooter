#include <iostream>
#include "gamescene.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
setMouseTracking(true);

   //   ui->field->setViewportUpdateMode(QGraphicsView::NoViewportUpdate);
   QPixmap cursorPixmap(":/images/shoot.svg");
cursorPixmap = cursorPixmap.transformed(QTransform().scale(0.1,0.1));
    ui->field->setCursor(QCursor(cursorPixmap));
    gameScene = new GameScene(this);
    ui->field->setScene(gameScene);
    gameScene->update();
timer = new QTimer();
connect(timer,&QTimer::timeout,this,&MainWindow::ontimeout);
timerCounter=0;
pauseCount= 30 ;
gameStatus = SIMPLE_SHOOT_PAUSE;
timer->start(100);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::playClip(int clipId)
{
QStringList clipNames;

clipNames << "qrc:/audio/shoot.mp3"  << "qrc:/audio/showing.mp3"  ;

if(clipId <0 || clipId >= clipNames.count()  )
    return;

if(player == nullptr)
   player = new QMediaPlayer;
   // ...
   player->setMedia(QUrl(clipNames[clipId]));
   player->setVolume(50);
   player->play();



}

void MainWindow::sceneReady()
{
  gameStatus = SIMPLE_SHOOT_SHOOTING;
timerCounter = 0;
pauseCount = 100;
  timer->start(100);
}

void MainWindow::shootingDone()
{
if(timerCounter < bestResoult  )
    bestResoult = timerCounter;
medianResoult = (medianResoult*(double)shootCount + (double)timerCounter)/(double)(shootCount+1);
shootCount++;
 gameScene->showText(QString("Best:%1\nMedian:%2\nCurent:%3").arg(bestResoult).arg(medianResoult).arg(timerCounter));
    timerCounter=0;
    pauseCount= 10 +  randomGenerator.bounded(20) ;
    gameStatus = SIMPLE_SHOOT_PAUSE;
    timer->start(100);
}


void MainWindow::keyPressEvent(QKeyEvent *event)
{
if(event->key() == Qt::Key_Escape )
    close();
if(event->key() == Qt::Key_Space)
     gameScene->showItems();
if(event->key() == Qt::Key_F)
{
  if(isFullScreen())
     showNormal();
  else
      showFullScreen();


}

}

 void MainWindow::resizeEvent(QResizeEvent *event)
 {
     gameScene->setSceneRect(QRect(0,0, event->size().width()-20,event->size().height()-20));
     QMainWindow::resizeEvent(event);
 }

 void MainWindow::ontimeout()
 {
 timerCounter++;
 if(timerCounter>=pauseCount)
 {
   timer->stop();
  if(gameStatus == SIMPLE_SHOOT_PAUSE )
   {
   playClip(1);
      gameScene->showItems();
  }
   else
  {

    gameScene->showItems();
  }
 }
 }
