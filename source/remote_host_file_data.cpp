#include "remote_host_file_data.h"

RemoteHostFileData::RemoteHostFileData(const QString &fileName, const QString &size, const QString &hash):
    m_fileName(fileName)
   , m_size (size)
   , m_hash (hash)
{

}
