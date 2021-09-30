#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QCryptographicHash>
#include <QFile>
#include <QFileInfo>
#include <QString>

#include "message_codes.h"

//ToDo: add file read/write methods
class FileOperations
{
public:
    FileOperations();

    QByteArray getFileHash(const QString &fileName);
    size_t getFileSize (const QString &filePath);
};

#endif // FILEMANAGER_H
