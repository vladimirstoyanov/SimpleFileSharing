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

#include "addfile_gui.h"
#include "search.h"
#include "ui_addfile_gui.h"

AddFile_GUI::AddFile_GUI(QWidget *parent) :
    QDialog(parent)
    , m_addFileLastDir("")
    , m_model (std::make_shared<QStandardItemModel>(0,3,this))
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
    //Dialog to chose files
    QStringList pathList = QFileDialog::getOpenFileNames(this, tr("Share File(s)"), m_addFileLastDir, tr("Files (*.*)"));

    if (pathList.size() == 0)
    {
        return;
    }

    QString dirNameTmp = "";
    if (!getDirNameByAbsolutePath(pathList[0], dirNameTmp))
    {
        m_addFileLastDir = dirNameTmp;
    }

    for (int i=0; i<pathList.size(); i++)
    {
        QString filename="";
        if (getFileNameByAbsolutePath(pathList[i],filename))
        {
            qDebug()<<"Error path: " + pathList[i];
            continue;
        }
        QFileInfo fi(pathList[i]);
        double size = fi.size()/(1024.0*1024.0); //calculate size in KB
        addDataInTableView(filename, pathList[i], QString::number(size));
    }
}

int AddFile_GUI::getDirNameByAbsolutePath(const QString &path, QString &dirname)
{
    QString separator;
    separator = "/";

    int index=-1;
    for (int i=path.length()-1; i>=0; i--)
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

    for (int i=0; i<=index; i++)
    {
        dirname+=path[i];
    }

    return 0;
}

void AddFile_GUI::initModelTableView()
{
    m_model->setHorizontalHeaderItem(0, new QStandardItem(QString("file name")));
    m_model->setHorizontalHeaderItem(1, new QStandardItem(QString("path")));
    m_model->setHorizontalHeaderItem(2, new QStandardItem(QString("size (MB)")));
    m_ui->tableView->setModel(m_model.get());
}

void AddFile_GUI::addDataInTableView(const QString &fileName, const QString &path, const QString &size)
{
    QModelIndex mi;
    QVariant v;

    //check is "path" exist
    for (int i=0; i<m_model->rowCount(); i++)
    {
        mi = m_model->index(i,1);
        v=mi.data();
        if (v.toString() == path)
            return;
    }

    m_model->setRowCount(m_model->rowCount()+1);

    m_model->setData(m_model->index(m_model->rowCount()-1,0),fileName);
    m_model->setData(m_model->index(m_model->rowCount()-1,1),path);
    m_model->setData(m_model->index(m_model->rowCount()-1,2),size);

    //set data not editable
    m_model->item(m_model->rowCount()-1,0)->setEditable(false);
    m_model->item(m_model->rowCount()-1,1)->setEditable(false);
    m_model->item(m_model->rowCount()-1,2)->setEditable(false);
}

int AddFile_GUI::getFileNameByAbsolutePath(const QString &path, QString &filename)
{
    QString separator, fileNameTmp ="";
    separator = "/";

    if (separator.length()<1)
    {
        return 1;
    }

    for (int i=path.length()-1; i>=0; i--)
    {

        if (path[i] == separator[0])
        {
            break;
        }
        if (i == 0) //if not found a separator, it's return "1" (an error)
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
    //set geometry of widgets
    m_ui->addButton->setGeometry(5,5,m_ui->addButton->width(), m_ui->addButton->height());
    m_ui->removeButton->setGeometry(m_ui->addButton->x() + m_ui->addButton->width() + 5, 5,m_ui->removeButton->width(), m_ui->removeButton->height());
    m_ui->tableView->setGeometry(5,m_ui->addButton->y()+ m_ui->addButton->height() + 5,this->width()-10,this->height()-(m_ui->addButton->height()*2+20));
    m_ui->okButton->setGeometry(this->width()-(m_ui->cancelButton->width()*2+10), m_ui->tableView->y()+ m_ui->tableView->height() + 5, m_ui->okButton->width(), m_ui->okButton->height());
    m_ui->cancelButton->setGeometry(m_ui->okButton->x() + m_ui->okButton->width() + 5, m_ui->okButton->y(), m_ui->cancelButton->width(), m_ui->okButton->height());
}
void AddFile_GUI::save()
{
    QFile file( "sharedFiles" );
    if (file.open(QIODevice::ReadWrite | QIODevice::Truncate))
    {
        QTextStream stream( &file );
        QModelIndex mi;
        QVariant v;

        for (int i=0; i<m_model->rowCount(); i++)
        {
            mi = m_model->index(i,0);
            v=mi.data();
            stream<<"<file>\n\t<file_name>" + v.toString() + "</file_name>\n\t";

            mi = m_model->index(i,1);
            v=mi.data();
            stream<<"<path>" + v.toString() + "</path>\n\t";

            mi = m_model->index(i,2);
            v=mi.data();
            stream<<"<size>" + v.toString() + "</size>\n</file>\n";
       }

       file.flush();
       file.close();
    }
}

//ToDo: refactor the below method
void AddFile_GUI::load()
{
    Search search;
    //clear the old data
    while (m_model->rowCount() > 0)
    {
        m_model->removeRow(0);
    }

    QFile file("sharedFiles");
    if(!file.open(QIODevice::ReadOnly))
    {
        return;
        //QMessageBox::information(0, "error", file.errorString());
    }

    QTextStream in(&file);

    while(!in.atEnd())
    {
        QString line = in.readLine();
        unsigned int index =0;
        QString fileTag = "<file>";
        search.searchAfter(line, fileTag, index);
        if (index != fileTag.length())
        {
            return;
        }

        line = in.readLine();
        QString fileName = "";
        if (search.getTextBetweenTwoStrings(line, "<file_name>", "</file_name>", fileName))
        {
            return;
        }

        line = in.readLine();
        QString path = "";
        if (search.getTextBetweenTwoStrings(line,"<path>", "</path>", path))
        {
            return;
        }

        line = in.readLine();
        QString size="";
        if (search.getTextBetweenTwoStrings(line,"<size>", "</size>", size))
        {
            return;
        }

        line = in.readLine();
        index = 0;
        fileTag = "</file>";
        search.searchAfter(line,fileTag, index);
        if (index != fileTag.length())
        {
            return;
        }

        //add to "tableView"
        addDataInTableView(fileName, path, size);
    }

    file.close();
}



//"OK" button
void AddFile_GUI::on_okButton_clicked()
{
    save();
    this->hide();
}

//remove button clicked
void AddFile_GUI::on_removeButton_clicked()
{
    //get selected rows
    QModelIndexList indexes = m_ui->tableView->selectionModel()->selectedRows();

    qSort(indexes.begin(), indexes.end());

    //remove last index in list
    while (!indexes.isEmpty())
    {
        m_model->removeRows(indexes.last().row(), 1);
        indexes.removeLast();
    }
}

void AddFile_GUI::showEvent( QShowEvent *event )
{
    QDialog::showEvent( event );
    load();
}

//ToDo: refactor the below method
QList< QList<QString> > AddFile_GUI::getListWithSharedFiles()
{
    Search search;
    QList< QList<QString> > sharedFilesList;

    QFile file("sharedFiles");
    if(!file.open(QIODevice::ReadOnly))
    {
        return sharedFilesList;
    }

    QTextStream in(&file);

    while(!in.atEnd())
    {
        QString line = in.readLine();
        unsigned int index =0;
        QString fileTag = "<file>";
        search.searchAfter(line, fileTag, index);
        if (index != fileTag.length())
        {
            file.close();
            return sharedFilesList;
        }

        line = in.readLine();
        QString fileName = "";
        if (search.getTextBetweenTwoStrings(line,"<file_name>", "</file_name>", fileName))
        {
            file.close();
            return sharedFilesList;
        }

        line = in.readLine();
        QString path = "";
        if (search.getTextBetweenTwoStrings(line,"<path>", "</path>", path))
        {
            file.close();
            return sharedFilesList;
        }

        line = in.readLine();
        QString size="";
        if (search.getTextBetweenTwoStrings(line,"<size>", "</size>", size))
        {
            file.close();
            return sharedFilesList;
        }

        line = in.readLine();
        index = 0;
        fileTag = "</file>";
        search.searchAfter(line, fileTag, index);
        if (index != fileTag.length())
        {
            file.close();
            return sharedFilesList;
        }

        //add to tableView
        QList<QString> item;
        item.push_back(fileName);
        item.push_back(path);
        item.push_back(size);
        sharedFilesList.push_back(item);
    }

    file.close();
    return sharedFilesList;
}

void AddFile_GUI::on_cancelButton_clicked()
{
    this->hide();
}
