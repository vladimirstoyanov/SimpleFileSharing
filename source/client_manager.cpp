#include "client_manager.h"

ClientManager::ClientManager():
    m_clientsNotFound(-1)
{

}

ClientManager::~ClientManager()
{
}

void ClientManager::addClient (std::shared_ptr<ClientThread> clientThread)
{
    if (m_clientThreads.find(clientThread->getRowId()) == m_clientThreads.end())
    {
        QObject::connect(clientThread.get(), SIGNAL(clientThreadFinished(int)), this, SLOT(onClientThreadFinished (int)));
        m_clientThreads.insert(std::make_pair (clientThread->getRowId(), clientThread));
    }
    startClientThread();
}

void ClientManager::onClientThreadFinished (int rowId)
{
    std::map<int, std::shared_ptr<ClientThread> >::iterator it = m_clientThreads.find(rowId);
    if (it != m_clientThreads.end())
    {
        it->second->terminate();
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
