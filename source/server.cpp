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
#include <QDebug>
Server::Server(const std::shared_ptr<SharedFiles> &sharedFiles, QObject *parent) :
    QTcpServer(parent)
    , m_sharedFiles(sharedFiles)
{
}

Server::~Server()
{
    for (auto& pair : m_serverThreads)
    {
        if (pair.second->isRunning())
        {
            pair.second->quit();
            pair.second->wait();
        }
    }
}

void Server::StartServer()
{
    NetworkManager networkManager;
    int port = networkManager.getPort ();
    if (!this->listen(QHostAddress::Any, port))
    {
        QString errorMessage = QString("Port %1 can't be opened!").arg(port);
        qWarning() << errorMessage;

        // Let the UI tier handle this presentation
        emit serverErrorOccurred(errorMessage);
    }
}

void Server::incomingConnection(qintptr id)
{
    qDebug()<<__PRETTY_FUNCTION__<<": ID: "<<QString::number(id);
    std::shared_ptr<ServerThread> thread = std::make_shared<ServerThread>(id, m_sharedFiles);

    connect(thread.get(), &ServerThread::serverThreadFinished, this, &Server::onServerThreadFinished);

    thread->start();

    m_serverThreads.insert(std::make_pair(id, thread));
}

void Server::onServerThreadFinished (qint64 id)
{
    qDebug()<<__PRETTY_FUNCTION__<<": Thread with id has finished: "<<id;

    auto it = m_serverThreads.find(id);
    if (it != m_serverThreads.end())
    {
        m_serverThreads.erase(it);
    }
}
