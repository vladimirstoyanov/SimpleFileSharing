#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QCryptographicHash>
#include <QFile>
#include <QString>

#include "message_codes.h"

class FileOperations
{
public:
    FileOperations();

    QByteArray getFileHash(const QString &fileName);
};

#endif // FILEMANAGER_H
