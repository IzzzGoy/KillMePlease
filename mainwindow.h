#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include"client.h"
#include"server.h"
#include<QPixmap>
#include<QGraphicsScene>
#include<QTimer>
#include<QString>
#include <pthread.h>
#include <QKeyEvent>
#include <unistd.h>
#include<table.h>

struct serverInfo
{
    Server* server;
    bool* state;
    serverInfo(Server* server,bool* state)
    {
        this->server = server;
        this->state = state;
    }
};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    QGraphicsScene* scene;
    ~MainWindow();

private slots:
    void on_exitButton_clicked();

    void on_newGameButton_clicked();

    void on_connectMenuButton_clicked();

    void on_connectToGameButton_clicked();

    void showpick();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_toGameButton_clicked();

protected:
    void keyPressEvent(QKeyEvent *event);
private:
    Table table;
    int numbOfPlayers;
    pthread_t tmp;
    QTimer* timer;
    Client client;
    Server server;
    bool state = true;
    static void* severServis(void* arg);

    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
