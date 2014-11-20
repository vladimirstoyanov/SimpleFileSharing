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

#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QStringList>
#include <QDebug>
#include <QFile>
#include <QStringList>
#include <QThread>
#include <QList>
#include "data.h"

#define NC_GET_FILE 16
#define NC_GET_LIST 20
#define NC_RECV_LIST 40
#define NC_HELLO 01
#define NC_RECV_HELLO 02

class Server: public QThread
{
    Q_OBJECT


    QByteArray socketData;
    quint64 fileSize;
    QFile File;
    QTcpSocket *Socket;

    void send(const char*,int);
    void parseData();
public:
    QString hostIP;
    Server(qintptr ID);
    ~Server();

private:
    qintptr descriptor;
    Data returnData();
    //QList< QList<QString> > getListWithSharedFiles ();
private slots:
    void ReadyRead();
    void SockError(QAbstractSocket::SocketError);
    void disconnected();

protected:
    void run();

signals:
    void Bytes(quint32);
};

#endif // SERVER_H
