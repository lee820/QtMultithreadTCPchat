#include "myqtcpserver.h"
#include <QDebug>
#include "mainwindow.h"
#include "myqthread.h"
#include "common.h"

MyQTcpServer::MyQTcpServer(QObject *parent) :
    QTcpServer (parent)
{
    mWin = (MainWindow *)parent;
}

MyQTcpServer::~MyQTcpServer()
{

}

void MyQTcpServer::incomingConnection(int socketDescriptor)
{
    qDebug("incoming start;");
    //只要有客户端连接成功，这里就会被调用
    MyQThread *pthread = new MyQThread(this);
    pthread->writeDescriptor(QString::number(socketDescriptor));
    //这个将线程开始和结束的信号，连接到父类mainWindow，方法在MainWindow实现
    connect(pthread, &MyQThread::started, mWin, &MainWindow::connectShow);
    connect(pthread, &MyQThread::finished, mWin, &MainWindow::Disconnected);
    connect(pthread, &MyQThread::sendToServer, this, &MyQTcpServer::receiveFromThread);
    connect(pthread->tcpSocket, &QTcpSocket::readyRead, pthread, &MyQThread::sendData);
    connect(pthread->tcpSocket, &QTcpSocket::disconnected, pthread, &MyQThread::quit_thread);
    if(!pthread->tcpSocket->setSocketDescriptor(socketDescriptor))
    {
        qDebug()<<"setSocketDescriptor fail.";
        return;
    }
    pthread->start(); //myqthread中的run函数开始执行
}

void MyQTcpServer::receiveFromThread(QTcpSocket *fd, QString *msg)
{
    QTcpSocket *tcp;
    tcp = fd;
    tcp->write(msg->toLatin1());
    qDebug()<<"server: "<<QThread::currentThreadId();
    qDebug()<<"send message: "<<*msg;
    qDebug()<<"server send.";
    //emit sendToMain(fd, msg);
}
