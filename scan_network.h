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

class ScanNetwork: public QObject, public QRunnable
{
    Q_OBJECT
public:
    QList<int> m_scannedIPAddresses; //ToDo: move this list in the private section

public:
    ScanNetwork();

    void    run();
    int     scanIP(const QString &ip);

    //get
    QString getCurrentIP () const { return this->m_currentIP; }

    //set
    void    setCurrentIP (const QString &currentIP) { this->m_currentIP = currentIP; }

private:
    QString m_currentIP;
    QMutex  m_mutex;

private:
    void    checkFinish();

signals:
    void    foundComputer(const QString &ip);
    void    finishScan();
};

#endif // SCAN_NETWORK_H
