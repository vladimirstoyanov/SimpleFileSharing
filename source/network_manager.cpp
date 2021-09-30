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

QString NetworkManager::getFileHash (QTcpSocket &tcpSocket, QByteArray &ba1, const QString &ip, const QString &query)
{
    connectToHost(tcpSocket, ip);
    return requestHash(tcpSocket, ba1, query);
}

QString NetworkManager::requestHash(QTcpSocket & socket, QByteArray &ba1, const QString &query)
{
    QByteArray ba2;
    ba2 = query.toLocal8Bit();
    socket.write(ba2);
    socket.waitForReadyRead();


    ba1=socket.readAll();

    //get hash
    QString hash="";
    int j=0;
    for (j=0; j<ba1.size(); j++)
    {
        if (ba1[j]=='#')
        {
            break;
        }
        hash+=ba1[j];
    }
    ba1 = ba1.remove(0,j+1);
    return hash;
}


void NetworkManager::downloadFile (const QString &ip, const QString &fileName, const QString &fileDirectory, const QString &query, const qint64 size, std::function<void (int)> downloadProgress)
{
    QTcpSocket tcpSocket;
    QByteArray ba1;

    QString remoteFileHash = getFileHash(tcpSocket,  ba1, ip, query);
    createFile (tcpSocket, ba1, fileName, fileDirectory,size, downloadProgress);

    if (checkFileHash(fileDirectory + fileName, remoteFileHash))
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

void NetworkManager::createFile (QTcpSocket &socket, QByteArray &ba1, const QString &fileName, const QString &fileDirectory, const qint64 size, std::function<void (int)> downloadProgress)
{
    int percentage = 0;
    QFile f(fileDirectory + fileName);

    if (!f.open(QIODevice::WriteOnly))
    {
        qDebug()<<"Client::run(): Can't open " + fileDirectory + fileName + ".";
        socket.close();
        return;
    }

    if (ba1.size()>0)
    {
        f.write(ba1);
    }

    int b = 0;

    while(socket.waitForReadyRead())
    {
        ba1=socket.readAll();
        if (size>b)
        {
            b+=ba1.size();
            double a = b/(size*1.0);
            percentage = 100*a;
            if (percentage != 100.0)
            {
                downloadProgress(percentage);
            }
        }

        f.write(ba1);
    }

    f.close();
    socket.close();
}
