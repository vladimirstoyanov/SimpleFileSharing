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
    QByteArray getHelloMessage () const { return m_helloMessage; }
    QByteArray getReceiveListMessage () const { return m_receiveListMessage; }

private:
    QByteArray m_helloMessage;
    QByteArray m_receiveListMessage;
};

#endif // PROTOCOLMESSAGES_H
