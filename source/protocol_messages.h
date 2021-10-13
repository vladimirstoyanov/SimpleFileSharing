#ifndef PROTOCOLMESSAGES_H
#define PROTOCOLMESSAGES_H

#include <QByteArray>
#include <QString>

#include "file_data.h"
#include "file_operations.h"

class ProtocolMessages
{
public:
    ProtocolMessages();

    QString    generateDownloadFileMessage (const QString &fileName, const QString &id);
    QString    generateSharedFilesListMessage (const std::vector<FileData> &files);
    QByteArray getFileListMessage () const { return m_getListMessage; }
    QByteArray getHelloMessage () const { return m_helloMessage; }
    QByteArray getHelloMessageReceived () const { return m_helloMessageReceived; }
    QByteArray getListMessageReceived () const { return m_listMessageReceived; }

private:
    QByteArray m_helloMessage;
    QByteArray m_helloMessageReceived;
    QByteArray m_getListMessage;
    QByteArray m_listMessageReceived;

};

#endif // PROTOCOLMESSAGES_H
