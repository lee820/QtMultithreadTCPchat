#ifndef MYQTHREAD_H
#define MYQTHREAD_H

#include <QThread>
#include <QTcpSocket>
#include <QtNetwork>
#include <QDateTime>

class MyQTcpServer;

class MyQThread : public QThread
{
    Q_OBJECT
public:
    MyQThread(QObject *parent = nullptr);
    ~MyQThread();
    void writeDescriptor(const QString &strSocketID);
    void run();

public:
    QTcpSocket *tcpSocket;
public:
    MyQTcpServer *myqtcpserver;
    int qisocketID;
    QString qstrsocketID;

signals:
    void sendToServer(QTcpSocket *fd, QString *msg);

public slots:
    void sendData();
    void quit_thread();

};

#endif // MYQTHREAD_H
