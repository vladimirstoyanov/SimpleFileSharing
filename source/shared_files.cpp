#include "shared_files.h"

SharedFiles::SharedFiles():
    m_sharedFiles("sharedFiles")
{
    load();
}

SharedFiles::~SharedFiles()
{
    save();
}

void SharedFiles::addFile (const FileData &fileData)
{
    m_fileData.push_back(fileData);
}

void SharedFiles::removeFile (const FileData &fileData)
{
    std::remove_if(m_fileData.begin(),
               m_fileData.end(),
               [&](const FileData & item)
               {
                    return (item == fileData);
               });
}

bool SharedFiles::load()
{
    ParseFileData parseFileData;

    QFile file(m_sharedFiles);
    if(!file.open(QIODevice::ReadOnly))
    {
        return false;
    }

    QTextStream in(&file);

    while(!in.atEnd())
    {
        QString fileName = "", path = "", size = "";
        if (!parseFileData.parseField(in, fileName, path, size))
        {
            return false;
        }
        FileData fileData (fileName, path, size);
        m_fileData.push_back(fileData);
    }
    file.close();

    return true;
}

bool SharedFiles::save()
{
    QFile file(m_sharedFiles);

    if (file.open(QIODevice::ReadWrite | QIODevice::Truncate))
    {
        QTextStream stream( &file );
        for (const auto &item: m_fileData)
        {
            stream<<"<file>\n\t<file_name>" + item.getFileName() + "</file_name>\n\t";
            stream<<"<path>" + item.getPath()+ "</path>\n\t";
            stream<<"<size>" + item.getSize() + "</size>\n</file>\n";
        }
       file.flush();
       file.close();
       return true;
    }

    return false;
}

void SharedFiles::update (const std::vector<FileData> &files)
{
    m_fileData.clear(); //remove odl files
    for (auto &item: files)
    {
        m_fileData.push_back(item);
    }
}
