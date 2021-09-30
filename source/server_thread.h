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

#ifndef SERVER_THREAD_H
#define SERVER_THREAD_H

#include <memory>
#include <vector>

#include <QDebug>
#include <QFile>
#include <QStringList>
#include <QThread>
#include <QTcpServer>
#include <QTcpSocket>

#include "file_data.h"
#include "message_codes.h"
#include "network_manager.h"
#include "parse_network_message.h"
#include "protocol_data.h"
#include "shared_files.h"

class ServerThread: public QThread
{
    Q_OBJECT
public:
    ServerThread(qintptr id, std::shared_ptr<SharedFiles> sharedFiels);
    virtual ~ServerThread();

protected:
    void run();

private:
    qintptr m_descriptor;
    std::shared_ptr<SharedFiles> m_sharedFiles;
    QByteArray m_socketData;
    QTcpSocket m_tcpSocket;

    ProtocolData getProtocolData();
    int  getFileIndex (const ProtocolData &data, const std::vector<FileData> &files);
    void parseData();

private slots:
    void disconnected();
    void readyToRead();
    void sockError(QAbstractSocket::SocketError);
};

#endif // SERVER_THREAD_H
