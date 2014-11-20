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

#include <QObject>
#include <QtCore>
#include <QDebug>
#include <QTcpSocket>
#include <QThreadPool>
#include <QThread>
#include <QRunnable>
#include <QList>
#include <QMutex>

class scan_network: public QObject, public QRunnable
{
    Q_OBJECT
public:
    scan_network();
    QString current_ip;
    QList<int> is_scanned;
    QMutex mutex;
    void run();
    int scanIP(QString ip);
private:
    void checkFinish();
signals:
    void foundComputer(QString ip);
    void finishScan();
};

#endif // SCAN_NETWORK_H
