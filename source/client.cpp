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
    , m_query(query)
    , m_fileSize (size)
{
}

Client::~Client()
{
}

void Client::run()
{
    //ToDo: update file download progress
    NetworkManager networkManager;
    auto fp = std::bind(&Client::downloadProgressCallback,this, 0);
    networkManager.downloadFile(m_hostIp, m_fileName, m_fileDir, m_query, m_fileSize, fp);
}

void Client::downloadProgressCallback (int percentage)
{
   emit setProgress(m_row, percentage);
}
