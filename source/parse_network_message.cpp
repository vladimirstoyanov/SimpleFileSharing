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

    QStringList pieces = resultMessage.split( "\n" );

    for (int i=1; i<pieces.length(); ++i) //first line is skipped (message protocol)
    {
        QStringList line = resultMessage.split("#");
        if (line.length()!=CODE_LENGTH)
        {
            //ToDo: return an error
            return remoteHostFileDataList;
        }
        QString size = line.at(0);
        QString id = line.at(1);
        QString fileName = line.at(2);
        RemoteHostFileData remoteHostFileData (id, fileName,size, "");
        remoteHostFileDataList.push_back(remoteHostFileData);
    }

    return remoteHostFileDataList;
}
