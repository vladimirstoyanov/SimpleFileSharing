#include "logger.h"

Logger::Logger()
{
}

void Logger::write(QString message, QString filename)
{
    QFile file(filename);
    file.open(QIODevice::Append | QIODevice::Text);
    QTextStream out(&file);
    out << message << '\n';

    file.close();
}

void Logger::writeHTMLSourceToFile(QString file_name,QString html_source)
{
    QFile file(file_name);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);
    out << html_source << '\n';

    file.close();
}
