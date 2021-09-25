#ifndef SOCKET_H
#define SOCKET_H

#include <vector>

#include <QNetworkInterface>
#include <QString>
#include <QTcpServer>
#include <QTcpSocket>

#include "parse_network_message.h"
#include "remote_host_file_data.h"

class Socket
{
public:
    Socket();
    virtual ~Socket () = default;

    std::vector<RemoteHostFileData> getSharedFilesByRemoteHost (const QString &ip, int port);

private:
    bool        connectToHost (QTcpSocket &socket, const QString &ip, int port);
    QString     getResultAsQString (QTcpSocket &socket);
    void        requestFileList (QTcpSocket &socket);
};

#endif // SOCKET_H
