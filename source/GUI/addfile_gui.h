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
menu: Share->File
*/

#include <QDialog>
#include <QDebug>
#include <QFileDialog>
#include <QStandardItemModel>
#include <QMessageBox>

#include "parse_file_data.h"
#include "shared_files.h"

#include <memory>
#include <vector>

namespace Ui
{
    class AddFile_GUI;
}

class AddFile_GUI : public QDialog
{
    Q_OBJECT

public:
    explicit AddFile_GUI( std::shared_ptr<SharedFiles> sharedFiles,
                          QWidget *parent = nullptr);
    virtual ~AddFile_GUI();

private slots:
    void onAddButtonClicked();    //"Add" button
    void onCancelButtonClicked(); //"Cancel" button
    void onOkButtonClicked();     //"OK" button
    void onRemoveButtonClicked(); //"Remove" button


private:
    QString                             m_addFileLastDir;
    int                                 m_fileNameId;
    std::unique_ptr<QStandardItemModel> m_model;
    int                                 m_offsetBetweenWidgets;
    int                                  m_pathId;
    std::shared_ptr<SharedFiles>        m_sharedFiles;
    int                                 m_sizeId;
    std::unique_ptr<Ui::AddFile_GUI>    m_ui;


    void addToTableView(const FileData & fileData);
    void clearTableView ();
    std::vector<FileData> convertToVectorOfFileData();
    int  getDirNameByAbsolutePath(const QString &path, QString &dirname);
    int  getFileNameByAbsolutePath(const QString &path, QString &filename);//get the filename by an absolute path
    void initModelTableView(); //initialization of QTableView with QStandardItemModel model
    bool isPathExist (const QString &path);
    void loadData(); //load list with shared files
    void resizeEvent(QResizeEvent *); //this event is called when "addfile_gui" window is resized
    void showEvent( QShowEvent *event ); //this event is called when "addfile_gui" window is shown
};

#endif // ADDFILE_GUI_H
