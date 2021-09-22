#ifndef PARSEFILEDATA_H
#define PARSEFILEDATA_H

#include <QString>
#include <QTextStream>

#include "search.h"

class ParseFileData
{
public:
    ParseFileData();
    bool parseField (QTextStream & in, QString &fileName, QString &path, QString &size);

private:
    bool isElementExist (const QString &line, const QString &element);
    bool parseData (const QString &line, const QString &element, QString &result);

};

#endif // PARSEFILEDATA_H
