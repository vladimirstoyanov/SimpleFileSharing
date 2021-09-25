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

#ifndef FILESHARING_GUI_H
#define FILESHARING_GUI_H

#include <QDir>
#include <QMainWindow>
#include <QMessageBox>
#include <QMovie>
#include <QNetworkInterface>
#include <QStandardItemModel>
#include <QTcpServer>
#include <QThreadPool>
#include <QTreeWidgetItem>

#include <memory>
#include <thread>

#include "about_gui.h"
#include "addfile_gui.h"
#include "client.h"
#include "my_server.h"
#include "scanip_gui.h"
#include "scan_network.h"
#include "setdir_gui.h"
#include "setnetwork_gui.h"
#include "shared_files.h"

/*
 Main GUI window
*/

namespace Ui
{
class FileSharing_GUI;
}

class FileSharing_GUI : public QMainWindow
{
    Q_OBJECT

public:
    explicit FileSharing_GUI(QWidget *parent = 0);
    virtual ~FileSharing_GUI();

public slots:
    void onFoundComputer(const QString &ip);
    void on_finishScan();

private:
    std::shared_ptr<About_GUI>              m_aboutGUI;
    std::shared_ptr<SharedFiles>            m_sharedFiles;
    std::shared_ptr<AddFile_GUI>            m_addFileGUI;
    int                                     m_defaultThreadCount;
    bool                                    m_downloadButtonClicked;
    QString                                 m_currentHost;
    std::shared_ptr<QLabel>                 m_loadingGif;
    std::shared_ptr<QStandardItemModel>     m_model;
    std::shared_ptr<QMovie>                 m_movie;
    std::shared_ptr<ScanIP_GUI>             m_scanIpGUI;
    std::shared_ptr<ScanNetwork>            m_scanNetwork;
    MyServer                                m_server;
    std::shared_ptr<SetDir_GUI>             m_setDirGUI;
    std::shared_ptr<SetNetwork_GUI>         m_setNetworkGUI;
    std::shared_ptr<QThreadPool>            m_threadPool;
    std::shared_ptr<Ui::FileSharing_GUI>    m_ui;


    void    addDataInTableView(const QString &file_name, const QString &size);
    void    addItemToThreeView(const QString &item);
    void    clearProgressColumnData ();
    void    initActions();
    void    initModelTableView();
    int     modifyPath(QString &path);
    void    next(const int row);
    int     scanNetwork();
    void    setThreadCount ();
    void    resizeEvent(QResizeEvent *);
    void    setupConnections ();
    void    setupGui ();
    void    setGeometryOfWidgets();
    QString setQuery(const int index);
    void    startDownload (const int row);
    void    startWaitAnimation();
    void    stopWaitAnimation();

private slots:
    void menu_about();
    void menu_addFile();
    void menu_downloadedDirectory();
    void menu_setNetwork();
    void on_treeWidget_itemClicked(QTreeWidgetItem *item);
    void on_downloadButton_clicked();                               //"Download" button
    void on_scanIP(const QString &ip);
    void on_scanIpButton_clicked();                                 //"Scan IP" button
    void on_scanNetworkButton_clicked();                            //"Scan Network" button
    void on_selectAllButton_clicked();                              //"Select All" button
    void on_selectNoneButton_clicked();                             //"Select None" button
    void on_setProgress (const int row, const double percentage);

signals:
    void EmerFinish(); //ToDo: change the name
};

#endif //FILESHARING_GUI_H
