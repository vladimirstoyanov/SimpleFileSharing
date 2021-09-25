#include "socket.h"

Socket::Socket()
{

}

bool Socket::connectToHost (QTcpSocket &socket, const QString &ip, int port)
{
    socket.connectToHost(ip, port);
    bool flag = socket.waitForConnected();
    if(!flag)
    {
        socket.close();
        qDebug()<<"Can't connect to " + ip;
        return false;
    }
    return true;
}

void Socket::requestFileList (QTcpSocket &socket)
{
    char *ncGetList = new char [3];
    ncGetList[0]=char(20); //20 list of shared files
    ncGetList[1]='\t';
    ncGetList[2]='\n';
    socket.write(ncGetList,3);
}

QString Socket::getResultAsQString (QTcpSocket &socket)
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
std::vector<RemoteHostFileData> Socket::getSharedFilesByRemoteHost (const QString &ip, int port)
{
    QTcpSocket socket;
    std::vector<RemoteHostFileData> result;
    if (!connectToHost(socket, ip, port))
    {
       return result;
    }
    requestFileList (socket);

    QString recv = getResultAsQString (socket);
    socket.close();

    ParseNetworkMessage parseNetworkMessage;
    return parseNetworkMessage.parseGetListResultMessage(recv);
}
