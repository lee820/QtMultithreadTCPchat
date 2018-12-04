#ifndef COMMON_H
#define COMMON_H

#include <QtNetwork>
class Common
{
public:
    static QMap<QTcpSocket *, QString>g_user_socket;
};

#endif // COMMON_H
