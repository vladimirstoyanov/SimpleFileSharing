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

Data::Data(){
    arguments = 0;
    string = 0;
    type = 0;
}

Data::~Data(){
    if(string)
        delete []string;
}

void Data::FromChar(const char *s){
    if(!s)
        return;

    if(string)
        delete []string;

    size = 0;
    type = 0;
    string = 0;
    arguments = "";

    int lenPos = 0;
    for(int i=0; i<strlen(s); i++)
    {
        if(s[i]=='\t') //get type
        {
            if (i!=1)
                return;
            lenPos = 2;
            type = int(s[0]); //fist byte is a type
        }

        if(s[i]=='\n') //get arguments
        {
            QByteArray bLen;
            for(int j=lenPos; j<i;j++)
                bLen += s[j];
            bLen += '\0';
            lenPos = i+1;
            if (strlen(bLen.constData())>0)
                arguments = bLen;
            break;
        }
    }

    //get data
    if (strlen(s)-lenPos>0)
    {
        string = new char[(strlen(s)-lenPos) + 1];
        for (int i=lenPos, j=0; i<strlen(s); i++, j++)
            string[j]=s[i];
    }

}

void Data::FromChar(const char *s, const char *arg, int cmd)
{
    if(string)
        delete []string;

    type = 0;

    if (arg && s)
        string = new char[4+strlen(arg) + strlen(s)];
    else if(arg)
        string = new char[4+strlen(arg)];
    else if (s)
        string = new char[4+strlen(s)];
    else
        string = new char[4];

    size=3;
    string[0] = cmd;
    string[1] = '\t';

    //argument
    int j=2;
    if (arg)
    {
        for (int i=0; i<strlen(arg); i++)
        {
            string[j++] = arg[i];
            size++;
        }
    }
    string[j++] = '\n';

    //data
    if (s)
    {
        for (int i=0; i<strlen(s); i++)
        {
            string[j++] = s[i];
            size++;
        }
    }
}

//return hash of file with name recorded in 'file_name' var
QByteArray Data::getHash(QString file_name)
{
    QCryptographicHash crypto(QCryptographicHash::Sha1);
    QFile file(file_name);
    if (!file.open(QFile::ReadOnly))
        return "";
    while(!file.atEnd()){
      crypto.addData(file.read(8192));
    }
    return crypto.result().toHex();
}
