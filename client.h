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

#ifndef CLIENT_H
#define CLIENT_H

#include <QThread>
#include <QTcpSocket>
#include <QDebug>
#include <QQueue>
#include <QHostInfo>
#include <QFileInfo>
#include "data.h"



class Client: public QThread
{
    Q_OBJECT
protected:
    void run();

public:
    Client(const QString HostAdress, const QString query_, const QString file_name, const QString file_dir, qint64 size, int index_);
    ~Client();
private:
    QString host_ip;
    QString query;
    QString file_name;
    QString file_dir;
    qint64 file_size;
    int row;
signals:
    void setProgress(int,double);
};

#endif // CLIENT_H
