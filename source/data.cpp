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

#include "data.h"

Data::Data():
    m_arguments (nullptr)
    , m_Fl (0)
    , m_size (0)
    , m_string (nullptr)
    , m_type (0)
{
}

Data::~Data(){
    if(m_string)
    {
        delete []m_string;
    }
}

void Data::fromChar(const char *s)
{
    if(!s)
    {
        return;
    }

    if(m_string)
    {
        delete []m_string;
    }

    m_size = 0;
    m_type = 0;
    m_string = nullptr;
    m_arguments = "";

    unsigned int lenPos = 0;
    for(unsigned int i=0; i<strlen(s); i++)
    {
        if(s[i]=='\t') //get type
        {
            if (i!=1)
                return;
            lenPos = 2;
            m_type = int(s[0]); //fist byte is a type
        }

        if(s[i]=='\n') //get arguments
        {
            QByteArray bLen;
            for(unsigned int j=lenPos; j<i;j++)
            {
                bLen += s[j];
            }
            bLen += '\0';
            lenPos = i+1;
            if (strlen(bLen.constData())>0)
            {
               m_arguments = bLen;
            }
            break;
        }
    }

    //get data
    if (strlen(s)-lenPos>0)
    {
        m_string = new char[(strlen(s)-lenPos) + 1];
        for (unsigned int i=lenPos, j=0; i<strlen(s); i++, j++)
        {
            m_string[j]=s[i];
        }
    }

}

void Data::fromChar(const char *s, const char *arg, int cmd)
{
    if(m_string)
    {
        delete []m_string;
    }

    m_type = 0;

    if (arg && s)
    {
        m_string = new char[4+strlen(arg) + strlen(s)];
    }
    else if(arg)
    {
        m_string = new char[4+strlen(arg)];
    }
    else if (s)
    {
        m_string = new char[4+strlen(s)];
    }
    else
    {
        m_string = new char[4];
    }

    m_size=3;
    m_string[0] = cmd;
    m_string[1] = '\t';

    //argument
    int j=2;
    if (arg)
    {
        for (unsigned int i=0; i<strlen(arg); i++)
        {
            m_string[j++] = arg[i];
            m_size++;
        }
    }
    m_string[j++] = '\n';

    //data
    if (s)
    {
        for (unsigned int i=0; i<strlen(s); i++)
        {
            m_string[j++] = s[i];
            m_size++;
        }
    }
}

QByteArray Data::getHash(const QString &fileName)
{
    int chunkSize = 8192;
    QCryptographicHash crypto(QCryptographicHash::Sha1);
    QFile file(fileName);

    if (!file.open(QFile::ReadOnly))
    {
        return "";
    }

    while(!file.atEnd())
    {
      crypto.addData(file.read(chunkSize));
    }

    return crypto.result().toHex();
}
