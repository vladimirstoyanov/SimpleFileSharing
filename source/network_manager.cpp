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
    char *ncGetList = new char [CODE_LENGTH];
    ncGetList[0]=char(NC_GET_LIST);
    ncGetList[1]='\t';
    ncGetList[2]='\n';
    socket.write(ncGetList, CODE_LENGTH);
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
        downloadProgress(DOWNLOAD_ERROR);
    }
    else
    {
        downloadProgress(DOWNLOAD_FINISHED);
    }
}


bool NetworkManager::checkFileHash (const QString &filePath, const QString &remoteFileHash)
{
    if (remoteFileHash!=m_fileOperations.getFileHash(filePath).data())
    {
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
        qDebug()<<__PRETTY_FUNCTION__<<": Can't open " + filePath + ".";
        socket.close();
        downloadProgress (DOWNLOAD_ERROR);
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


void NetworkManager::sendBuffer (QTcpSocket &socket, const char *data, int size)
{
    socket.write(data,size);
    socket.waitForBytesWritten();
}

bool NetworkManager::sendFile (QTcpSocket & tcpSocket, const FileData &fileData)
{
    QString fileHash= m_fileOperations.getFileHash(fileData.getPath());
    fileHash +='#';
    QByteArray fileHashBytes = fileHash.toLocal8Bit();
    sendBuffer(tcpSocket, fileHashBytes.constData(), fileHashBytes.size());

    QFile file(fileData.getPath());
    if (!file.open(QIODevice::ReadOnly))
    {
        return false;
    }

    while (!file.atEnd())
    {
        QByteArray  bytes = file.read(BUFFER_SIZE);
        sendBuffer(tcpSocket, bytes.constData(),bytes.size());
    }
    return true;
}


void NetworkManager::sendSharedFilesList (QTcpSocket &tcpSocket, const std::vector<FileData> &files, ProtocolData &data)
{
    QString sharedFiles = "";
    qDebug()<<"files.size():"<<sharedFiles.size();
    for (unsigned int i=0; i<files.size(); ++i)
    {
        QFileInfo fi(files[i].getPath());
        sharedFiles+=QString::number(fi.size());

        sharedFiles+= "#";
        sharedFiles+=files[i].getFileName() + "\n";
    }

    int index = 0;
    qDebug()<<files.size();
    qDebug()<<"Size: "<<sharedFiles.length();
    QString part = sharedFiles.mid(index, index+BUFFER_SIZE);
    index+=BUFFER_SIZE;
    QByteArray sendData;
    sendData = part.toLocal8Bit();
    data.fromChar(sendData, nullptr, NC_RECV_LIST);
    sendBuffer(tcpSocket, data.getString(), data.getSize());
    while (index < sharedFiles.length())
    {
        qDebug()<<"index:"<<index;
        part = sharedFiles.mid(index, BUFFER_SIZE);
        index+=BUFFER_SIZE;
        sendData = part.toLocal8Bit();
        sendBuffer(tcpSocket, sendData.constData(), sendData.size());
    }
}

void NetworkManager::sendHelloMessage (QTcpSocket& tcpSocket)
{
    sendBuffer(tcpSocket, "\x02\t\n", CODE_LENGTH);
}
