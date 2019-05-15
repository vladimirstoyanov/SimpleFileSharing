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

Client::Client(const QString &hostAdress,
        const QString &query,
        const QString &fileName,
        const QString &fileDir,
        const qint64 size,
        const int index):
    m_row(index)
    , m_hostIp(hostAdress)
    , m_fileDir (fileDir)
    , m_fileName(fileName)
    , m_fileSize (size)
    , m_query(query)
{
}

Client::~Client()
{
}

void Client::run()
{
    QTcpSocket socket;
    Data data;
    socket.connectToHost(m_hostIp, 26001);
    double percentage = 0;
    emit setProgress(m_row, 0);

    if(!socket.waitForConnected())
    {
        socket.close();
        return;
    }

    QByteArray ba2;
    ba2 = m_query.toLocal8Bit(); //query to get hash of checked file
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
        {
            break;
        }
        hash+=ba1[j];
    }
    ba1 = ba1.remove(0,j+1);
    QFile f(m_fileDir + m_fileName);

    if (!f.open(QIODevice::WriteOnly))
    {
        qDebug()<<"Client::run(): Can't open " + m_fileDir + m_fileName + ".";
        socket.close();
        return;
    }

    if (ba1.size()>0)
    {
        f.write(ba1);
    }

    int b = 0;

    while(socket.waitForReadyRead())
    {
        ba1=socket.readAll();
        if (m_fileSize>b)
        {
            b+=ba1.size();
            double a = b/(m_fileSize*1.0);
            percentage = 100*a;
            //qDebug()<<QString::number(procentage) + "% " +QString::number(a) + " " + QString::number(file_size);
            if (percentage != 100)
            {
                emit setProgress(m_row, percentage);
            }
        }

        f.write(ba1);
    }

    f.close();
    socket.close();

    if (hash!=data.getHash(m_fileDir + m_fileName).data())
    {
        emit setProgress(m_row, -1);
        qDebug()<<m_fileDir + m_fileName + " failed to download.";
        qDebug()<<"hash: "<<hash;
        qDebug()<<"getHash: "<<data.getHash(m_fileDir + m_fileName).data();
    }
    else
    {
        emit setProgress(m_row, 100);
    }
}
