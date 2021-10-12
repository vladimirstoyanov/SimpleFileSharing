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

#ifndef SCAN_NETWORK_H
#define SCAN_NETWORK_H

#include <QDebug>
#include <QList>
#include <QMutex>
#include <QRunnable>
#include <QTcpSocket>
#include <QThread>
#include <QThreadPool>
#include <QtCore>

#include <vector>
#include "network_manager.h"

class ScanNetwork: public QObject, public QRunnable
{
    Q_OBJECT

public:
    ScanNetwork(const unsigned int maxHostCount);

    void    run();
    int     scanIP(const QString &ip);

    void    markHostAsScanned (const unsigned int index);
    void    markHostInProgress (const unsigned int index);
    void    markHostAsUnscanned (const unsigned int index);
    QString getCurrentIP () const { return this->m_currentIP; }
    void    setCurrentIP (const QString &currentIP) { this->m_currentIP = currentIP; }


private:
    QString             m_currentIP;
    int                 m_getHostIndexError;
    int                 m_hostInProgress;
    int                 m_hostScanned;
    int                 m_hostUnscanned;
    unsigned int        m_maxHostCount;
    QMutex              m_mutex;
    NetworkManager      m_networkManager;
    std::vector<int>    m_scannedIPAddresses;

    void    checkIsScanFinished();
    int     getHostIp (QString &ip);

signals:
    void    foundHost(const QString &ip);
    void    scanFinished();
};

#endif // SCAN_NETWORK_H
