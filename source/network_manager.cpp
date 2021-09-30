#include "network_manager.h"

NetworkManager::NetworkManager():
    m_port(26001)
{

}

bool NetworkManager::connectToHost (QTcpSocket &socket, const QString &ip)
{
    socket.connectToHost(ip, m_port);
    bool flag = socket.waitForConnected();
    if(!flag)
    {
        socket.close();
        qDebug()<<"Can't connect to " + ip;
        return false;
    }
    return true;
}

void NetworkManager::requestFileList (QTcpSocket &socket)
{
    char *ncGetList = new char [3];
    ncGetList[0]=char(20); //20 list of shared files
    ncGetList[1]='\t';
    ncGetList[2]='\n';
    socket.write(ncGetList,3);
}

QString NetworkManager::getResultAsQString (QTcpSocket &socket)
{
    QByteArray byteBuffer;
    QString recv = "";
    while(socket.waitForReadyRead())
    {
        byteBuffer=socket.readAll();
        recv+=byteBuffer.data();
    }

    return recv;
}
std::vector<RemoteHostFileData> NetworkManager::getSharedFilesByRemoteHost (const QString &ip)
{
    QTcpSocket socket;
    std::vector<RemoteHostFileData> result;
    if (!connectToHost(socket, ip))
    {
       return result;
    }
    requestFileList (socket);

    QString recv = getResultAsQString (socket);
    socket.close();

    ParseNetworkMessage parseNetworkMessage;
    return parseNetworkMessage.parseGetListResultMessage(recv);
}

QString NetworkManager::getFileHash (QTcpSocket &tcpSocket, QByteArray &fileBuffer, const QString &ip, const QString &query)
{
    connectToHost(tcpSocket, ip);
    return requestHash(tcpSocket, fileBuffer, query);
}

QString NetworkManager::requestHash(QTcpSocket & socket, QByteArray &fileBuffer, const QString &query)
{
    QByteArray hashQuery;
    hashQuery = query.toLocal8Bit();
    socket.write(hashQuery);
    socket.waitForReadyRead();


    fileBuffer=socket.readAll();

    //get hash
    QString hash="";
    int j=0;
    for (j=0; j<fileBuffer.size(); j++)
    {
        if ('#' == fileBuffer[j])
        {
            break;
        }
        hash+=fileBuffer[j];
    }
    fileBuffer = fileBuffer.remove(0,j+1);
    return hash;
}


void NetworkManager::downloadFile (const QString &ip,
                                   const QString &filePath,
                                   const QString &query,
                                   const qint64 size,
                                   std::function<void (int)> downloadProgress)
{
    QTcpSocket tcpSocket;
    QByteArray fileBuffer;

    QString remoteFileHash = getFileHash(tcpSocket,  fileBuffer, ip, query);
    createFile (tcpSocket, fileBuffer, filePath, size, downloadProgress);

    if (checkFileHash(filePath, remoteFileHash))
    {
        downloadProgress(-1); //error
    }
    else
    {
        downloadProgress(100);
    }
}


bool NetworkManager::checkFileHash (const QString &filePath, const QString &remoteFileHash)
{
    Data data;
    if (remoteFileHash!=data.getHash(filePath).data())
    {

        qDebug()<<filePath + " failed to download.";
        qDebug()<<"hash: "<<remoteFileHash;
        qDebug()<<"getHash: "<<data.getHash(filePath).data();
        return false;
    }

    return true;
}

void NetworkManager::createFile (QTcpSocket &socket,
                                 QByteArray &fileBuffer,
                                 const QString &filePath,
                                 const qint64 size, std::function<void (int)> downloadProgress)
{
    int percentage = 0;
    QFile f(filePath);

    if (!f.open(QIODevice::WriteOnly))
    {
        qDebug()<<"Client::run(): Can't open " + filePath + ".";
        socket.close();
        downloadProgress (-1);
        return;
    }

    if (fileBuffer.size()>0)
    {
        f.write(fileBuffer);
    }

    int currentSize = 0;
    while(socket.waitForReadyRead())
    {
        fileBuffer=socket.readAll();
        if (size>currentSize)
        {
            currentSize+=fileBuffer.size();
            percentage = 100*(double)(currentSize/(size*1.0));
            if (percentage != 100.0)
            {
                downloadProgress(percentage);
            }
        }

        f.write(fileBuffer);
    }

    f.close();
    socket.close();
}
