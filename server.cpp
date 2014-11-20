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

Server::Server(qintptr ID)
{
    descriptor = ID;
}

Server::~Server()
{
}

void Server::ReadyRead()
{
    qDebug()<<"Ready read..";
    int bytes = Socket->bytesAvailable();
    emit Bytes(bytes);

    char *buf = new char[bytes];
    Socket->read(buf,bytes);
    socketData.append(buf,bytes); //QByteArray
    delete []buf;

    parseData();
}

void Server::parseData()
{
     Data d = returnData();
     AddFile_GUI addfile;
     switch (d.type)
     {
        case NC_GET_FILE://this case block send file
        {

                //check d.string is to list
                QList< QList<QString> > files = addfile.getListWithSharedFiles();
                int index=-1;
                qDebug()<<d.arguments;

                //get index
                int i=0;
                QString num="", filename="";
                while(i<d.arguments.size() && d.arguments[i]!='#')
                {
                    num+=d.arguments[i];
                    i++;
                }
                i++;
                while(i<d.arguments.size() && d.arguments[i]!='\0')
                {
                    filename+=d.arguments[i];
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
                QByteArray send_data;
                send_data = part.toLocal8Bit();
                d.FromChar(send_data,0,NC_RECV_LIST);
                send(d.string, d.size);
                while (index < sharedFiles.length())
                {
                    qDebug()<<"index:"<<index;
                    part = sharedFiles.mid(index, 8192);
                    index+=8192;
                    send_data = part.toLocal8Bit();
                    send(send_data.constData(), send_data.size());
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
     Socket->close();
}


Data Server::returnData()
{
    Data d;
    if(socketData.length()<3)
        return d;

    for(int i=0;i<socketData.length();i++)
    {
        if(socketData[i] == '\n')
        {
            QByteArray bSize;
            for(int j=0;j<=i;j++)
                bSize.append(socketData[j]);

            //QByteArray str = socketData.left(bSize.toInt());
            socketData = socketData.remove(0,bSize.toInt());
            d.FromChar(bSize.constData());
            break;
        }
    }
    return d;
}

void Server::SockError(QAbstractSocket::SocketError error)
{
    Socket->close();
}

void Server::send(const char *data, int size)
{
    Socket->write(data,size);
    Socket->waitForBytesWritten();
}

void Server::run()
{

    Socket = new QTcpSocket();

    qDebug()<<"Run funtion";
    if(!Socket->setSocketDescriptor(descriptor))
    {
        qDebug()<<"Can't set socket descriptor...";
        return;
    }

    connect(Socket,SIGNAL(readyRead()),SLOT(ReadyRead()),Qt::DirectConnection);
    connect(Socket,SIGNAL(disconnected()),this,SLOT(disconnected()),Qt::DirectConnection);
    connect(Socket,SIGNAL(error(QAbstractSocket::SocketError)),SLOT(SockError(QAbstractSocket::SocketError)),Qt::QueuedConnection);

    exec();
}

void Server::disconnected()
{
   Socket->deleteLater();
   exit(0);
}
/*
QList< QList<QString> > Server::getListWithSharedFiles()
{
    CSearch cs;
    QList< QList<QString> > l_shared_files;

    QFile file("sharedFiles");
    if(!file.open(QIODevice::ReadOnly)) {
        return l_shared_files;
    }

    QTextStream in(&file);

    while(!in.atEnd()) {
        QString line = in.readLine();
        int index =0;
        cs.Search_After(line,"<file>", &index);
        if (index != 6)
        {
            file.close();
            return l_shared_files;
        }

        line = in.readLine();
        QString file_name = "";
        if (cs.getTextBetweenTwoStrings(line,"<file_name>", "</file_name>", file_name))
        {
            file.close();
            return l_shared_files;
        }

        line = in.readLine();
        QString path = "";
        if (cs.getTextBetweenTwoStrings(line,"<path>", "</path>", path))
        {
            file.close();
            return l_shared_files;
        }

        line = in.readLine();
        QString size="";
        if (cs.getTextBetweenTwoStrings(line,"<size>", "</size>", size))
        {
            file.close();
            return l_shared_files;
        }

        line = in.readLine();
        index = 0;
        cs.Search_After(line,"</file>", &index);
        if (index != 7)
        {
            file.close();
            return l_shared_files;
        }

        //add to tableView
        QList<QString> item;
        item.push_back(file_name);
        item.push_back(path);
        item.push_back(size);
        l_shared_files.push_back(item);
    }

    file.close();
    return l_shared_files;
}
*/
