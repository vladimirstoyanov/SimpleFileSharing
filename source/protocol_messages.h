#ifndef PROTOCOLMESSAGES_H
#define PROTOCOLMESSAGES_H

#include <QByteArray>
#include <QString>

class ProtocolMessages
{
public:
    ProtocolMessages();

    QByteArray getReceiveListMessage () const { return m_receiveListMessage; }
    QByteArray getHelloMessage () const { return m_helloMessage; }

    QString    getDownloadFileMessage (const QString &fileName, const QString &id);



private:
    QByteArray m_helloMessage;
    QByteArray m_receiveListMessage;
};

#endif // PROTOCOLMESSAGES_H
