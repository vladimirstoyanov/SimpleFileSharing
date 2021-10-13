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

#ifndef CLIENT_THREAD_H
#define CLIENT_THREAD_H

#include <functional>

#include <QDebug>
#include <QFileInfo>
#include <QHostInfo>
#include <QQueue>
#include <QThread>
#include <QTcpSocket>

#include "protocol_data.h"
#include "network_manager.h"

class ClientThread: public QThread
{
    Q_OBJECT
protected:
    void run();

public:
    ClientThread(
            const QString &hostAdress,
            const QString &query,
            const QString &fileName,
            const QString &fileDir,
            const qint64 size,
            const int rowId);

    virtual ~ClientThread();
    int getRowId () const { return m_rowId; }

private:
    int     m_rowId;
    QString m_hostIp;
    QString m_fileDir;
    QString m_fileName;
    QString m_query;
    qint64  m_fileSize;

    void downloadProgressCallback (int percentage);


signals:
    void setProgress(const int, const double);
    void clientThreadFinished (int);
};

#endif // CLIENT_THREAD_H
