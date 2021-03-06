#include <iostream>
#include <QDir>
#include <QMessageBox>
#include <QJsonObject>
#include <QJsonDocument>
#include "gamescene.h"
#include "userschoose.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

  QString settingsPath = QDir::current().path() +QString("/simpleshoot.json");
  QFile infile(settingsPath);
  if(infile.open(QIODevice::ReadOnly))
    {
     QByteArray content = infile.readAll();
     infile.close();
    loadSettings(content);
 }

  UsersChoose usersChoose(getUsersList(),this);
    usersChoose.setModal(true);
    usersChoose.exec();
QString user = usersChoose.getChoosedUser();
currentUser = user;
if(!user.isEmpty())
    getSettingsForUser(user);

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


void MainWindow::getSettingsForUser(QString user)
{
    int userIndex=0;
    while (userIndex<currentSettings.count()) {
        QJsonObject userData = currentSettings[userIndex].toObject();
        if(userData["user"].toString() ==  currentUser)
            break;

        userIndex++;
    }
  if(userIndex<currentSettings.count())
  {
        QJsonObject userData = currentSettings[userIndex].toObject();
       bestResoult= userData["record"].toInt();
        medianResoult =  userData["median"].toInt() ;
       shootCount = 10;
  }
}

  void MainWindow::loadSettings(QByteArray jsonContent)
  {
  QJsonDocument doc = QJsonDocument::fromJson(jsonContent);
  if(!doc.isArray() || doc.array().count() ==0    )
      return;
currentSettings = doc.array();

  }
      QByteArray MainWindow::getSettings()
      {
          if(!currentUser.isEmpty())
          {
             int userIndex=0;
             while (userIndex<currentSettings.count()) {
                 QJsonObject userData = currentSettings[userIndex].toObject();
                 if(userData["user"].toString() ==  currentUser)
                     break;
                 userIndex++;
             }
           if(userIndex<currentSettings.count())
           {
                 QJsonObject userData = currentSettings[userIndex].toObject();
                userData["record"] = bestResoult;
                userData["median"] = medianResoult;
                currentSettings[userIndex]=userData;

           }else
             {
                 QJsonObject userData;
                 userData.insert("user",currentUser);
                 userData.insert("record",bestResoult);
                 userData.insert("median",medianResoult);
              currentSettings.append(userData);
           }

          }

          if(currentSettings.count()>0)
          {
           QJsonDocument doc(currentSettings);
           return doc.toJson();
          }
          else return QByteArray();
      }

      QStringList MainWindow::getUsersList()
      {
            QStringList usersList;
      if(currentSettings.count()>0)

            {

          foreach (QJsonValue userJsonValue, currentSettings) {
              usersList.append(userJsonValue.toObject()["user"].toString());
          }

      }

       return usersList;

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
   playClip(1);
    gameScene->showItems();
  }
 }
 }

  void MainWindow::closeEvent(QCloseEvent *event)
  {
       QByteArray content = getSettings();
       if(!content.isEmpty())
     {
      QString settingsPath = QDir::current().path() +QString("/simpleshoot.json");
      QFile outfile(settingsPath);
      if(outfile.open(QIODevice::WriteOnly))
        {
        outfile.write(content);
         outfile.close();

     }
       }
       QMainWindow::closeEvent(event);
  }
