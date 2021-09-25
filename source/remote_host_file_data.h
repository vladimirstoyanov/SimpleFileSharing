#ifndef REMOTEHOSTFILEDATA_H
#define REMOTEHOSTFILEDATA_H

#include <QString>

class RemoteHostFileData
{
public:
    explicit RemoteHostFileData(const QString &fileName, const QString &size, const QString &hash);
    virtual ~RemoteHostFileData () = default;

    QString getFileName () const { return this->m_fileName; }
    QString getSize () const { return this->m_size; }
    QString getHash () const { return this->m_hash; }

private:
    QString m_fileName;
    QString m_size;
    QString m_hash;
};

#endif // REMOTEHOSTFILEDATA_H
