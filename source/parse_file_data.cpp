#include "parse_file_data.h"

ParseFileData::ParseFileData()
{

}

bool ParseFileData::parseData (const QString &line, const QString &element, QString &result)
{
    Search search;
    result = "";
    if (search.getTextBetweenTwoStrings(line, "<" + element + ">", "</" + element + ">", result))
    {
        return false;
    }
    return true;
}

bool ParseFileData::isElementExist (const QString &line, const QString &element)
{
    unsigned int index=0;
    Search search;

    search.searchAfter(line, element, index);
    if (index != element.length())
    {
        return false;
    }

    return true;
}

bool ParseFileData::parseField (QTextStream & in, QString &id, QString &fileName, QString &path, QString &size)
{
    if (isElementExist (in.readLine(), "<file>") &&
        parseData (in.readLine(), "id", id) &&
        parseData (in.readLine(), "file_name", fileName) &&
        parseData (in.readLine(), "path", path) &&
        parseData (in.readLine(), "size", size)&&
        isElementExist (in.readLine(), "</file>"))
    {
            return true;
    }
    return false;
}
