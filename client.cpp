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

#include "client.h"
#include <QDataStream>

Client::Client(const QString hostadress, const QString query_, const QString file_name_, const QString file_dir_,  qint64 size, int index_)
{
    host_ip = hostadress;
    query = query_;
    file_name = file_name_;
    file_dir = file_dir_;
    file_size = size;
    row = index_;
}
Client::~Client()
{
}

void Client::run()
{
    QTcpSocket socket;
    Data data;
    socket.connectToHost(host_ip,26001);
    double percentage = 0;
    emit setProgress(row, 0);

    if(!socket.waitForConnected())
    {
        socket.close();
        return;
    }

    QByteArray ba2;
    ba2 = query.toLocal8Bit(); //query to get hash of checked file
    socket.write(ba2);
    socket.waitForReadyRead();

    QByteArray ba1;
    ba1=socket.readAll();

    //get hash
    QString hash="";
    int j=0;
    for (j=0; j<ba1.size(); j++)
    {
        if (ba1[j]=='#')
            break;
        hash+=ba1[j];
    }
    ba1 = ba1.remove(0,j+1);
    QFile f(file_dir + file_name);

    if (!f.open(QIODevice::WriteOnly))
    {
        qDebug()<<"Client::run(): Can't open " + file_dir + file_name + ".";
        socket.close();
        return;
    }

    if (ba1.size()>0)
        f.write(ba1);

    int b = 0;

    while(socket.waitForReadyRead())
    {
        ba1=socket.readAll();
        if (file_size>b)
        {
            b+=ba1.size();
            double a = b/(file_size*1.0);
            percentage = 100*a;
            //qDebug()<<QString::number(procentage) + "% " +QString::number(a) + " " + QString::number(file_size);
            if (percentage != 100)
                emit setProgress(row, percentage);
        }

        f.write(ba1);
    }

    f.close();
    socket.close();
    if (hash!=data.getHash(file_dir + file_name).data())
    {
        emit setProgress(row, -1);
        qDebug()<<file_dir + file_name + " failed to download.";
        qDebug()<<"hash:"<<hash;
        qDebug()<<"getHash:"<<data.getHash(file_dir + file_name).data();
    }
    else
        emit setProgress(row, 100);
}
