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

class Search {
    public:
        Search();
        virtual ~Search();

        bool searchBefore(const QString &text, const QString &subtext, unsigned int &index);
        bool searchAfter(const QString &text, const QString &subtext, unsigned int &index);
        int  getTextBetweenTwoStrings(const QString &line, const QString &str1, const QString &str2, QString &text);

};

#endif // SEARCH_H
