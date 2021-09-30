#ifndef NETWORK_MANAGER_H
#define NETWORK_MANAGER_H

#include <functional>
#include <vector>

#include <QFile>
#include <QNetworkInterface>
#include <QString>
#include <QTcpServer>
#include <QTcpSocket>

#include "data.h"
#include "parse_network_message.h"
#include "remote_host_file_data.h"

class NetworkManager
{
public:
    NetworkManager();
    virtual ~NetworkManager () = default;

    void                            downloadFile (const QString &ip,
                                                  const QString &fileName,
                                                  const QString &fileDirectory,
                                                  const QString &query,
                                                  const qint64 size,
                                                  std::function<void(int)> downloadProgress);
    std::vector<RemoteHostFileData> getSharedFilesByRemoteHost (const QString &ip);

private:
    int         m_port;

    bool        checkFileHash (const QString &filePath, const QString &remoteFileHash);
    bool        connectToHost (QTcpSocket &socket, const QString &ip);
    void        createFile (QTcpSocket &socket, QByteArray &ba1, const QString &fileName, const QString &fileDirectory, const qint64 size, std::function<void(int)> downloadProgress);
    QString     getResultAsQString (QTcpSocket &socket);
    QString     getFileHash (QTcpSocket &tcpSocket, QByteArray &ba1, const QString &ip, const QString &query);
    void        requestFileList (QTcpSocket &socket);
    QString     requestHash(QTcpSocket & socket, QByteArray &ba1, const QString &query);
};

#endif // NETWORK_MANAGER_H
