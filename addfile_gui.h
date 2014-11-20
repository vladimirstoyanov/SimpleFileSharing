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

#ifndef ADDFILE_GUI_H
#define ADDFILE_GUI_H

/*
 *From menu: Share->File
*/

#include <QDialog>
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QStandardItemModel>
#include <QMessageBox>
#include <QList>

namespace Ui {
class AddFile_GUI;
}

class AddFile_GUI : public QDialog
{
    Q_OBJECT

public:
    explicit AddFile_GUI(QWidget *parent = 0);
    ~AddFile_GUI();
    QList< QList<QString> > getListWithSharedFiles (); //list with shared files (name, size etc.)

private slots:
    void on_pushButton_clicked(); //'Add' button
    void on_pushButton_2_clicked(); //'OK' button
    void on_pushButton_4_clicked(); //'Remove' button
    void on_pushButton_3_clicked(); //'Cancel' button

private:
    Ui::AddFile_GUI *ui;
    QStandardItemModel *model;
    QString add_file_last_dir;

    int getDirNameFromPath(QString path, QString &dirname); //from full path of file, get only path to directory where is a file
    void initModelTableView(); //initialization of QTableView with QStandardItemModel model
    void addDataInTableView(QString file_name, QString path, QString size);
    int  getFileNameFromPath(QString path, QString &filename);//from full path of file, get only filename
    void resizeEvent(QResizeEvent *); //resize window event
    void save(); //save list with shared files in 'sharedFiles' file
    void load(); //load list with shared files from 'sharedFiles' file
    void showEvent( QShowEvent *event ); //event when window appear
};

#endif // ADDFILE_GUI_H
