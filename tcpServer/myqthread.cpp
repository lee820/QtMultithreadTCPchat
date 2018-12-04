#include "myqthread.h"
#include "myqtcpserver.h"
#include <QDebug>
#include "common.h"

MyQThread::MyQThread(QObject *parent):QThread(parent)
{
    tcpSocket = new QTcpSocket;
    //myqtcpserver = (MyQTcpServer *)parent;
}

MyQThread::~MyQThread()
{

}

void MyQThread::run()
{
    exec();
}

void MyQThread::sendData()
{
    QByteArray buffer;
    QString cmd;
    QString help = "Command: \n help: For help\n time: Get time\n getUserList: get all user name.";

    //客户端的消息格式（username#cmd#send_to_username)
    buffer = tcpSocket->readAll();
    cmd.prepend(buffer);
    QStringList text_list = cmd.split("#");
    qDebug()<<"host get: "<<text_list[0]<<"and"<<text_list[1];

    //经过上面的处理后text_list[0] = username
    //text_list[1] = cmd
    //text_list[2] = sent_to_username

    Common::g_user_socket.insert(tcpSocket, text_list[0]);

    //如果用户连接了服务器，则把cmd当做消息发给用户
    if(!text_list[2].isEmpty())
    {
        QMap<QTcpSocket *, QString>::iterator it = Common::g_user_socket.begin();
        QTcpSocket *tcp = nullptr;
        while(it != Common::g_user_socket.end())
        {
            if(it.value() == text_list[2])
            {
                tcp = it.key();
                break;
            }
        //emit sendToServer(user_socket[text_list[2]], &text_list[1]);
            ++it;
        }
        if(tcp == nullptr)
        {
            qDebug()<<"no tcpsocket found";
            return;
        }
        //子线程不能跨线程调用socket，socket在构造函数中New,
        //这样socket就属于myqtcpserver。这里调用就不会跨线程。
        QString msg = text_list[0] + ": " + text_list[1];
        tcp->write(msg.toLatin1());

    }
    if(text_list[1] == "help")
    {
        tcpSocket->write(help.toLatin1());
    }
    if(text_list[1] == "time")
    {
        qDebug()<<"get time cmd";
        QDateTime cur_date = QDateTime::currentDateTime();
        QString cur_date_time = cur_date.toString("yyyy.MM.dd hh:mm:ss");
        tcpSocket->write(cur_date_time.toLatin1());
    }
    if(text_list[1] == "getUserList")
    {
        //获取已连接用户列表
        QMap<QTcpSocket *, QString>::iterator it = Common::g_user_socket.begin();
        QString msg;
        while(it != Common::g_user_socket.end())
        {
            msg += it.value();
            msg += "\n";
            ++it;
        }
        qDebug()<<"start send.";
        tcpSocket->write(msg.toLatin1());
    }
}

void MyQThread::quit_thread()
{
    tcpSocket->close();
    quit();
}

void MyQThread::writeDescriptor(const QString &strSocketID)
{
   qstrsocketID = strSocketID;
}
