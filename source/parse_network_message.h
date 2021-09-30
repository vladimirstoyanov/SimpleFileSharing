#ifndef PARSENETWORKMESSAGE_H
#define PARSENETWORKMESSAGE_H

#include <vector>

#include <QString>

#include "message_codes.h"
#include "protocol_data.h"
#include "remote_host_file_data.h"

class ParseNetworkMessage
{
public:
    ParseNetworkMessage();

    std::vector<RemoteHostFileData> parseGetListResultMessage (const QString &resultMessage);
    ProtocolData parseProtocolData (const QByteArray & byteArray);
};

#endif // PARSENETWORKMESSAGE_H
