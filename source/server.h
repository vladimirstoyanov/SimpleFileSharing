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

#ifndef MYSERVER_H
#define MYSERVER_H

#include <QMessageBox>
#include <QTcpServer>
#include <map>

#include "network_manager.h"
#include "shared_files.h"
#include "server_thread.h"

class Server : public QTcpServer
{
    Q_OBJECT

public:
    explicit Server(std::shared_ptr<SharedFiles> sharedFiles, QObject *parent = 0);
    virtual ~Server ();
    void StartServer();

protected:
    void incomingConnection(qintptr id);

private:
    std::shared_ptr<SharedFiles> m_sharedFiles;
    std::map <qint64,  std::shared_ptr<ServerThread> > m_serverThreads;

private slots:
    void onServerThreadFinished (qint64 id);
};

#endif // MYSERVER_H
