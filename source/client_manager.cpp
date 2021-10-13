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
        clientThread->start();
    }
}

void ClientManager::onClientThreadFinished (int rowId)
{
    if (m_client_threads.find(rowId) != m_client_threads.end())
    {
        m_client_threads[rowId]->quit();
        m_client_threads[rowId]->deleteLater();
        m_client_threads.erase(rowId);
    }
}
