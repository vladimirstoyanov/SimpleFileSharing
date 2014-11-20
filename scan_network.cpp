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

scan_network::scan_network()
    : QRunnable()
{
    current_ip = "";
}

void scan_network::run()
{
    mutex.lock();
    if (current_ip == "")
    {
        mutex.unlock();
        return;
    }

    QString current_ip_tmp = current_ip;
    int index = -1;

    for (int i=0; i<is_scanned.size(); i++)
    {
        if (is_scanned[i] == 0)
        {
            is_scanned[i]=1;
            current_ip_tmp+=QString::number(i+1);
            index=i;
            break;
        }
    }

    mutex.unlock();

    if (index==-1)
        return;

    if(!scanIP(current_ip_tmp))
        emit foundComputer(current_ip_tmp);

    mutex.lock();

    is_scanned[index]=2;

    qDebug()<<current_ip_tmp + " has been scaned.";

    checkFinish();

    mutex.unlock();
}

//return 0: when found IP address
int scan_network::scanIP(QString ip)
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

    char *nc_hello = new char [3];
    nc_hello[0]=char(1);
    nc_hello[1]='\t';
    nc_hello[2]='\n';
    socket.write(nc_hello,3);

    delete nc_hello;

    QByteArray ba1;
    content ="";

    socket.waitForReadyRead();
    ba1=socket.readAll();

    while(socket.waitForReadyRead())
        ba1=socket.readAll();

    socket.close();
    socket.deleteLater();

    const char *recv = ba1.constData();
    if (!recv)
        return 1;

    if(strlen(recv)<3)
        return 1;

    if (recv[0]!=char(2) || recv[1]!='\t' || recv[2]!='\n')
        return 1;

    return 0;
}

void scan_network::checkFinish()
{
    int count = 0;
    for (int i=0; i<255; i++)
    {
        if (is_scanned[i] == 2)
            count++;
    }

    if (count == 254)
        emit finishScan();
}

