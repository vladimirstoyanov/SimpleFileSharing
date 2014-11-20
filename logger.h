#ifndef LOGGER_H
#define LOGGER_H
#include <QFile>
#include <QDateTime>
#include <QTextStream>
class Logger
{
public:
    Logger();
    static void write(QString message, QString filename);
    void writeHTMLSourceToFile(QString file_name, QString html_source);
};

#endif // LOGGER_H
