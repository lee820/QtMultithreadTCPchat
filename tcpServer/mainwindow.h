#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QTcpServer>
#include "myqtcpserver.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void write_log(QString str);

public slots:
    void receiveFromServer(QTcpSocket *fd, QString *msg);

private slots:
    //void server_New_Connect();
    //void socket_Read_Data();
    //void socket_Disconnected();
    void on_pushButton_listen_clicked();

public slots:
    void connectShow();
    void Disconnected();

private:
    Ui::MainWindow *ui;
    QTcpSocket *socket;
    //QTcpServer *server;
    MyQTcpServer *server;
    int connect_cnt = 0;
};

#endif // MAINWINDOW_H
