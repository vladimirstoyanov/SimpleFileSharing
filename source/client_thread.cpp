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

#include "client_thread.h"
#include <QDataStream>

ClientThread::ClientThread(const QString &hostAdress,
        const QString &query,
        const QString &fileName,
        const QString &fileDir,
        const qint64 size,
        const int rowId):
    m_rowId(rowId)
    , m_hostIp(hostAdress)
    , m_fileDir (fileDir)
    , m_fileName(fileName)
    , m_query(query)
    , m_fileSize (size)
{
}

ClientThread::~ClientThread()
{
    if (isRunning())
    {
        quit();
    }
    wait();
}

void ClientThread::run()
{
    NetworkManager networkManager;
    QString filePath = m_fileDir + m_fileName;
    auto downlaodProgress = std::bind(&ClientThread::downloadProgressCallback,this, 0);
    networkManager.downloadFile(m_hostIp, filePath, m_query, m_fileSize, downlaodProgress);
    emit clientThreadFinished(m_rowId);
}

void ClientThread::downloadProgressCallback (int percentage)
{
   emit setProgress(m_rowId, percentage);
}
