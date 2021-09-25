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
#include <QStandardItemModel>
#include <QThreadPool>
#include <QTreeWidgetItem>

#include <memory>
#include <thread>
#include <vector>

#include "about_gui.h"
#include "addfile_gui.h"
#include "client.h"
#include "my_server.h"
#include "remote_host_file_data.h"
#include "scanip_gui.h"
#include "scan_network.h"
#include "setdir_gui.h"
#include "setnetwork_gui.h"
#include "shared_files.h"
#include "socket.h"

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
    int                                     m_downloadColumnId;
    QString                                 m_currentHost;
    std::shared_ptr<QLabel>                 m_loadingGif;
    int                                     m_loadingGifWidth;
    int                                     m_loadingGifHeight;
    int                                     m_maxHostCount;
    std::shared_ptr<QStandardItemModel>     m_model;
    std::shared_ptr<QMovie>                 m_movie;
    int                                     m_nameColumnId;
    int                                     m_offsetBetweenWidgets;
    int                                     m_port;
    int                                     m_progressColumnId;
    std::shared_ptr<ScanIP_GUI>             m_scanIpGUI;
    std::shared_ptr<ScanNetwork>            m_scanNetwork;
    MyServer                                m_server;
    std::shared_ptr<SetDir_GUI>             m_setDirGUI;
    std::shared_ptr<SetNetwork_GUI>         m_setNetworkGUI;
    int                                     m_sizeColumnId;
    std::shared_ptr<QThreadPool>            m_threadPool;
    std::shared_ptr<Ui::FileSharing_GUI>    m_ui;


    void    addDataInTableView(const QString &file_name, const QString &size);
    void    addItemToThreeView(const QString &item);
    void    allocateMemory();
    void    clearProgressColumnData ();
    void    clearTableView ();
    bool    getBaseNetworkIp (const QString &ipAddress, QString &baseNetworkIp);
    bool    getLastNumberOfIpAddress (const QString &ipAddress, int &lastNumber);
    void    initActions();
    void    initArrayOfScannedIpAddresses (int lastNumberOfIpAddress);
    void    initModelTableView();
    int     modifyPath(QString &path);
    void    next(const int row);
    void    postScanNetwork ();
    void    preScanNetwork ();
    void    resizeEvent(QResizeEvent *);
    int     scanNetwork();
    void    setThreadCount ();
    void    setupConnections ();
    void    setupGui ();
    void    setGeometryOfWidgets();
    QString setQuery(const int index);
    void    startDownload (const int row);
    void    startLoadingAnimation();
    void    startScan(const QString &baseNetworkIp, const int lastNumberOfIpAddress);
    void    stopLoadingAnimation();

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
