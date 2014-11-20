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

#include <QMainWindow>
#include <QTreeWidgetItem>
#include <QStandardItemModel>
#include <QNetworkInterface>
#include <QThreadPool>
#include <QTcpServer>
#include <QMessageBox>
#include <QDir>
#include <QMovie>
#include <QLabel>
#include "scan_network.h"
#include "client.h"
#include "addfile_gui.h"
#include "myserver.h"
#include "setdir_gui.h"
#include "setnetwork_gui.h"
#include "scanip_gui.h"

/*
 Main GUI window
*/

namespace Ui {
class FileSharing_GUI;
}

class FileSharing_GUI : public QMainWindow
{
    Q_OBJECT

public:
    //quint16 Threads;

    explicit FileSharing_GUI(QWidget *parent = 0);
    ~FileSharing_GUI();


public slots:
    void onFoundComputer(QString ip);
    void on_finishScan();
private slots:
    void menu_addFile();
    void menu_downloadedDirectory();
    void menu_setNetwork();
    void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);
    void on_scanIP(QString ip);
    void on_setProgress (int row, double percentage);

    void on_pushButton_clicked(); //'Scan Network' button
    void on_pushButton_2_clicked(); //'Download' button
    void on_pushButton_3_clicked(); //'Scan IP' button
    void on_pushButton_4_clicked(); //'Select All' button
    void on_pushButton_5_clicked(); //'Select None' button



private:
    Ui::FileSharing_GUI *ui;
    QStandardItemModel *model;
    scan_network *sn;
    SetDir_GUI *sd_g;
    SetNetwork_GUI *sn_g;
    QThreadPool *tp;
    ScanIP_GUI *sip_g;
    AddFile_GUI *m_AddFile_Gui;
    qreal inBytes, outBytes;
    MyServer server;
    QString current_host;
    QMovie *movie;
    QLabel *l_waiting_gif;
    bool download_button_clicked;

    void addItemToThreeView(QString item);
    void setGeometryOfWidgets();
    void resizeEvent(QResizeEvent *);
    void initModelTableView();
    int scanNetwork();
    void initActions();
    void addDataInTableView(QString file_name, QString size);
    int modifyPath(QString &path);
    void startWaitAnimation();
    void stopWaitAnimation();
    void next(int row);
    QString setQuery(int index);

signals:
    void EmerFinish();
};

#endif //FILESHARING_GUI_H
