#include "protocol_messages.h"

ProtocolMessages::ProtocolMessages():
      m_helloMessage("\x01\t\n")
    , m_helloMessageReceived("\x02\t\n")
    , m_getListMessage ("\x14\t\n")
    , m_listMessageReceived("\x28\t\n")
{

}

QString ProtocolMessages::generateDownloadFileMessage (const QString &fileName, const QString & id)
{
     QString query = "\x10\t";
     query+= id + "#" + fileName+"\n";
     return query;
}

QString ProtocolMessages::generateSharedFilesListMessage (const std::vector<FileData> &files)
{
    QString sharedFiles = m_listMessageReceived;
    FileOperations fileOperations;
    for (unsigned int i=0; i<files.size(); ++i)
    {
        sharedFiles+=QString::number(fileOperations.getFileSize(files[i].getPath()));
        sharedFiles+= "#";
        sharedFiles+=files[i].getFileId();
        sharedFiles+="#";
        sharedFiles+=files[i].getFileName();
        sharedFiles+="\n";
    }

    return sharedFiles;
}
