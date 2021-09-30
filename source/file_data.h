#ifndef FILEDATA_H
#define FILEDATA_H

#include <QString>

#include "message_codes.h"

class FileData
{
public:
    FileData (const QString &fileName, const QString &path, const QString &size);

    QString getFileName () const { return this->m_fileName; }
    QString getPath () const { return this->m_path; }
    QString getSize () const { return this->m_size; }

    bool operator == (const FileData &obj) const
    {
        return (this->m_fileName == obj.m_fileName &&
                this->m_path == obj.m_path &&
                this->m_size == obj.m_size);
    }

private:
    QString m_fileName;
    QString m_path;
    QString m_size;
};

#endif // FILEDATA_H
