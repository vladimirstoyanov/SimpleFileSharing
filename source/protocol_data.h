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

class ProtocolData
{
public:
    explicit ProtocolData();
    virtual ~ProtocolData();


    QByteArray getArguments () const { return this->m_arguments; }
    char getMessageCode () const { return this->m_messageCode; }

    void setMessageCode (const char messageCode) { m_messageCode = messageCode; }
    void setArguments (const QByteArray &arguments) { m_arguments = arguments; }
private:
    char        m_messageCode;
    QByteArray  m_arguments;
};

#endif // PROTOCOL_DATA_H
