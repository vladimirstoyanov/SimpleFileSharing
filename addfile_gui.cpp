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

#include "addfile_gui.h"
#include "ui_addfile_gui.h"
#include <QFileInfo>
#include <math.h>
#include "search.h"

AddFile_GUI::AddFile_GUI(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddFile_GUI)
{
    ui->setupUi(this);
    initModelTableView();
    add_file_last_dir = "";
}

AddFile_GUI::~AddFile_GUI()
{
    delete ui;
}

//'Add' button
void AddFile_GUI::on_pushButton_clicked()
{
    //Dialog to chose files
    QStringList l_path = QFileDialog::getOpenFileNames(this, tr("Share File(s)"),add_file_last_dir,tr("Files (*.*)"));

    if (l_path.size() == 0)
        return;

    QString afld_tmp = "";
    if (!getDirNameFromPath(l_path[0], afld_tmp))
        add_file_last_dir = afld_tmp;

    for (int i=0; i<l_path.size(); i++)
    {
        QString filename="";
        if (getFileNameFromPath(l_path[i],filename))
        {
            qDebug()<<"Error path: " + l_path[i];
            continue;
        }
        QFileInfo fi(l_path[i]);
        double size = fi.size()/(1024.0*1024.0); //calculate size in KB
        addDataInTableView(filename, l_path[i], QString::number(size));
    }
}

int AddFile_GUI::getDirNameFromPath(QString path, QString &dirname)
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
        return 1;

    for (int i=0; i<=index; i++)
        dirname+=path[i];

    return 0;
}

void AddFile_GUI::initModelTableView()
{
    model = new QStandardItemModel(0,3,this);
    model->setHorizontalHeaderItem(0, new QStandardItem(QString("file name")));
    model->setHorizontalHeaderItem(1, new QStandardItem(QString("path")));
    model->setHorizontalHeaderItem(2, new QStandardItem(QString("size (MB)")));
    ui->tableView->setModel(model);
}

void AddFile_GUI::addDataInTableView(QString file_name, QString path, QString size)
{
    QModelIndex mi;
    QVariant v;

    //check for exist path
    for (int i=0; i<model->rowCount(); i++)
    {
        mi = model->index(i,1);
        v=mi.data();
        if (v.toString() == path)
            return;
    }

    model->setRowCount(model->rowCount()+1);

    model->setData(model->index(model->rowCount()-1,0),file_name);
    model->setData(model->index(model->rowCount()-1,1),path);
    model->setData(model->index(model->rowCount()-1,2),size);

    //set data not editable
    model->item(model->rowCount()-1,0)->setEditable(false);
    model->item(model->rowCount()-1,1)->setEditable(false);
    model->item(model->rowCount()-1,2)->setEditable(false);
}

int AddFile_GUI::getFileNameFromPath(QString path, QString &filename)
{
    QString separator, filename_tmp ="";
    separator = "/";

    if (separator.length()<1)
        return 1;

    for (int i=path.length()-1; i>=0; i--)
    {

        if (path[i] == separator[0])
            break;
        if (i == 0) //if not found separator, return 1, mean error
            return 1;
        filename_tmp = path[i] + filename_tmp;
    }
    filename = filename_tmp;
    return 0;
}

void AddFile_GUI::resizeEvent(QResizeEvent *)
{
    //set geometry of widgets
    ui->pushButton->setGeometry(5,5,ui->pushButton->width(), ui->pushButton->height());
    ui->pushButton_4->setGeometry(ui->pushButton->x() + ui->pushButton->width() + 5, 5,ui->pushButton_4->width(), ui->pushButton_4->height());
    ui->tableView->setGeometry(5,ui->pushButton->y()+ ui->pushButton->height() + 5,this->width()-10,this->height()-(ui->pushButton->height()*2+20));
    ui->pushButton_2->setGeometry(this->width()-(ui->pushButton_3->width()*2+10), ui->tableView->y()+ ui->tableView->height() + 5, ui->pushButton_2->width(), ui->pushButton_2->height());
    ui->pushButton_3->setGeometry(ui->pushButton_2->x() + ui->pushButton_2->width() + 5, ui->pushButton_2->y(), ui->pushButton_3->width(), ui->pushButton_2->height());
}
void AddFile_GUI::save()
{
    QFile file( "sharedFiles" );
    if (file.open(QIODevice::ReadWrite | QIODevice::Truncate))
    {
        QTextStream stream( &file );
        QModelIndex mi;
        QVariant v;

        //check for exist path
        for (int i=0; i<model->rowCount(); i++)
        {
            mi = model->index(i,0);
            v=mi.data();
            stream<<"<file>\n\t<file_name>" + v.toString() + "</file_name>\n\t";

            mi = model->index(i,1);
            v=mi.data();
            stream<<"<path>" + v.toString() + "</path>\n\t";

            mi = model->index(i,2);
            v=mi.data();
            stream<<"<size>" + v.toString() + "</size>\n</file>\n";
       }
       file.flush();
       file.close();
    }
}
void AddFile_GUI::load()
{
    CSearch cs;
    //clear old data
    while (model->rowCount() > 0)
    {
        model->removeRow(0);
    }

    QFile file("sharedFiles");
    if(!file.open(QIODevice::ReadOnly)) {
        return;
        //QMessageBox::information(0, "error", file.errorString());
    }

    QTextStream in(&file);

    while(!in.atEnd()) {
        QString line = in.readLine();
        int index =0;
        cs.Search_After(line,"<file>", &index);
        if (index != 6)
            return;

        line = in.readLine();
        QString file_name = "";
        if (cs.getTextBetweenTwoStrings(line,"<file_name>", "</file_name>", file_name))
            return;

        line = in.readLine();
        QString path = "";
        if (cs.getTextBetweenTwoStrings(line,"<path>", "</path>", path))
            return;

        line = in.readLine();
        QString size="";
        if (cs.getTextBetweenTwoStrings(line,"<size>", "</size>", size))
            return;

        line = in.readLine();
        index = 0;
        cs.Search_After(line,"</file>", &index);
        if (index != 7)
            return;

        //add to tableView
        addDataInTableView(file_name, path, size);
    }

    file.close();
}



//OK button
void AddFile_GUI::on_pushButton_2_clicked()
{
    save();
    this->hide();
}

//remove button clicked
void AddFile_GUI::on_pushButton_4_clicked()
{
    //get selected rows
    QModelIndexList indexes = ui->tableView->selectionModel()->selectedRows();

    qSort(indexes.begin(), indexes.end());

    //remove last index in list
    while (!indexes.isEmpty())
    {
        model->removeRows(indexes.last().row(), 1);
        indexes.removeLast();
    }
}

void AddFile_GUI::showEvent( QShowEvent *event )
{
    QDialog::showEvent( event );
    load();
}

QList< QList<QString> > AddFile_GUI::getListWithSharedFiles()
{
    CSearch cs;
    QList< QList<QString> > l_shared_files;

    QFile file("sharedFiles");
    if(!file.open(QIODevice::ReadOnly)) {
        return l_shared_files;
    }

    QTextStream in(&file);

    while(!in.atEnd()) {
        QString line = in.readLine();
        int index =0;
        cs.Search_After(line,"<file>", &index);
        if (index != 6)
        {
            file.close();
            return l_shared_files;
        }

        line = in.readLine();
        QString file_name = "";
        if (cs.getTextBetweenTwoStrings(line,"<file_name>", "</file_name>", file_name))
        {
            file.close();
            return l_shared_files;
        }

        line = in.readLine();
        QString path = "";
        if (cs.getTextBetweenTwoStrings(line,"<path>", "</path>", path))
        {
            file.close();
            return l_shared_files;
        }

        line = in.readLine();
        QString size="";
        if (cs.getTextBetweenTwoStrings(line,"<size>", "</size>", size))
        {
            file.close();
            return l_shared_files;
        }

        line = in.readLine();
        index = 0;
        cs.Search_After(line,"</file>", &index);
        if (index != 7)
        {
            file.close();
            return l_shared_files;
        }

        //add to tableView
        QList<QString> item;
        item.push_back(file_name);
        item.push_back(path);
        item.push_back(size);
        l_shared_files.push_back(item);
    }

    file.close();
    return l_shared_files;
}

void AddFile_GUI::on_pushButton_3_clicked()
{
    this->hide();
}
