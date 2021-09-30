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

#include "filesharing_gui.h"
#include "ui_filesharing_gui.h"

FileSharing_GUI::FileSharing_GUI(QWidget *parent) :
    QMainWindow(parent)
    , m_aboutGUI (std::make_shared<About_GUI>())
    , m_sharedFiles (std::make_shared<SharedFiles> ())
    , m_addFileGUI (std::make_shared<AddFile_GUI>(m_sharedFiles))
    , m_defaultThreadCount (4)
    , m_downloadButtonClicked (false)
    , m_downloadColumnId(2)
    , m_currentHost ("")
    , m_loadingGif (std::make_shared<QLabel>(this))
    , m_loadingGifWidth (16)
    , m_loadingGifHeight (16)
    , m_maxHostCount (256)
    , m_model (std::make_shared<QStandardItemModel>(0,4,this))
    , m_movie (std::make_shared<QMovie>("ajax_waiting.gif"))
    , m_nameColumnId(0)
    , m_offsetBetweenWidgets(5)
    , m_progressColumnId(1)
    , m_scanIpGUI (std::make_shared<ScanIP_GUI> ())
    , m_scanNetwork (std::make_shared<ScanNetwork>())
    , m_server (m_sharedFiles)
    , m_setDirGUI (std::make_shared<SetDir_GUI> ())
    , m_setNetworkGUI (std::make_shared<SetNetwork_GUI>())
    , m_sizeColumnId (3)
    , m_threadPool (std::make_shared<QThreadPool>(this))
    , m_ui(std::make_shared<Ui::FileSharing_GUI> ())
{
    setupGui();
}

FileSharing_GUI::~FileSharing_GUI()
{
    m_scanNetwork->deleteLater();
}

void FileSharing_GUI::onFoundComputer(const QString &ip)
{
    addItemToThreeView(ip);
}

void FileSharing_GUI::on_finishScan()
{
    qDebug()<<"The network scan has finished.";
    postScanNetwork();
}

void FileSharing_GUI::addItemToThreeView(const QString &name)
{
    for (int i=0; i<m_ui->treeWidget->topLevelItemCount(); i++)
    {
        if (m_ui->treeWidget->topLevelItem(i)->text(0) == name)
        {
                return;
        }
    }

    std::shared_ptr<QTreeWidgetItem> item = std::make_shared<QTreeWidgetItem>();
    item->setText(0,name);
    m_ui->treeWidget->addTopLevelItem(item.get());
}

void FileSharing_GUI::setGeometryOfWidgets()
{
    m_ui->scanNetworkButton->setGeometry(m_offsetBetweenWidgets
                                  , m_offsetBetweenWidgets
                                  , m_ui->scanNetworkButton->width()
                                  , m_ui->scanNetworkButton->height());

    m_ui->treeWidget->setGeometry(m_offsetBetweenWidgets
                                  , m_offsetBetweenWidgets*2+m_ui->scanNetworkButton->height()
                                  , this->width()*0.3
                                  , this->height() - (m_offsetBetweenWidgets*3 + m_ui->scanNetworkButton->height()));

    m_ui->tableView->setGeometry(m_ui->treeWidget->width()+m_offsetBetweenWidgets*2
                                 , m_offsetBetweenWidgets
                                 , this->width()-(m_ui->treeWidget->width()+m_offsetBetweenWidgets*3)
                                 , this->height()-(m_offsetBetweenWidgets*3 + m_ui->downloadButton->height() + + m_ui->menuBar->height()));

    m_ui->downloadButton->setGeometry(this->width()-(m_offsetBetweenWidgets+m_ui->downloadButton->width())
                                    , this->height() - (m_offsetBetweenWidgets + m_ui->downloadButton->height() + m_ui->menuBar->height())
                                    , m_ui->downloadButton->width()
                                    , m_ui->downloadButton->height());
}

void FileSharing_GUI::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);

    m_ui->treeWidget->setGeometry(m_offsetBetweenWidgets
                                  , m_offsetBetweenWidgets
                                  , this->width()*0.3
                                  , this->height() - (m_offsetBetweenWidgets*4 + m_ui->scanNetworkButton->height()*2 + m_ui->menuBar->height()));

    m_ui->scanNetworkButton->setGeometry(m_offsetBetweenWidgets
                                  , m_offsetBetweenWidgets*2+m_ui->treeWidget->height()
                                  , m_ui->scanNetworkButton->width()
                                  , m_ui->scanNetworkButton->height());

    m_ui->scanIpButton->setGeometry(m_offsetBetweenWidgets
                                    , m_ui->scanNetworkButton->y() + m_ui->scanNetworkButton->height() + m_offsetBetweenWidgets
                                    , m_ui->scanNetworkButton->width()
                                    , m_ui->scanNetworkButton->height());

    m_ui->tableView->setGeometry(m_ui->treeWidget->width()+m_offsetBetweenWidgets*2
                                    , m_offsetBetweenWidgets
                                    , this->width()-(m_ui->treeWidget->width()+m_offsetBetweenWidgets*3)
                                    , this->height()-(m_offsetBetweenWidgets*3 + m_ui->downloadButton->height() + + m_ui->menuBar->height()));

    m_ui->downloadButton->setGeometry(this->width()-(m_offsetBetweenWidgets+m_ui->downloadButton->width())
                                    , this->height() - (m_offsetBetweenWidgets + m_ui->downloadButton->height()+m_ui->menuBar->height())
                                    , m_ui->downloadButton->width()
                                    , m_ui->downloadButton->height());

    m_loadingGif->setGeometry((m_ui->scanNetworkButton->width() + m_ui->treeWidget->width())/2 - 3
                                    , m_ui->scanIpButton->y()-m_offsetBetweenWidgets
                                    , m_loadingGifWidth
                                    , m_loadingGifHeight);

    m_ui->selectAllButton->setGeometry(m_ui->downloadButton->x() - (m_offsetBetweenWidgets + m_ui->selectAllButton->width())
                                    , m_ui->downloadButton->y()
                                    , m_ui->selectAllButton->width()
                                    , m_ui->selectAllButton->height());

    m_ui->selectNoneButton->setGeometry(m_ui->selectAllButton->x() - (m_offsetBetweenWidgets + m_ui->selectNoneButton->width())
                                    , m_ui->downloadButton->y()
                                    , m_ui->selectNoneButton->width()
                                    , m_ui->selectNoneButton->height());
}

void FileSharing_GUI::initModelTableView()
{
    m_model->setHorizontalHeaderItem(m_nameColumnId, new QStandardItem(QString("name")));
    m_model->setHorizontalHeaderItem(m_progressColumnId, new QStandardItem(QString("progress")));
    m_model->setHorizontalHeaderItem(m_downloadColumnId, new QStandardItem(QString("download")));
    m_model->setHorizontalHeaderItem(m_sizeColumnId, new QStandardItem(QString("size (KB)")));
    m_ui->tableView->setModel(m_model.get());
}

void FileSharing_GUI::preScanNetwork ()
{
    m_ui->treeWidget->clear();
    m_ui->scanNetworkButton->setEnabled(false);
    startLoadingAnimation();
}

void FileSharing_GUI::postScanNetwork ()
{
    m_ui->scanNetworkButton->setEnabled(true);
    stopLoadingAnimation();
}

bool FileSharing_GUI::getBaseNetworkIp (const QString &ipAddress, QString &baseNetworkIp)
{
    baseNetworkIp ="";
    int countDots = 0;
    int ipAddressDotCount = 3;
    for (int i=0; i<ipAddress.length(); ++i)
    {
        if (ipAddress[i]=='.')
        {
            ++countDots;
            if (countDots == ipAddressDotCount)
            {
                break;
            }
        }
        baseNetworkIp+=ipAddress[i];
    }

    if (countDots<ipAddressDotCount)
    {
        return false;
    }
    baseNetworkIp+='.';
    return true;
}

bool FileSharing_GUI::getLastNumberOfIpAddress (const QString &ipAddress, int &lastNumber)
{
    QString lastNum = "";
    lastNumber = 0;
    int maxDigitsCount = 3;
    int minDigitsCount = 1;

    for (int i = ipAddress.length()-1; ipAddress[i]!='.'; --i)
    {
        lastNum = ipAddress[i] + lastNum;
    }

    if (lastNum.length()>maxDigitsCount || lastNum.length()<minDigitsCount)
    {
        return false;
    }

    lastNumber = lastNum.toInt();
    return true;
}

void FileSharing_GUI::startScan(const QString &baseNetworkIp, const int lastNumberOfIpAddress)
{
    m_threadPool->releaseThread();

    if ("" == m_scanNetwork->getCurrentIP())
    {
        m_scanNetwork->setCurrentIP(baseNetworkIp);
    }

    initArrayOfScannedIpAddresses (lastNumberOfIpAddress);

    m_scanNetwork->setAutoDelete(false);

    for (int i=1; i<m_maxHostCount; ++i)
    {
        m_threadPool->start(m_scanNetwork.get());
    }
}

int FileSharing_GUI::scanNetwork()
{
    QString baseNetworkIp= "";
    int lastNumberOfIpAddress = 0;
    QString ipAddress = m_setNetworkGUI->load();

    preScanNetwork ();

    qDebug()<<"IP address: " + ipAddress;
    if ("" == ipAddress ||
        !getBaseNetworkIp(ipAddress, baseNetworkIp) ||
        !getLastNumberOfIpAddress (ipAddress, lastNumberOfIpAddress))
    {
        postScanNetwork ();
        return 1;
    }

    startScan (baseNetworkIp, lastNumberOfIpAddress);

    return 0;
}

void FileSharing_GUI::initArrayOfScannedIpAddresses (int lastNumberOfIpAddress)
{
    for(int i=0; i<m_maxHostCount-1; ++i)
    {
        if (i == lastNumberOfIpAddress-1)
        {
            m_scanNetwork->m_scannedIPAddresses[i]=1; //mark as scanned
            continue;
         }
         m_scanNetwork->m_scannedIPAddresses[i]=0; //mark as unscanned
    }
}

void FileSharing_GUI::clearTableView ()
{
    while (m_model->rowCount() > 0)
    {
        m_model->removeRow(0);
    }
}

//"Scan Network" button
void FileSharing_GUI::on_scanNetworkButton_clicked()
{
    clearTableView();
    scanNetwork();
}

void FileSharing_GUI::initActions()
{
    m_ui->actionFilie_or_Directory->setShortcut(QKeySequence::New);
    m_ui->actionFilie_or_Directory->setStatusTip(tr("File"));
    connect(m_ui->actionFilie_or_Directory, SIGNAL(triggered()), this, SLOT(menu_addFile()));
    connect(m_ui->actionDownloaded_Directory, SIGNAL(triggered()), this, SLOT(menu_downloadedDirectory()));
    connect(m_ui->actionSet_network, SIGNAL(triggered()), this, SLOT(menu_setNetwork()));
    connect(m_ui->actionAbout, SIGNAL(triggered()), this, SLOT(menu_about()));
}

void FileSharing_GUI::menu_about()
{
    m_aboutGUI->show();
}

void FileSharing_GUI::menu_addFile()
{
    m_addFileGUI->show();
}

//set dir (downloadedDirectory)
void FileSharing_GUI::menu_downloadedDirectory()
{
    m_setDirGUI->show();
}

void FileSharing_GUI::menu_setNetwork()
{
    m_setNetworkGUI->show();
}


void FileSharing_GUI::on_treeWidget_itemClicked(QTreeWidgetItem *item)
{
    QApplication::setOverrideCursor(Qt::WaitCursor); //change the cursor
    clearTableView();

    m_currentHost = item->text(0);
    NetworkManager networkManager;
    std::vector<RemoteHostFileData> remoteHostFileData = networkManager.getSharedFilesByRemoteHost(m_currentHost);

    for (auto &item : remoteHostFileData)
    {
        addDataInTableView(item.getFileName(), item.getSize());
    }

    QApplication::restoreOverrideCursor();
}

void FileSharing_GUI::addDataInTableView(const QString &fileName, const QString &size)
{
    m_model->setRowCount(m_model->rowCount()+1);

    m_model->setData(m_model->index(m_model->rowCount()-1,m_nameColumnId),fileName);
    m_model->setData(m_model->index(m_model->rowCount()-1,m_progressColumnId),"");

    //checkBox
    std::shared_ptr<QStandardItem> item0 = std::make_shared<QStandardItem>(true);
    item0->setCheckable(true);
    item0->setCheckState(Qt::Unchecked);
    m_model->setItem(m_model->rowCount()-1,m_downloadColumnId,item0.get());

    double size_num = size.toInt();
    size_num/=1024.0;
    m_model->setData(m_model->index(m_model->rowCount()-1,m_sizeColumnId),QString::number(size_num));

    //set data to be not editable
    m_model->item(m_model->rowCount()-1,m_nameColumnId)->setEditable(false);
    m_model->item(m_model->rowCount()-1,m_progressColumnId)->setEditable(false);
    m_model->item(m_model->rowCount()-1,m_downloadColumnId)->setEditable(false);
    m_model->item(m_model->rowCount()-1,m_sizeColumnId)->setEditable(false);
}

int FileSharing_GUI::modifyPath(QString &path)
{
    if (path[path.length()-1] == '\\' ||path[path.length()-1] == '/' )
    {
        return 1;
    }

    for (int i = path.length()-1; i>=0; --i)
    {
        if (path[i] == '\\' || path[i] == '/')
        {
            path+=path[i];
            return 1;
        }
    }

    return 0;
}

void FileSharing_GUI::startLoadingAnimation()
{
    m_movie->start();
    m_loadingGif->show();
}

void FileSharing_GUI::stopLoadingAnimation()
{
    m_movie->stop();
    m_loadingGif->hide();
}

QString FileSharing_GUI::setQuery(int index)
{
    if (index>=m_model->rowCount())
    {
        return "";
    }

    QString query = "\x10\t";

    QString filename=m_model->item(index, m_nameColumnId)->text();
    int num=0;
    for (int i = 0; i<m_model->rowCount(); ++i)
    {
        if (index == i)
        {
            break;
        }

        if (filename == m_model->item(i,m_nameColumnId)->text())
        {
            ++num;
        }
    }
    query+=QString::number(num) + "#" + filename+"\n";
    return query;
}

void FileSharing_GUI::clearProgressColumnData ()
{
    for (int i=0; i<m_model->rowCount(); ++i)
    {
        m_model->item(i, m_progressColumnId)->setText("");
    }
}

void FileSharing_GUI::startDownload (const int row)
{
    QModelIndex index;
    for (int i=row; i<m_model->rowCount(); ++i)
    {
        index = m_model->index(i,m_downloadColumnId, QModelIndex());
        if(index.data(Qt::CheckStateRole) == Qt::Checked)
        {
            QString dir = m_setDirGUI->load();
            if (0 == modifyPath(dir))
            {
                dir = QDir::currentPath();
            }

            QString query = setQuery(i);
            if ("" == query)
            {
                continue;
            }

            double sizeDouble = m_model->item(i,m_sizeColumnId)->text().toDouble();
            qint64 size = sizeDouble *1000;
            std::shared_ptr<Client> thread  = std::make_shared<Client>(m_currentHost, query, m_model->item(i,m_nameColumnId)->text(),dir,size,i);
            connect(thread.get(), SIGNAL(setProgress(int,double)), this, SLOT(on_setProgress(int,double)));
            thread->start();
            return;
        }
    }
}

//"Download" button
void FileSharing_GUI::on_downloadButton_clicked()
{
    if (m_downloadButtonClicked)
    {
        return;
    }

    clearProgressColumnData ();

    m_downloadButtonClicked = true;

    startDownload(0);

    m_downloadButtonClicked = false;
}


void FileSharing_GUI::next(const int row)
{
    startDownload(row + 1);
    m_downloadButtonClicked = false;
}

void FileSharing_GUI::on_setProgress(const int row, const double percentage)
{
    double error = -1;
    double downloadFinished = 100;

    if (error == percentage)
    {
        m_model->item(row, m_progressColumnId)->setText("Fail to download");
    }
    else
    {
        m_model->item(row, m_progressColumnId)->setText(QString::number(percentage) + "%");
    }

    if (downloadFinished == percentage)
    {
        next(row);
    }
}

//"Scan IP" button
void FileSharing_GUI::on_scanIpButton_clicked()
{
    m_scanIpGUI->show();
}

void FileSharing_GUI::on_scanIP(const QString &ip)
{
    if (""!=ip)
    {
        addItemToThreeView(ip);
    }
}

//"Select All" button
void FileSharing_GUI::on_selectAllButton_clicked()
{
    for (int i=0; i<m_model->rowCount(); ++i)
    {
        m_model->item(i, m_downloadColumnId)->setCheckState(Qt::Checked);
    }
}

//"Select None" button
void FileSharing_GUI::on_selectNoneButton_clicked()
{
    for (int i=0; i<m_model->rowCount(); i++)
    {
        m_model->item(i, m_downloadColumnId)->setCheckState(Qt::Unchecked);
    }
}

void FileSharing_GUI::setThreadCount ()
{
    int availableThreadCount = std::thread::hardware_concurrency();
    if (0==availableThreadCount)
    {
        m_threadPool->setMaxThreadCount(m_defaultThreadCount);
    }
    else
    {
        m_threadPool->setMaxThreadCount(availableThreadCount);
    }
}

void FileSharing_GUI::allocateMemory()
{
    m_scanNetwork->m_scannedIPAddresses.reserve(m_maxHostCount);
}

void FileSharing_GUI::setupConnections ()
{
    connect(m_scanIpGUI.get(),SIGNAL(scanIP(QString)),this,SLOT(on_scanIP(QString)));

    //when a host has been found
    connect(m_scanNetwork.get(),SIGNAL(foundComputer(QString)),this,SLOT(onFoundComputer(QString)));

    //when the network scanner has finished
    connect(m_scanNetwork.get(),SIGNAL(finishScan()),this,SLOT(on_finishScan()));
}

void FileSharing_GUI::setupGui ()
{
    m_ui->setupUi(this);

    setThreadCount();
    setGeometryOfWidgets();
    initModelTableView();
    m_server.StartServer();

    //init menu actions
    initActions();

    setupConnections();

    m_ui->treeWidget->setHeaderLabel("Avaliable Hosts");

    //init waiting gif animation
    m_loadingGif->setMovie(m_movie.get());

    allocateMemory();
}

