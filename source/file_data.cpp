#include "file_data.h"


FileData::FileData (const QString &fileId,
                    const QString &fileName,
                    const QString &path,
                    const QString &size):
    m_fileId (fileId),
    m_fileName(fileName),
    m_path (path),
    m_size (size)
{

}


