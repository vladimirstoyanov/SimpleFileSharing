#ifndef NETWORK_MANAGER_H
#define NETWORK_MANAGER_H

#include <functional>
#include <vector>

#include <QFile>
#include <QFileInfo>
#include <QNetworkInterface>
#include <QString>
#include <QTcpServer>
#include <QTcpSocket>

#include "data.h"
#include "file_data.h"
#include "message_codes.h"
#include "parse_network_message.h"
#include "remote_host_file_data.h"

class NetworkManager
{
public:
    NetworkManager();
    virtual ~NetworkManager () = default;

    void                            downloadFile (const QString &ip,
                                                  const QString &filePath,
                                                  const QString &query,
                                                  const qint64 size,
                                                  std::function<void(int)> downloadProgress);
    std::vector<RemoteHostFileData> getSharedFilesByRemoteHost (const QString &ip);
    void                            sendHelloMessage (QTcpSocket &socket);
    void                            sendBuffer (QTcpSocket &socket, const char *data, int size);
    bool                            sendFile (QTcpSocket & tcpSocket, const FileData &fileData, Data &data);
    void                            sendSharedFilesList(QTcpSocket &tcpSocket, const std::vector<FileData> &sharedFiles, Data &data);

private:
    int         m_port;

    bool        checkFileHash (const QString &filePath, const QString &remoteFileHash);
    bool        connectToHost (QTcpSocket &socket, const QString &ip);
    void        createFile (QTcpSocket &socket, QByteArray &fileBuffer, const QString &filePath, const qint64 size, std::function<void(int)> downloadProgress);
    QString     getResultAsQString (QTcpSocket &socket);
    QString     getFileHash (QTcpSocket &tcpSocket, QByteArray &fileBuffer, const QString &ip, const QString &query);
    void        requestFileList (QTcpSocket &socket);
    QString     requestHash(QTcpSocket & socket, QByteArray &ba1, const QString &query);


};

#endif // NETWORK_MANAGER_H
