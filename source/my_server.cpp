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

#include "my_server.h"
#include <QDebug>
MyServer::MyServer(std::shared_ptr<SharedFiles> sharedFiles, QObject *parent) :
    QTcpServer(parent)
    , m_sharedFiles(sharedFiles)
{
}

void MyServer::StartServer()
{
    if (!this->listen(QHostAddress::Any, 26001)) //listen on 26001 port
    {
        QMessageBox::critical(nullptr,"Error!", "Can't share files! Please open port 26001 on your firewall!");
    }
}

void MyServer::incomingConnection(qintptr ID)
{
    qDebug()<<"Incoming connection:" + QString::number(ID);
    std::shared_ptr<Server> thread = std::make_shared<Server>(ID, m_sharedFiles);

    connect(thread.get(), SIGNAL(finished()),thread.get(), SLOT(deleteLater()));

    thread->start();
}
