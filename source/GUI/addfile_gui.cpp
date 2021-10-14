/*
    This file is part of Simple File Sharing.

    Simple File Sharing is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Simple File Sharing is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Simple File Sharing.  If not, see <http://www.gnu.org/licenses/>.
*/


#include <QFileInfo>

#include <math.h>

#include "GUI/addfile_gui.h"
#include "search.h"
#include "ui_addfile_gui.h"

AddFile_GUI::AddFile_GUI(std::shared_ptr<SharedFiles> sharedFiles, QWidget *parent) :
    QDialog(parent)
    , m_addFileLastDir("")
    , m_fileNameId (0)
    , m_model (std::make_shared<QStandardItemModel>(0,3,this))
    , m_offsetBetweenWidgets(5)
    , m_pathId (1)
    , m_sharedFiles (sharedFiles)
    , m_sizeId (2)
    , m_ui(std::make_shared<Ui::AddFile_GUI> ())
{
    m_ui->setupUi(this);
    initModelTableView();
}

AddFile_GUI::~AddFile_GUI()
{
}

//"Add" button
void AddFile_GUI::on_addButton_clicked()
{
    QStringList pathList = QFileDialog::getOpenFileNames(this, tr("Share File(s)"), m_addFileLastDir, tr("Files (*.*)"));

    if (pathList.empty())
    {
        return;
    }

    QString dirNameTmp = "";
    if (!getDirNameByAbsolutePath(pathList[0], dirNameTmp))
    {
        m_addFileLastDir = dirNameTmp;
    }

    for (int i=0; i<pathList.size(); ++i)
    {
        QString filename="";
        if (getFileNameByAbsolutePath(pathList[i],filename))
        {
            qDebug()<<"Error path: " + pathList[i];
            continue;
        }
        QFileInfo fi(pathList[i]);
        double size = fi.size()/(1024.0*1024.0); //calculate size in KB
        FileData fileData (QString::number(i), filename, pathList[i],  QString::number(size));
        addToTableView(fileData);
    }
}

int AddFile_GUI::getDirNameByAbsolutePath(const QString &path, QString &dirname)
{
    QString separator;
    separator = "/";

    int index=-1;
    for (int i=path.length()-1; i>=0; --i)
    {

        if (path[i] == separator[0])
        {
            index = i;
            break;
        }
    }
    if (index == -1)
    {
        return 1;
    }

    for (int i=0; i<=index; ++i)
    {
        dirname+=path[i];
    }

    return 0;
}

void AddFile_GUI::initModelTableView()
{
    m_model->setHorizontalHeaderItem(m_fileNameId, new QStandardItem(QString("file name")));
    m_model->setHorizontalHeaderItem(m_pathId, new QStandardItem(QString("path")));
    m_model->setHorizontalHeaderItem(m_sizeId, new QStandardItem(QString("size (MB)")));
    m_ui->tableView->setModel(m_model.get());
}

bool AddFile_GUI::isPathExist (const QString &path)
{
    QModelIndex mi;
    QVariant v;

    for (int i=0; i<m_model->rowCount(); ++i)
    {
        mi = m_model->index(i,m_pathId);
        v=mi.data();
        if (v.toString() == path)
        {
            return true;
        }
    }
    return false;
}

void AddFile_GUI::addToTableView(const FileData & fileData)
{
    if (isPathExist(fileData.getPath()))
    {
        return;
    }

    m_model->setRowCount(m_model->rowCount()+1);

    m_model->setData(m_model->index(m_model->rowCount()-1,m_fileNameId),fileData.getFileName());
    m_model->setData(m_model->index(m_model->rowCount()-1,m_pathId),fileData.getPath());
    m_model->setData(m_model->index(m_model->rowCount()-1,m_sizeId),fileData.getSize());

    m_model->item(m_model->rowCount()-1,m_fileNameId)->setEditable(false);
    m_model->item(m_model->rowCount()-1,m_pathId)->setEditable(false);
    m_model->item(m_model->rowCount()-1,m_sizeId)->setEditable(false);
}

int AddFile_GUI::getFileNameByAbsolutePath(const QString &path, QString &filename)
{
    QString separator, fileNameTmp ="";
    separator = "/";

    if (separator.length()<1)
    {
        return 1;
    }

    for (int i=path.length()-1; i>=0; --i)
    {

        if (path[i] == separator[0])
        {
            break;
        }
        if (0 == i) //if a separator not found
        {
            return 1;
        }
        fileNameTmp = path[i] + fileNameTmp;
    }
    filename = fileNameTmp;
    return 0;
}

void AddFile_GUI::resizeEvent(QResizeEvent *)
{
    //set geometry of the widgets
    m_ui->addButton->setGeometry(m_offsetBetweenWidgets,
                                 m_offsetBetweenWidgets,
                                 m_ui->addButton->width(),
                                 m_ui->addButton->height());

    m_ui->removeButton->setGeometry(m_ui->addButton->x() + m_ui->addButton->width() + m_offsetBetweenWidgets,
                                    m_offsetBetweenWidgets,
                                    m_ui->removeButton->width(),
                                    m_ui->removeButton->height());

    m_ui->tableView->setGeometry(m_offsetBetweenWidgets,
                                 m_ui->addButton->y()+ m_ui->addButton->height() + m_offsetBetweenWidgets,
                                 this->width()-m_offsetBetweenWidgets*2,
                                 this->height()-(m_ui->addButton->height()*2+4*m_offsetBetweenWidgets));

    m_ui->okButton->setGeometry(this->width()-(m_ui->cancelButton->width()*2+2*m_offsetBetweenWidgets),
                                m_ui->tableView->y()+ m_ui->tableView->height() + m_offsetBetweenWidgets,
                                m_ui->okButton->width(),
                                m_ui->okButton->height());

    m_ui->cancelButton->setGeometry(m_ui->okButton->x() + m_ui->okButton->width() + m_offsetBetweenWidgets,
                                    m_ui->okButton->y(),
                                    m_ui->cancelButton->width(),
                                    m_ui->okButton->height());
}

void AddFile_GUI::clearTableView ()
{
    while (m_model->rowCount() > 0)
    {
        m_model->removeRow(0);
    }
}

void AddFile_GUI::loadData ()
{
    clearTableView();
    std::vector<FileData> sharedFiles = m_sharedFiles->getFileListData();
    for (auto &item: sharedFiles)
    {
          addToTableView(item);
    }
}

std::vector<FileData> AddFile_GUI::convertToVectorOfFileData()
{
        std::vector<FileData> sharedFiles;
        sharedFiles.reserve(m_model->rowCount());

        for (int i=0; i<m_model->rowCount(); i++)
        {
            FileData fileData (QString::number(i),
                               m_model->index(i,m_fileNameId).data().toString(),
                               m_model->index(i,m_pathId).data().toString(),
                               m_model->index(i,m_sizeId).data().toString());
            sharedFiles.push_back(fileData);
        }

        return sharedFiles;
}

//"OK" button
void AddFile_GUI::on_okButton_clicked()
{
    m_sharedFiles->update(convertToVectorOfFileData());
    this->hide();
}

//"Remove" button clicked
void AddFile_GUI::on_removeButton_clicked()
{
    //get selected rows
    QModelIndexList indexes = m_ui->tableView->selectionModel()->selectedRows();

    qSort(indexes.begin(), indexes.end());

    while (!indexes.isEmpty())
    {
        m_model->removeRows(indexes.last().row(), 1);
        indexes.removeLast();
    }
}

void AddFile_GUI::showEvent( QShowEvent *event )
{
    QDialog::showEvent( event );
    loadData();
}

void AddFile_GUI::on_cancelButton_clicked()
{
    this->hide();
}
