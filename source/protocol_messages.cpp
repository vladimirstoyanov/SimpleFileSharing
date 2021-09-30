#include "protocol_messages.h"

ProtocolMessages::ProtocolMessages():
    m_helloMessage("\x02\t\n")
    , m_receiveListMessage("\x28\t\n")
{

}


QString ProtocolMessages::getFileMessage (const QString &fileName, const QString & numberOfFile)
{
     QString query = "\x10\t";
     query+=numberOfFile + "#" + fileName+"\n";
     return query;
}
