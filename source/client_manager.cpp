#include "client_manager.h"

ClientManager::ClientManager():
    m_clientsNotFound(-1)
{

}

void ClientManager::addClient (std::shared_ptr<ClientThread> clientThread)
{
    if (m_clientThreads.find(clientThread->getRowId()) == m_clientThreads.end())
    {
        QObject::connect(clientThread.get(), SIGNAL(clientThreadFinished(int)), this, SLOT(onClientThreadFinished (int)));
        m_clientThreads[clientThread->getRowId()] = clientThread;
    }
    startClientThread();
}

void ClientManager::onClientThreadFinished (int rowId)
{
    if (m_clientThreads.find(rowId) != m_clientThreads.end())
    {
        m_clientThreads[rowId]->terminate();
        m_clientThreads.erase(rowId);
    }
    startClientThread();
}

void ClientManager::startClientThread ()
{
    if (!isThreadRunning())
    {
        int rowId =getNextClient();
        if (rowId!=m_clientsNotFound)
        {
            m_clientThreads[rowId]->start();
        }
    }
}

bool ClientManager::isThreadRunning ()
{
    for (auto &item: m_clientThreads)
    {
        if (item.second->isRunning())
        {
            return true;
        }
    }
    return false;
}

int ClientManager::getNextClient ()
{
    for (auto &item: m_clientThreads)
    {
        return item.second->getRowId();
    }
    return m_clientsNotFound;
}
