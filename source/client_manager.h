#ifndef CLIENT_MANAGER_H
#define CLIENT_MANAGER_H

#include <map>
#include <memory>


#include "client_thread.h"

class ClientManager: public QObject
{
    Q_OBJECT
public:
    ClientManager();

    void addClient (std::shared_ptr<ClientThread> clientThread);
private:
    std::map <int, std::shared_ptr<ClientThread> > m_clientThreads;
    const int                                      m_clientsNotFound;

    int  getNextClient ();
    bool isThreadRunning ();
    void startClientThread ();

public slots:
    void onClientThreadFinished (int rowId);
};

#endif // CLIENT_MANAGER_H
