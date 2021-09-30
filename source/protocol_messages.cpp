#include "protocol_messages.h"

ProtocolMessages::ProtocolMessages():
    m_helloMessage("\x02\t\n")
    , m_receiveListMessage("\x28\t\n")
{

}

QString ProtocolMessages::getDownloadFileMessage (const QString &fileName, const QString & id)
{
     QString query = "\x10\t";
     query+= id + "#" + fileName+"\n";
     return query;
}
