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
#include <vector>

#include <QDebug>
#include <QFile>
#include <QStringList>
#include <QThread>
#include <QTcpServer>
#include <QTcpSocket>

#include "data.h"
#include "file_data.h"
#include "message_codes.h"
#include "network_manager.h"
#include "shared_files.h"

class Server: public QThread
{
    Q_OBJECT
public:
    Server(qintptr ID, std::shared_ptr<SharedFiles> sharedFiels);
    virtual ~Server();

protected:
    void run();

private:
    qintptr m_descriptor;
    std::shared_ptr<SharedFiles> m_sharedFiles;
    QByteArray m_socketData;
    std::shared_ptr<QTcpSocket> m_tcpSocket;

    Data returnData();
    int  getFileIndex (const Data &data, const std::vector<FileData> &files);
    void parseData();

private slots:
    void disconnected();
    void readyToRead();
    void sockError(QAbstractSocket::SocketError); //ToDo: change the name to sockError

signals:
    void Bytes(qint64); //ToDo: change the name to bytes
};

#endif // SERVER_H
