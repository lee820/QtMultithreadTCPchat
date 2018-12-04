#ifndef MYQTCPSERVER_H
#define MYQTCPSERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include "myqthread.h"

class MainWindow;
extern QMap<QString, QTcpSocket*>user_socket;

class MyQTcpServer : public QTcpServer
{
    Q_OBJECT
public:
    MyQTcpServer(QObject *parent = nullptr);
    ~MyQTcpServer();

protected:
    void incomingConnection(int socketDescriptor);

public slots:
    void receiveFromThread(QTcpSocket* fd, QString *msg);
signals:
    void sendToMain(QTcpSocket *fd, QString *msg);
private:
    MainWindow *mWin;
};

#endif // MYQTCPSERVER_H
