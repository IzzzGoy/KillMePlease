#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    numbOfPlayers = 1;
    ui->playersCountLable->setText(QString("%1").arg(numbOfPlayers));
    scene = new QGraphicsScene;
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->stackedWidget->setCurrentIndex(0);
}

MainWindow::~MainWindow()
{
    delete scene;
    delete timer;
    delete ui;
}

void MainWindow::on_exitButton_clicked()
{
    exit(0);
}

void MainWindow::on_newGameButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::on_connectMenuButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_connectToGameButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
    QByteArray tmp = ui->lineEdit->text().toLocal8Bit();
    client.start_client(tmp.data());
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),SLOT(showpick()));
    timer->start(33);
    protocolInfo = new ProtocolInfo(&client,&state,&protocolState);
    pthread_create(&protocolThread,0,protocolServis,static_cast<void*>(protocolInfo));
}

void MainWindow::showpick()
{
    if(protocolState)
    {
        if(state)
        {
            sow_results();
            scene->clear();
            table.Drow(client.grid,client.frameX,client.frameY,scene,client.id);
        }
        else
        {
            ui->gameOverLable->setText(QString::number(client.id));
            pthread_join(protocolThread,NULL);
            sleep(5);
            client.close_client();
            exit(0);
        }
    }
}
void MainWindow::on_pushButton_2_clicked()
{
    if(numbOfPlayers < 4)
    {
        numbOfPlayers++;

        ui->playersCountLable->setText(QString("%1").arg(numbOfPlayers));
    }
}

void MainWindow::on_pushButton_3_clicked()
{
    if(numbOfPlayers > 1)
    {
        numbOfPlayers--;

        ui->playersCountLable->setText(QString("%1").arg(numbOfPlayers));
    }
}

void MainWindow::on_toGameButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
    server.serverInit(numbOfPlayers);
    serverInfo* info = new serverInfo(&server,&state);
    pthread_create(&tmp,0,severServis,static_cast<void*>(info));
    client.start_client("127.0.0.1");
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),SLOT(showpick()));
    timer->start(33);
    protocolInfo = new ProtocolInfo(&client,&state,&protocolState);
    pthread_create(&protocolThread,0,protocolServis,static_cast<void*>(protocolInfo));
}

void* MainWindow::severServis(void* arg)
{
    serverInfo* info = static_cast<serverInfo*>(arg);
    info->server->doServer();
    *info->state = false;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    char key = event->key();
    if(key == 'W' || key == 'w')
    {
        client.set_direction('w');
    }
    else if (key == 'S' || key == 's')
    {
        client.set_direction('s');
    }
    else if (key == 'A' || key == 'a')
    {
        client.set_direction('a');
    }
    else if (key == 'D' || key == 'd')
    {
        client.set_direction('d');
    }

}

void MainWindow::sow_results()
{
    if(client.id == 0)
    {
        ui->firstPlayerNameLable->setText(QString("You"));
    }
    else
    {
        ui->firstPlayerNameLable->setText(QString("Player 1"));
    }
    ui->firstPlayerResultsLable->setText(QString("%1").arg(client.scores.frameScores[0]));
    if(client.id == 1)
    {
        ui->secondPlayerNameLable->setText(QString("You"));
    }
    else
    {
        ui->secondPlayerNameLable->setText(QString("Player 2"));
    }
    ui->secondPlayerResultsLable->setText(QString("%1").arg(client.scores.frameScores[1]));
    if(client.id == 2)
    {
        ui->thirdPlayerNameLable->setText(QString("You"));
    }
    else
    {
        ui->thirdPlayerNameLable->setText(QString("Player 3"));
    }
    ui->thirdPlayerResultsLable->setText(QString("%1").arg(client.scores.frameScores[2]));
    if(client.id == 3)
    {
        ui->foursPlayerNameLable->setText(QString("You"));
    }
    else
    {
        ui->foursPlayerNameLable->setText(QString("Player 4"));
    }
    ui->foursPlayerResultsLable->setText(QString("%1").arg(client.scores.frameScores[3]));
}

void *MainWindow::protocolServis(void *arg)
{
    ProtocolInfo* info = static_cast<ProtocolInfo*>(arg);
    while (info->client->protocol())
    {
        *info->protocolState = true;
    }
    *info->state = false;
    pthread_exit(0);
}
