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

#include "server.h"
#include "search.h"
#include <QFileInfo>
#include <QDir>
#include "addfile_gui.h"

Server::Server(qintptr ID):
    m_descriptor(ID)
    , m_tcpSocket (std::make_shared<QTcpSocket>())
{
}

Server::~Server()
{
}

void Server::ReadyRead()
{
    qDebug()<<"Ready read..";
    int bytes = m_tcpSocket->bytesAvailable();
    emit Bytes(bytes);

    char *buf = new char[bytes];
    m_tcpSocket->read(buf,bytes);
    m_socketData.append(buf,bytes); //QByteArray
    delete []buf;

    parseData();
}

void Server::parseData()
{
     Data d = returnData();
     AddFile_GUI addfile;
     switch (d.getType())
     {
        case NC_GET_FILE://this case block send file
        {

                //check d.string is to list
                QList< QList<QString> > files = addfile.getListWithSharedFiles();
                int index=-1;
                QByteArray arguments = d.getArguments();
                qDebug()<<arguments;

                //get index
                int i=0;
                QString num="", filename="";
                while(i<arguments.size() && arguments[i]!='#')
                {
                    num+=arguments[i];
                    i++;
                }
                i++;
                while(i<arguments.size() && arguments[i]!='\0')
                {
                    filename+=arguments[i];
                    i++;
                }
                int number = num.toInt();
                for (int i=0; i<files.size(); i++)
                {
                    if (filename == files[i][0])
                    {
                        if (number>0)
                        {
                            number--;
                            continue;
                        }
                        index = i;
                        break;
                    }
                }

                if (index==-1)
                    break;

                //send hash
                QString s_hash= d.getHash(files[index][1]);
                s_hash +='#';
                QByteArray b_hash = s_hash.toLocal8Bit();
                send(b_hash.constData(), b_hash.size());

                QFile file(files[index][1]);
                if (!file.open(QIODevice::ReadOnly))
                    break;

                while (!file.atEnd()) {
                    QByteArray  bytes = file.read(8192);
                    send(bytes.constData(),bytes.size());
                }

                break;
        }
        case NC_GET_LIST:
        {
                QList< QList<QString> > files = addfile.getListWithSharedFiles();
                QString sharedFiles = "";
                qDebug()<<"files.size():"<<files.size();
                for (int i=0; i<files.size(); i++)
                {
                    if (files[i].size()<3)
                    {
                        qDebug()<<"ignore:"<<i;
                        continue;
                    }
                    QFileInfo fi(files[i][1]);
                    sharedFiles+=QString::number(fi.size());

                    //sharedFiles+=getHash(files[i][1]);
                    sharedFiles+= "#";
                    sharedFiles+=files[i][0] + "\n";
                }
                int index = 0;
                //qDebug()<<sharedFiles;
                qDebug()<<files.size();
                qDebug()<<"SIZE: "<<sharedFiles.length();
                QString part = sharedFiles.mid(index, index+8192);
                index+=8192;
                QByteArray sendData;
                sendData = part.toLocal8Bit();
                d.fromChar(sendData,0,NC_RECV_LIST);
                send(d.getString(), d.getSize());
                while (index < sharedFiles.length())
                {
                    qDebug()<<"index:"<<index;
                    part = sharedFiles.mid(index, 8192);
                    index+=8192;
                    sendData = part.toLocal8Bit();
                    send(sendData.constData(), sendData.size());
                }
                break;
        }
        case NC_HELLO:
        {
                send("\x02\t\n",3);
                break;
        }
            default:
                break;
     }
     m_tcpSocket->close();
}


Data Server::returnData()
{
    Data d;
    if(m_socketData.length()<3)
        return d;

    for(int i=0;i<m_socketData.length();i++)
    {
        if(m_socketData[i] == '\n')
        {
            QByteArray bSize;
            for(int j=0;j<=i;j++)
                bSize.append(m_socketData[j]);

            //QByteArray str = socketData.left(bSize.toInt());
            m_socketData = m_socketData.remove(0,bSize.toInt());
            d.fromChar(bSize.constData());
            break;
        }
    }
    return d;
}

void Server::SockError(QAbstractSocket::SocketError error)
{
    m_tcpSocket->close();
}

void Server::send(const char *data, int size)
{
    m_tcpSocket->write(data,size);
    m_tcpSocket->waitForBytesWritten();
}

void Server::run()
{
    qDebug()<<"Run funtion";
    if(!m_tcpSocket->setSocketDescriptor(m_descriptor))
    {
        qDebug()<<"Can't set socket descriptor...";
        return;
    }

    connect(m_tcpSocket.get(),SIGNAL(readyRead()),SLOT(ReadyRead()),Qt::DirectConnection);
    connect(m_tcpSocket.get(),SIGNAL(disconnected()),this,SLOT(disconnected()),Qt::DirectConnection);
    connect(m_tcpSocket.get(),SIGNAL(error(QAbstractSocket::SocketError)),SLOT(SockError(QAbstractSocket::SocketError)),Qt::QueuedConnection);

    exec();
}

void Server::disconnected()
{
   m_tcpSocket->deleteLater();
   exit(0);
}
