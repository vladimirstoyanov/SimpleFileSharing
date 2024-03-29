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

#include "scan_network_thread.h"

ScanNetworkThread::ScanNetworkThread(const unsigned int maxHostCount)
    : QRunnable()
    , m_currentIP ("")
    , m_getHostIndexError(-1)
    , m_hostInProgress (1)
    , m_hostScanned (2)
    , m_hostUnscanned (0)
    , m_maxHostCount(maxHostCount)
    , m_mutex ()
    , m_scannedIPAddresses(m_maxHostCount)

{
}

int ScanNetworkThread::getUnscannedHostIp (QString &ip)
{
    ip = m_currentIP;
    int hostNumber = m_getHostIndexError;
    for (unsigned int i=0; i<m_scannedIPAddresses.size(); ++i)
    {
        if (m_hostUnscanned == m_scannedIPAddresses[i])
        {
            m_scannedIPAddresses[i]=m_hostInProgress;
            ip+=QString::number(i+1);
            hostNumber=i;
            break;
        }
    }

    return hostNumber;
}

void ScanNetworkThread::run()
{
    m_mutex.lock();
    if ("" == m_currentIP)
    {
        m_mutex.unlock();
        return;
    }
    QString ip = "";
    int index = getUnscannedHostIp (ip);
    qDebug ()<<__PRETTY_FUNCTION__<<": "<<"Trying to check "<<ip<<", index: "<<index;
    m_mutex.unlock();

    if (m_getHostIndexError == index)
    {
        return;
    }

    if(m_networkManager.scanIp(ip))
    {
        emit foundHost(ip);
    }

    m_mutex.lock();
    markHostAsScanned(index);
    qDebug()<<ip + " has been scaned.";
    checkIsScanFinished();
    m_mutex.unlock();
}

void ScanNetworkThread::checkIsScanFinished()
{
    unsigned int count = 0;
    for (unsigned int i=0; i<m_maxHostCount-1; ++i)
    {
        if (m_scannedIPAddresses[i] == m_hostScanned)
        {
            ++count;
        }
    }

    if (count == m_maxHostCount-2) //"m_maxHostCount-2" because minus this machine host address and the broadcast address
    {
        emit scanFinished();
    }
}

void ScanNetworkThread::markHostAsScanned (const unsigned int index)
{
    if (index<m_scannedIPAddresses.size())
    {
        m_scannedIPAddresses[index] = m_hostScanned;
    }
}

void ScanNetworkThread::markHostInProgress (const unsigned int index)
{
    if (index<m_scannedIPAddresses.size())
    {
        m_scannedIPAddresses[index] = m_hostInProgress;
    }
}
void ScanNetworkThread::markHostAsUnscanned (const unsigned int index)
{
    if (index<m_scannedIPAddresses.size())
    {
        m_scannedIPAddresses[index] = m_hostUnscanned;
    }
}
