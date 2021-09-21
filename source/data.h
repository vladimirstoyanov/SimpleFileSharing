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

#include <string>

#include <QByteArray>
#include <QCryptographicHash>
#include <QDebug>
#include <QFile>

class Data
{
public:
    explicit Data();
    virtual ~Data();

    QByteArray  getHash(const QString &fileName);
    void        fromChar(const char *);
    void        fromChar(const char *s, const char *arg, int cmd);

    //get
    QByteArray  getArguments () const  { return this->m_arguments; }
    qint64      getSize() const { return this->m_size; }
    char *      getString () const { return this->m_string; }
    int         getType () const { return this->m_type; }

    //set
    void        setArgument (const QByteArray &arguments) { this->m_arguments = arguments; }
    void        setSize(qint64 size) { this->m_size = size; }
    void        setString (char *string) { this->m_string = string; }
    void        setType (int type ){ this->m_type = type; }

private:
    QByteArray  m_arguments;
    char        m_Fl;
    qint64      m_size;
    char        *m_string; //ToDo: use QString, std::string or ByteArray instead char * and change the name
    int         m_type;

};

#endif // DATA_H
