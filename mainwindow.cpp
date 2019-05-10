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
}

MainWindow::~MainWindow()
{
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
    QByteArray tmp = ui->lineEdit->text().toLocal8Bit();
    client.start_client(tmp.data());
    ui->stackedWidget->setCurrentIndex(3);
    ui->scoreCountLable->setText(QString("%1").arg(0));
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
            //        client.protocol();
            table.Drow(client.coordinates.grid,client.coordinates.X,client.coordinates.Y,scene);
            ui->scoreCountLable->setText(QString("%1").arg(client.score));
        }
        else
        {
            pthread_join(tmp,NULL);
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
    ui->scoreCountLable->setText(QString("%1").arg(0));
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

void *MainWindow::protocolServis(void *arg)
{
    ProtocolInfo* info = static_cast<ProtocolInfo*>(arg);
    while (*info->state)
    {
        info->client->protocol();
        *info->protocolState = true;
    }
}
