/*
    This file is part of Simple File Sharing.

    Simple File Sharing is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Simple File Sharing is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Simple File Sharing.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "server_thread.h"
#include "search.h"
#include <QFileInfo>
#include <QDir>
#include "GUI/addfile_gui.h"

ServerThread::ServerThread(qintptr id, std::shared_ptr<SharedFiles> sharedFiels):
    m_descriptor(id)
    , m_sharedFiles (sharedFiels)
{
}

ServerThread::~ServerThread()
{
    qDebug()<<__PRETTY_FUNCTION__;
}

void ServerThread::readyToRead()
{
    qDebug()<<__PRETTY_FUNCTION__;
    qint64 bytes = m_tcpSocket.bytesAvailable();
    m_socketData = m_tcpSocket.read(bytes);

    parseData();
}

int ServerThread::getFileIndex (const ProtocolData &data, const std::vector<FileData> &files)
{
    int index=DOWNLOAD_ERROR;
    QByteArray arguments = data.getArguments();
    qDebug()<<arguments;

    int i=0;
    QString fileId="", filename="";
    while(i<arguments.size() && arguments[i]!='#')
    {
        fileId+=arguments[i];
        ++i;
    }
    ++i;
    while(i<arguments.size() && arguments[i]!='\0')
    {
        filename+=arguments[i];
        ++i;
    }

    for (unsigned int i=0; i<files.size(); ++i)
    {
        if (filename == files[i].getFileName() && fileId == files[i].getFileId())
        {
            index = i;
            break;
        }
    }

    return index;
}
void ServerThread::parseData()
{
     ProtocolData protocolData = getProtocolData();
     NetworkManager networkManager;
     std::vector<FileData> files  = m_sharedFiles->get();
     int message  = protocolData.getMessageCode();
     switch (message)
     {
        case NC_GET_FILE:
        {
                qDebug()<<__PRETTY_FUNCTION__<<":Get file requested";
                int index = getFileIndex (protocolData, files);
                if (DOWNLOAD_ERROR == index)
                {
                    //ToDo: notify the GUI
                    break;
                }
                networkManager.sendFile(m_tcpSocket, files[index]);

                break;
        }
        case NC_GET_LIST:
        {
                qDebug()<<__PRETTY_FUNCTION__<<":File list requested";
                networkManager.sendSharedFilesList(m_tcpSocket, files);
                break;
        }
        case NC_HELLO:
        {
                qDebug()<<__PRETTY_FUNCTION__<<":hello message received";
                networkManager.sendHelloMessageReceived(m_tcpSocket);
                break;
        }
        default:
        {
                break;
        }
     }
     emit serverThreadFinished(m_descriptor);
}


ProtocolData ServerThread::getProtocolData()
{
    ParseNetworkMessage parseNetworkMessage;
    return parseNetworkMessage.parseProtocolData(m_socketData);
}

void ServerThread::sockError(QAbstractSocket::SocketError error)
{
    qDebug()<<__PRETTY_FUNCTION__<<':'<<error;
    m_tcpSocket.close();
}

void ServerThread::run()
{
    if(!m_tcpSocket.setSocketDescriptor(m_descriptor))
    {
        qDebug()<<__PRETTY_FUNCTION__<<": Can't set a socket descriptor...";
        return;
    }

    connect(&m_tcpSocket,SIGNAL(readyRead()),SLOT(readyToRead()),Qt::DirectConnection);
    connect(&m_tcpSocket,SIGNAL(disconnected()),this,SLOT(disconnected()),Qt::DirectConnection);
    connect(&m_tcpSocket,SIGNAL(error(QAbstractSocket::SocketError)),SLOT(sockError(QAbstractSocket::SocketError)),Qt::QueuedConnection);

    exec();
}

void ServerThread::disconnected()
{
   qDebug ()<<__PRETTY_FUNCTION__;
   emit serverThreadFinished(m_descriptor);
}
