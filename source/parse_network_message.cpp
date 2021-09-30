#include "parse_network_message.h"

ParseNetworkMessage::ParseNetworkMessage()
{

}

ProtocolData ParseNetworkMessage::parseProtocolData (const QByteArray & byteArray)
{
    ProtocolData protocolData;
    if(byteArray.length()<CODE_LENGTH)
    {
        return protocolData;
    }

    protocolData.setMessageCode(byteArray[0]);

    int index =-1;
    for (int i=0; i<byteArray.length(); ++i)
    {
        if('\n' == byteArray[i])
        {
            index = i;
            break;
        }
    }
    if (-1 == index)
    {
        return protocolData;
    }

    protocolData.setArguments (byteArray.mid(1, index));
    return protocolData;
}

std::vector<RemoteHostFileData> ParseNetworkMessage::parseGetListResultMessage (const QString &resultMessage)
{
    std::vector<RemoteHostFileData> remoteHostFileDataList;
    if (resultMessage == "" || resultMessage.length()<CODE_LENGTH)
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
