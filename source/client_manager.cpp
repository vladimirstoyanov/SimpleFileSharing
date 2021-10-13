#include "client_manager.h"

ClientManager::ClientManager()
{

}

void ClientManager::addClient (std::shared_ptr<ClientThread> clientThread)
{
    if (m_client_threads.find(clientThread->getRowId()) == m_client_threads.end())
    {
        QObject::connect(clientThread.get(), SIGNAL(clientThreadFinished(int)), this, SLOT(onClientThreadFinished (int)));
        m_client_threads[clientThread->getRowId()] = clientThread;
    }
    startClientThread();
}

void ClientManager::onClientThreadFinished (int rowId)
{
    if (m_client_threads.find(rowId) != m_client_threads.end())
    {
        m_client_threads[rowId]->terminate();
        m_client_threads.erase(rowId);
    }
    startClientThread();
}

void ClientManager::startClientThread ()
{
    if (!isThreadRunning())
    {
        int rowId =getNextClient();
        if (rowId!=-1)
        {
            m_client_threads[rowId]->start();
        }
    }
}

bool ClientManager::isThreadRunning ()
{
    for (auto &item: m_client_threads)
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
    for (auto &item: m_client_threads)
    {
        return item.second->getRowId();
    }
    return -1;
}
