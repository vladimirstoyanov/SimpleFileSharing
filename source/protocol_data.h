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

#ifndef PROTOCOL_DATA_H
#define PROTOCOL_DATA_H

#include <string>

#include <QByteArray>
#include <QDebug>
#include <QFile>

#include "message_codes.h"

//ToDo: refactor this class
class ProtocolData
{
public:
    explicit ProtocolData();
    virtual ~ProtocolData();

    void        fromChar(const char *);
    void        fromChar(const char *s, const char *arg, int cmd);

    //get
    QByteArray  getArguments () const  { return this->m_arguments; }
    qint64      getSize() const { return this->m_size; }
    char *      getString () const { return this->m_string; }
    int         getType () const { return this->m_type; }

private:
    QByteArray  m_arguments;
    qint64      m_size;
    char        *m_string;
    int         m_type;

};

#endif // PROTOCOL_DATA_H
