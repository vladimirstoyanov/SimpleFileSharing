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

#ifndef __SEARCH_CPP__
#define __SEARCH_CPP__

#include "search.h"
#include <string>
#include <vector>
using namespace std;

Search::Search()
{

}

Search::~Search()
{

}

bool Search::searchBefore(const QString &text, const QString &subtext, unsigned int &index)
{
    vector<int> T(subtext.length() + 1, -1);

    if(subtext.length() == 0 && subtext.length()>text.length())
    {
        index = 0;
        return false;
    }
    for(int i = 1; i <= subtext.length(); i++)
    {
        int pos = T[i - 1];
        while(pos != -1 && subtext[pos] != subtext[i - 1]) pos = T[pos];
            T[i] = pos + 1;
    }

    int sp = index;
    int kp = 0;
    while(sp < text.length())
    {
        while(kp != -1 && (kp == subtext.length() || subtext[kp] != text[sp]))
        {
            kp = T[kp];
        }
        kp++;
        sp++;
        if(kp == subtext.length())
        {
            index = sp - subtext.length();
            return true;
        }
    }

    index = 0;
    return false;
}
bool Search::searchAfter(const QString &text, const QString &subtext, unsigned int &index)
{
    vector<int> T(subtext.length() + 1, -1);

    if(subtext.length() == 0 && subtext.length()>text.length())
    {
        index = 0;
        return false;
    }
    for(int i = 1; i <= subtext.length(); i++)
    {
        int pos = T[i - 1];
        while(pos != -1 && subtext[pos] != subtext[i - 1]) pos = T[pos];
            T[i] = pos + 1;
    }

    int sp = index;
    int kp = 0;
    while(sp < text.length())
    {
        while(kp != -1 && (kp == subtext.length() || subtext[kp] != text[sp]))
        {
            kp = T[kp];
        }
        kp++;
        sp++;
        if(kp == subtext.length())
        {
            //matches.push_back(sp - K.size());
            index = sp;
            return true;
        }
    }

    index = 0;
    return false;
}

int  Search::getTextBetweenTwoStrings(const QString &line, const QString &str1, const QString &str2, QString &text)
{
    unsigned int index=0, index1= 0;
    if (!searchAfter(line, str1, index))
    {
        return 1;
    }

    if (!searchBefore(line,str2, index1))
    {
        return 1;
    }
    if (index1<index)
    {
        return 1;
    }

    for(unsigned int i=index; i<index1; i++)
    {
        text+=line[i];
    }

    return 0;
}

#endif
