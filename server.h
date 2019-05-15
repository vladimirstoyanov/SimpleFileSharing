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

#include <memory>

#include <QDebug>
#include <QFile>
#include <QList>
#include <QStringList>
#include <QThread>
#include <QTcpServer>
#include <QTcpSocket>

#include "data.h"

//ToDo: if it possible, replace the below constants with an enum
#define NC_GET_FILE 16
#define NC_GET_LIST 20
#define NC_RECV_LIST 40
#define NC_HELLO 01
#define NC_RECV_HELLO 02

class Server: public QThread
{
    Q_OBJECT

    void send(const char*,int);
    void parseData();

public:
    //ToDo: move the below member to the private section
    QFile m_file;

public:
    Server(qintptr ID);
    virtual ~Server();

protected:
    void run();

private:
    qintptr m_descriptor;
    QByteArray m_socketData;
    std::shared_ptr<QTcpSocket> m_tcpSocket;

    Data returnData();

private slots:
    void disconnected();
    void ReadyRead(); //ToDo: change the name to readyRead ()
    void SockError(QAbstractSocket::SocketError); //ToDo: change the name to sockError


signals:
    void Bytes(quint32); //ToDo: change the name to bytes
};

#endif // SERVER_H
