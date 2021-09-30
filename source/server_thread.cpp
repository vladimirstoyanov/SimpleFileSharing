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
#include "addfile_gui.h"

ServerThread::ServerThread(qintptr id, std::shared_ptr<SharedFiles> sharedFiels):
    m_descriptor(id)
    , m_sharedFiles (sharedFiels)
{
}

ServerThread::~ServerThread()
{
}

void ServerThread::readyToRead()
{
    qDebug()<<__PRETTY_FUNCTION__;
    qint64 bytes = m_tcpSocket.bytesAvailable();
    emit Bytes(bytes);

    char *buf = new char[bytes];
    m_tcpSocket.read(buf,bytes);
    m_socketData.append(buf,  bytes); //FIXME
    delete []buf;

    parseData();
}

int ServerThread::getFileIndex (const Data &data, const std::vector<FileData> &files)
{
    int index=DOWNLOAD_ERROR;
    QByteArray arguments = data.getArguments();
    qDebug()<<arguments;

    int i=0;
    QString num="", filename="";
    while(i<arguments.size() && arguments[i]!='#')
    {
        num+=arguments[i];
        ++i;
    }
    ++i;
    while(i<arguments.size() && arguments[i]!='\0')
    {
        filename+=arguments[i];
        ++i;
    }
    int number = num.toInt();
    for (unsigned int i=0; i<files.size(); ++i)
    {
        if (filename == files[i].getFileName())
        {
            if (number>0)
            {
                --number;
                continue;
            }
            index = i;
            break;
        }
    }

    return index;
}
void ServerThread::parseData()
{
     Data data = returnData();
     NetworkManager networkManager;
     std::vector<FileData> files  = m_sharedFiles->get();

     switch (data.getType())
     {
        case NC_GET_FILE:
        {

                int index = getFileIndex (data, files);
                if (DOWNLOAD_ERROR == index)
                {
                    //ToDo: notify the GUI
                    break;
                }
                networkManager.sendFile(m_tcpSocket, files[index], data);

                break;
        }
        case NC_GET_LIST:
        {
                networkManager.sendSharedFilesList(m_tcpSocket, files, data);
                break;
        }
        case NC_HELLO:
        {
                networkManager.sendHelloMessage(m_tcpSocket);
                break;
        }
        default:
        {
                break;
        }
     }
     m_tcpSocket.close();
}


Data ServerThread::returnData()
{
    Data data;
    if(m_socketData.length()<CODE_LENGTH)
    {
        return data;
    }

    for(int i=0;i<m_socketData.length(); ++i)
    {
        if('\n' == m_socketData[i])
        {
            QByteArray bSize;
            for(int j=0;j<=i;++j)
            {
                bSize.append(m_socketData[j]);
            }

            m_socketData = m_socketData.remove(0,bSize.toInt());
            data.fromChar(bSize.constData());
            break;
        }
    }
    return data;
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
   m_tcpSocket.deleteLater();
   exit(0);
}
