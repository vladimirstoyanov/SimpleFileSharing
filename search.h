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

#ifndef SEARCH_H
#define SEARCH_H
#include <QString>

class CSearch {
    public:
        CSearch();
        ~CSearch();

        void Search_Before(QString text, QString subtext, int *index);
        void Search_After(QString text, QString subtext, int *index);
        int getTextBetweenTwoStrings(QString line, QString str1, QString str2, QString &text); //get text between 'str1' and 'str2' from 'line' and set it to 'text' variable

};

#endif // SEARCH_H
