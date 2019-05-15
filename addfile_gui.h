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

#include <memory>

namespace Ui
{
    class AddFile_GUI;
}

class AddFile_GUI : public QDialog
{
    Q_OBJECT

public:
    explicit AddFile_GUI(QWidget *parent = nullptr);
    virtual ~AddFile_GUI();

    //ToDo: move the below list to the private section
    QList< QList<QString> > getListWithSharedFiles (); //list with shared files (name, size etc.)

private slots:
    void on_addButton_clicked();    //"Add" button
    void on_cancelButton_clicked(); //"Cancel" button
    void on_okButton_clicked();     //"OK" button
    void on_removeButton_clicked(); //"Remove" button


private:
    QString                             m_addFileLastDir;
    std::shared_ptr<QStandardItemModel> m_model;
    std::shared_ptr<Ui::AddFile_GUI>    m_ui;

private:
    void addDataInTableView(const QString &fileName, const QString &path, const QString &size);
    int  getDirNameByAbsolutePath(const QString &path, QString &dirname);
    int  getFileNameByAbsolutePath(const QString &path, QString &filename);//get the filename by an absolute path
    void initModelTableView(); //initialization of QTableView with QStandardItemModel model
    void load(); //load list with shared files from "sharedFiles" file
    void resizeEvent(QResizeEvent *); //this event is called when "addfile_gui" window is resized
    void save(); //save list with shared files in "sharedFiles" file
    void showEvent( QShowEvent *event ); //this event is called when "addfile_gui" window is shown

};

#endif // ADDFILE_GUI_H
