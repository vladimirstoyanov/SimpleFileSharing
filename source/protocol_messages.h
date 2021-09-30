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

    QString    getDownloadFileMessage (const QString &fileName, const QString &id);
    QByteArray getHelloMessage () const { return m_helloMessage; }
    QByteArray getReceiveListMessage () const { return m_receiveListMessage; }
    QString    getSharedFilesListMessage (const std::vector<FileData> &files);

private:
    QByteArray m_helloMessage;
    QByteArray m_receiveListMessage;
};

#endif // PROTOCOLMESSAGES_H
