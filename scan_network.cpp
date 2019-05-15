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

#include "scan_network.h"

ScanNetwork::ScanNetwork()
    : QRunnable()
    , m_currentIP ("")
    , m_mutex ()
{

}

void ScanNetwork::run()
{
    m_mutex.lock();
    if (m_currentIP == "")
    {
        m_mutex.unlock();
        return;
    }

    QString currentIpTmp = m_currentIP;
    int index = -1;

    for (int i=0; i<m_scannedIPAddresses.size(); i++)
    {
        if (m_scannedIPAddresses[i] == 0)
        {
            m_scannedIPAddresses[i]=1;
            currentIpTmp+=QString::number(i+1);
            index=i;
            break;
        }
    }

    m_mutex.unlock();

    if (index==-1)
    {
        return;
    }

    if(!scanIP(currentIpTmp))
    {
        emit foundComputer(currentIpTmp);
    }

    m_mutex.lock();

    m_scannedIPAddresses[index]=2;

    qDebug()<<currentIpTmp + " has been scaned.";

    checkFinish();

    m_mutex.unlock();
}

//return 0: when found IP address
int ScanNetwork::scanIP(const QString &ip)
{
    QTcpSocket socket;
    QString content = "";

    socket.connectToHost(ip,26001);
    bool flag = socket.waitForConnected();
    if(!flag)
    {
        socket.close();
        socket.deleteLater();

        return 1;
    }

    char *ncHello = new char [3];
    ncHello[0]=char(1);
    ncHello[1]='\t';
    ncHello[2]='\n';
    socket.write(ncHello,3);

    delete ncHello;

    QByteArray ba1;
    content ="";

    socket.waitForReadyRead();
    ba1=socket.readAll();

    while(socket.waitForReadyRead())
    {
        ba1=socket.readAll();
    }

    socket.close();
    socket.deleteLater();

    const char *recv = ba1.constData();
    if (!recv)
    {
        return 1;
    }

    if(strlen(recv)<3)
    {
        return 1;
    }

    if (recv[0]!=char(2) || recv[1]!='\t' || recv[2]!='\n')
    {
        return 1;
    }

    return 0;
}

void ScanNetwork::checkFinish()
{
    int count = 0;
    for (int i=0; i<255; i++)
    {
        if (m_scannedIPAddresses[i] == 2)
            count++;
    }

    if (count == 254)
        emit finishScan();
}

