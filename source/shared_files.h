#ifndef SHARED_FILES_H
#define SHARED_FILES_H

#include <vector>

#include <QFile>

#include "file_data.h"
#include "parse_file_data.h"

class SharedFiles
{
public:
    explicit SharedFiles();
    virtual ~SharedFiles ();

    void addFile (const FileData &fileData);
    std::vector<FileData> get () const { return this->m_fileData; }
    void removeFile (const FileData &fileData);
    void update (const std::vector<FileData> &files);

private:
    bool load();
    bool save();

private:
    std::vector<FileData> m_fileData;
    QString               m_sharedFiles;
};

#endif // SHARED_FILES_H
