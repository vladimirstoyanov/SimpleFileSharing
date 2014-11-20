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

#ifndef DATA_H
#define DATA_H

#include <QDebug>
#include <string>
#include <QByteArray>
#include <QCryptographicHash>
#include <QFile>


class Data
{
public:
    int type;
    qint64 size;
    QByteArray arguments;
    char *string;
    char Fl;

    explicit Data();
    ~Data();

    void FromChar(const char *);
    void FromChar(const char *s, const char *arg, int cmd);
    QByteArray getHash(QString file_name);
};

#endif // DATA_H
