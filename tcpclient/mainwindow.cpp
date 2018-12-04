#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    socket = new QTcpSocket();

    QObject::connect(socket, &QTcpSocket::readyRead, this, &MainWindow::socket_Read_Data);
    QObject::connect(socket, &QTcpSocket::disconnected, this, &MainWindow::socket_Disconnected);
    //QObject::connect(socket, &QTcpSocket::disconnect, this, &MainWindow::socket_Disconnected);
    ui->pushButton_Send->setEnabled(false);
    ui->lineEdit_IP->setText("127.0.0.1");
    ui->lineEdit_Port->setText("9096");
    ui->lineEdit_UserName->setText("请输入用户名...");
}

MainWindow::~MainWindow()
{
    socket->close();
    delete ui;
}

void MainWindow::on_pushButton_Connect_clicked()
{

    //连接到服务器
    if(ui->pushButton_Connect->text() == tr("连接"))
    {
        ui->textBrowser_Recv->append("start connect to server...");
        QString ip;
        int port;

        ip = ui->lineEdit_IP->text();
        port = ui->lineEdit_Port->text().toInt();

        //取消之前的连接
        socket->abort();
        //开始连接到服务器
        socket->connectToHost(ip, port);
        if(!socket->waitForConnected(30000))
        {
            //ui->textBrowser_Recv->append("connect timeout.");
            MyDEBUG("connect timeout."); //连接超时
            return;
        }
        MyDEBUG("connect successfully.");
        ui->pushButton_Send->setEnabled(true);
        ui->pushButton_Connect->setText("断开连接");
        ui->textBrowser_Recv->append("connect success.");

    }

    else
    {
        socket->disconnectFromHost();
        ui->pushButton_Connect->setText("连接");
        ui->pushButton_Send->setEnabled(false);
    }
}

void MainWindow::on_pushButton_Send_clicked()
{
    //服务器接收数据格式（username#cmd#sendtoname）
    QString req;
    req.append(ui->lineEdit_UserName->text());
    req.append("#");
    req.append(ui->textEdit_Send->toPlainText());
    req.append("#");
    req.append(ui->lineEdit_Sendname->text());
    qDebug()<<req;
    socket->write(req.toLatin1());
    socket->flush();
}

void MainWindow::socket_Read_Data()
{
    QByteArray recvbuf;
    QString recv;
    recvbuf = socket->readAll();
    if(!recvbuf.isEmpty())
    {
        QString recv;
        recv.prepend(recvbuf);
        MyDEBUG("从服务器接受数据如下：");
        ui->textBrowser_Recv->append(recv);
    }
}

void MainWindow::socket_Disconnected()
{
    ui->pushButton_Send->setEnabled(false);
    ui->pushButton_Connect->setText("连接");
    MyDEBUG("断开连接");
}
