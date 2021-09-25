#include "parse_network_message.h"

ParseNetworkMessage::ParseNetworkMessage()
{

}


std::vector<RemoteHostFileData> ParseNetworkMessage::parseGetListResultMessage (const QString &resultMessage)
{
    std::vector<RemoteHostFileData> remoteHostFileDataList;
    if (resultMessage == "" || resultMessage.length()<3)
    {
        return remoteHostFileDataList;
    }

    int i=2;
    QString text ="";
    QString size ="";
    QString name ="";
    do
    {
        ++i;
        if (resultMessage[i]=='#') //hash
        {
            size = text;
            text="";
            continue;
        }
        if (resultMessage[i]=='\n') //name
        {
            name = text;
            RemoteHostFileData remoteHostFileData(name, size, "");
            remoteHostFileDataList.push_back(remoteHostFileData);
            name="";
            size="";
            text="";
            continue;
        }
        text+=resultMessage[i];
    }
    while(i<resultMessage.length());

    return remoteHostFileDataList;
}
