#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->lineEdit_Port->setText("9096");
    ui->pushButton_Send->setEnabled(false);
    //server = new QTcpServer();
    server = new MyQTcpServer(this);

    //QObject::connect(server, &QTcpServer::newConnection, this, &MainWindow::server_New_Connect);
}
MainWindow::~MainWindow()
{
    server->close();
    server->deleteLater();
    delete ui;
}


void MainWindow::on_pushButton_listen_clicked()
{
    if(ui->pushButton_listen->text() == tr("开始监听"))
    {
        //从输入框获取端口号
        int port = ui->lineEdit_Port->text().toInt();
        //监听指定端口
        if(!server->listen(QHostAddress::Any, port))
        {
            ui->textBrowser_Recv->setText(server->errorString());
            return;
        }
        ui->pushButton_listen->setText("取消");
        ui->textBrowser_Recv->setText("listen successfully...");
    }
    else
    {
        /*if(socket->state() == QAbstractSocket::ConnectedState)
        {
            socket->disconnect();
        }*/
        server->close();
        ui->pushButton_listen->setText("开始监听");
        ui->pushButton_Send->setEnabled(false);
    }
}

void MainWindow::connectShow()
{
    char cnt[64]={0};
    connect_cnt++;
    sprintf(cnt, "目前已连接客户端数量：%d", connect_cnt);
    ui->textBrowser_Recv->append(cnt);
}

void MainWindow::Disconnected()
{
    char cnt[64]={0};
    ui->textBrowser_Recv->append("一个客户端断开连接。");
    connect_cnt--;
    sprintf(cnt, "目前已连接客户端数量：%d", connect_cnt);
    ui->textBrowser_Recv->append(cnt);
}

void MainWindow::write_log(QString str)
{
    ui->textBrowser_Recv->append(str);
}

void MainWindow::receiveFromServer(QTcpSocket *fd, QString *msg)
{
    qDebug()<<"main send.";
    qDebug()<<"main: "<<QThread::currentThreadId();
    //fd->write(msg->toLatin1());
}
