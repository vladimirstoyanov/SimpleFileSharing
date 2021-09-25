#ifndef PARSENETWORKMESSAGE_H
#define PARSENETWORKMESSAGE_H

#include <vector>

#include <QString>

#include "remote_host_file_data.h"

class ParseNetworkMessage
{
public:
    ParseNetworkMessage();

    std::vector<RemoteHostFileData> parseGetListResultMessage (const QString &resultMessage);
};

#endif // PARSENETWORKMESSAGE_H
