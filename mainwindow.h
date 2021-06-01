#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QResizeEvent>
#include <QMediaPlayer>
#include <QRandomGenerator>
#include <QJsonArray>
#include <QCloseEvent>

class GameScene;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    enum GameStatus
    {
      SIMPLE_SHOOT_IDLE,
      SIMPLE_SHOOT_PAUSE,
      SIMPLE_SHOOT_SHOOTING
    };

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
        void playClip(int clipId=0);
        void sceneReady();
        void shootingDone();
private slots:
void ontimeout();
private:
    Ui::MainWindow *ui;
    QMediaPlayer * player=nullptr;
    GameScene * gameScene=nullptr;
    QTimer * timer;
     QRandomGenerator randomGenerator;
    int timerCounter=0;
    int pauseCount=0;
    int gameStatus=0;
    int shootCount=0;
    double medianResoult;
    int bestResoult=200;
    QString currentUser;

    QJsonArray currentSettings;
    void loadSettings(QByteArray jsonContent);
    QByteArray getSettings();
    void getSettingsForUser(QString user);
    QStringList getUsersList();

protected:
 void keyPressEvent(QKeyEvent *event);
 void resizeEvent(QResizeEvent *event);
 void closeEvent(QCloseEvent *event);

};
#endif // MAINWINDOW_H
