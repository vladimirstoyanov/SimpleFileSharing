#include "file_operations.h"

FileOperations::FileOperations()
{

}


QByteArray FileOperations::getFileHash(const QString &fileName)
{
    QCryptographicHash crypto(QCryptographicHash::Sha1);
    QFile file(fileName);

    if (!file.open(QFile::ReadOnly))
    {
        return "";
    }

    while(!file.atEnd())
    {
      crypto.addData(file.read(BUFFER_SIZE));
    }

    return crypto.result().toHex();
}
