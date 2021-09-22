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
    , m_downloadButtonClicked (false)
    , m_currentHost ("")
    , m_loadingGif (std::make_shared<QLabel>(this))
    , m_model (std::make_shared<QStandardItemModel>(0,4,this))
    , m_movie (std::make_shared<QMovie>("ajax_waiting.gif"))
    , m_scanIpGUI (std::make_shared<ScanIP_GUI> ())
    , m_scanNetwork (std::make_shared<ScanNetwork>())
    , m_server (m_sharedFiles)
    , m_setDirGUI (std::make_shared<SetDir_GUI> ())
    , m_setNetworkGUI (std::make_shared<SetNetwork_GUI>())
    , m_threadPool (std::make_shared<QThreadPool>(this))
    , m_ui(std::make_shared<Ui::FileSharing_GUI> ())
{
    m_ui->setupUi(this);

    //thread pool used to scanning a network
    m_threadPool->setMaxThreadCount(50); //set max thread count to available CPU threads

    setGeometryOfWidgets();
    initModelTableView();
    m_server.StartServer();

    //init menu actions
    initActions();

    //'scanip_gui.cpp' make connect with this class, when given IP using this program.
    connect(m_scanIpGUI.get(),SIGNAL(scanIP(QString)),this,SLOT(on_scanIP(QString)));

    m_ui->treeWidget->setHeaderLabel("Avaliable Hosts");

    //init waiting gif animation
    m_loadingGif->setMovie(m_movie.get());

    //when found computer
    connect(m_scanNetwork.get(),SIGNAL(foundComputer(QString)),this,SLOT(onFoundComputer(QString)));

    //when a scanner finished
    connect(m_scanNetwork.get(),SIGNAL(finishScan()),this,SLOT(on_finishScan()));
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
    m_ui->scanNetworkButton->setEnabled(true);
    qDebug()<<"The network scan has finished.";
    stopWaitAnimation();
}

//add item to treeview
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

//set geometry of widgets
void FileSharing_GUI::setGeometryOfWidgets()
{
    m_ui->scanNetworkButton->setGeometry(5
                                  , 5
                                  , m_ui->scanNetworkButton->width()
                                  , m_ui->scanNetworkButton->height());

    m_ui->treeWidget->setGeometry(5
                                  , 10+m_ui->scanNetworkButton->height()
                                  , 180
                                  , this->height() - (15 + m_ui->scanNetworkButton->height()));

    m_ui->tableView->setGeometry(m_ui->treeWidget->width()+10
                                 , 5
                                 , this->width()-(m_ui->treeWidget->width()+15),this->height()-(15 + m_ui->downloadButton->height() + + m_ui->menuBar->height()));

    m_ui->downloadButton->setGeometry(this->width()-(5+m_ui->downloadButton->width())
                                    , this->height() - (5 + m_ui->downloadButton->height() + m_ui->menuBar->height())
                                    , m_ui->downloadButton->width(),m_ui->downloadButton->height());
}

void FileSharing_GUI::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);

    m_ui->treeWidget->setGeometry(5
                                  , 5
                                  , 180
                                  , this->height() - (20 + m_ui->scanNetworkButton->height()*2 + m_ui->menuBar->height()));

    m_ui->scanNetworkButton->setGeometry(5
                                  , 10+m_ui->treeWidget->height()
                                  , m_ui->scanNetworkButton->width()
                                  , m_ui->scanNetworkButton->height());

    m_ui->scanIpButton->setGeometry(5
                                    , m_ui->scanNetworkButton->y() + m_ui->scanNetworkButton->height() + 5
                                    , m_ui->scanNetworkButton->width()
                                    , m_ui->scanNetworkButton->height());

    m_ui->tableView->setGeometry(m_ui->treeWidget->width()+10
                                    , 5
                                    , this->width()-(m_ui->treeWidget->width()+15)
                                    , this->height()-(15 + m_ui->downloadButton->height() + + m_ui->menuBar->height()));

    m_ui->downloadButton->setGeometry(this->width()-(5+m_ui->downloadButton->width())
                                    , this->height() - (5 + m_ui->downloadButton->height()+m_ui->menuBar->height())
                                    , m_ui->downloadButton->width(),m_ui->downloadButton->height());

    m_loadingGif->setGeometry((m_ui->scanNetworkButton->width() + m_ui->treeWidget->width())/2 - 3
                                    , m_ui->scanIpButton->y()-5, 16,16);

    m_ui->selectAllButton->setGeometry(m_ui->downloadButton->x() - (5 + m_ui->selectAllButton->width())
                                    , m_ui->downloadButton->y()
                                    , m_ui->selectAllButton->width()
                                    , m_ui->selectAllButton->height());

    m_ui->selectNoneButton->setGeometry(m_ui->selectAllButton->x() - (5 + m_ui->selectNoneButton->width())
                                    , m_ui->downloadButton->y()
                                    , m_ui->selectNoneButton->width()
                                    , m_ui->selectNoneButton->height());
}

void FileSharing_GUI::initModelTableView()
{
    m_model->setHorizontalHeaderItem(0, new QStandardItem(QString("name")));
    m_model->setHorizontalHeaderItem(1, new QStandardItem(QString("progress")));
    m_model->setHorizontalHeaderItem(2, new QStandardItem(QString("download")));
    m_model->setHorizontalHeaderItem(3, new QStandardItem(QString("size (KB)")));
    m_ui->tableView->setModel(m_model.get());
}

int FileSharing_GUI::scanNetwork()
{
    //clear data into treeView
    m_ui->treeWidget->clear();

    m_ui->scanNetworkButton->setEnabled(false);
    startWaitAnimation();

    QString ipAddress = "";
    ipAddress = m_setNetworkGUI->load();

    qDebug()<<"IP address: " + ipAddress;
    if (ipAddress == "")
    {
        m_ui->scanNetworkButton->setEnabled(true);
        stopWaitAnimation();
        return 1;
    }

    QString networkIp ="";
    QString lastNum = "";
    int lastNumToInt = 0;
    int countDots = 0;
    for (int i=0; i<ipAddress.length(); i++)
    {
        if (ipAddress[i]=='.')
        {
            countDots+=1;
            if (countDots == 3) //FIXME: what is 3?
            {
                break;
            }
        }
        networkIp+=ipAddress[i];
    }

    for (int i = ipAddress.length()-1; ipAddress[i]!='.'; i--)
    {
        lastNum =ipAddress[i] + lastNum;
    }

    if (lastNum.length()>3 || lastNum.length()<1) //FIXME: what is 3 and 1?
    {
        stopWaitAnimation();
        return 1;
    }

    lastNumToInt = lastNum.toInt();

    if (countDots<3)
    {
        m_ui->scanNetworkButton->setEnabled(true);
        stopWaitAnimation();
        return 1;
    }

    networkIp+='.';
    m_threadPool->releaseThread();
    if (m_scanNetwork->getCurrentIP() == "")
    {
        m_scanNetwork->setCurrentIP(networkIp);
    }

    if (m_scanNetwork->m_scannedIPAddresses.size()==0)
    {
        for(int i=1; i<256; i++)
        {
            if (i == lastNumToInt)
            {
                m_scanNetwork->m_scannedIPAddresses.append(1);
                continue;
            }
            m_scanNetwork->m_scannedIPAddresses.append(0);
        }
    }
    else
    {
        for(int i=0; i<255; i++)
        {
            if (i == lastNumToInt-1)
            {
                m_scanNetwork->m_scannedIPAddresses[i]=1;
                continue;
            }
            m_scanNetwork->m_scannedIPAddresses[i]=0;
        }
    }

    m_scanNetwork->setAutoDelete(false);
    for (int i=1; i<256; i++)
    {
        m_threadPool->start(m_scanNetwork.get());
    }

    return 0;
}

//"Scan Network" button
void FileSharing_GUI::on_scanNetworkButton_clicked()
{
    while (m_model->rowCount() > 0)
    {
        m_model->removeRow(0);
    }

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


//ToDo: refactor the below function
void FileSharing_GUI::on_treeWidget_itemClicked(QTreeWidgetItem *item /*, const int column */)
{
    //change cursor
    QApplication::setOverrideCursor(Qt::WaitCursor);

    //remove data in tableView
    while (m_model->rowCount() > 0)
    {
        m_model->removeRow(0);
    }

    qDebug()<<item->text(0);
    m_currentHost = item->text(0);

    QTcpSocket socket;

    socket.connectToHost(item->text(0),26001);
    bool flag = socket.waitForConnected();
    if(!flag)
    {
        socket.close();
        qDebug()<<"Can't connect to " + item->text(0);
        QApplication::restoreOverrideCursor();
        return;
    }

    char *ncGetList = new char [3];
    ncGetList[0]=char(20);
    ncGetList[1]='\t';
    ncGetList[2]='\n';
    socket.write(ncGetList,3);

    QByteArray ba1;
    QString recv = "";
    while(socket.waitForReadyRead())
    {
        //qDebug()<<"-------------";
        ba1=socket.readAll();
        recv+=ba1.data();
        //qDebug()<<recv;
    }
    socket.close();
    qDebug()<<"size:"<<recv.size();
    if (recv == "")
    {
        QApplication::restoreOverrideCursor();
        return;
    }
    if(recv.length()<3)
    {
        QApplication::restoreOverrideCursor();
        return;
    }
    //qDebug()<<"recv:"<<recv;

    //fill tableView
    int i=2;
    QString text ="";
    QString size ="";
    QString name ="";
    do
    {
        i++;
        if (recv[i]=='#') //hash
        {
            size = text;
            text="";
            continue;
        }
        if (recv[i]=='\n') //name
        {
            name = text;
            addDataInTableView(name,size);
            name="";
            size="";
            text="";
            continue;
        }
        text+=recv[i];
    }
    while(i<recv.length());

    QApplication::restoreOverrideCursor();
}

void FileSharing_GUI::addDataInTableView(const QString &fileName, const QString &size)
{

    m_model->setRowCount(m_model->rowCount()+1);

    m_model->setData(m_model->index(m_model->rowCount()-1,0),fileName);
    m_model->setData(m_model->index(m_model->rowCount()-1,1),"");

    //checkBox
    std::shared_ptr<QStandardItem> item0 = std::make_shared<QStandardItem>(true);
    item0->setCheckable(true);
    item0->setCheckState(Qt::Unchecked);
    m_model->setItem(m_model->rowCount()-1,2,item0.get());

    double size_num = size.toInt();
    size_num/=1024.0;
    m_model->setData(m_model->index(m_model->rowCount()-1,3),QString::number(size_num));

    //set data not editable
    m_model->item(m_model->rowCount()-1,0)->setEditable(false);
    m_model->item(m_model->rowCount()-1,1)->setEditable(false);
    m_model->item(m_model->rowCount()-1,2)->setEditable(false);
    m_model->item(m_model->rowCount()-1,3)->setEditable(false);
}

int FileSharing_GUI::modifyPath(QString &path)
{
    if (path[path.length()-1] == '\\' ||path[path.length()-1] == '/' )
    {
        return 1;
    }

    for (int i = path.length()-1; i>=0; i--)
    {
        if (path[i] == '\\' || path[i] == '/')
        {
            path+=path[i];
            return 1;
        }
    }

    return 0;
}

void FileSharing_GUI::startWaitAnimation()
{
    m_movie->start();
    m_loadingGif->show();
}

void FileSharing_GUI::stopWaitAnimation()
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

    QString filename=m_model->item(index, 0)->text();
    int num=0;
    for (int i = 0; i<m_model->rowCount(); i++)
    {
        if (index == i)
        {
            break;
        }

        if (filename == m_model->item(i,0)->text())
        {
            num++;
        }
    }
    query+=QString::number(num) + "#" + filename+"\n";
    return query;
}

//"Download" button
void FileSharing_GUI::on_downloadButton_clicked()
{
    if (m_downloadButtonClicked)
    {
        return;
    }

    //clear all data in progress column
    for (int i=0; i<m_model->rowCount(); i++)
    {
        m_model->item(i, 1)->setText("");
    }

    m_downloadButtonClicked = true;
    QModelIndex index;
    for (int i=0; i<m_model->rowCount(); i++)
    {
        index = m_model->index(i,2, QModelIndex());
        if(index.data(Qt::CheckStateRole) == Qt::Checked)
        {
            QString dir = m_setDirGUI->load();
            if (modifyPath(dir)==0)
            {
                   dir = QDir::currentPath();
            }
            QString query = setQuery(i);
            if (query == "")
            {
                continue;
            }
            double sizeDouble = m_model->item(i,3)->text().toDouble();
            qint64 size = sizeDouble * 1000;
            std::shared_ptr<Client> thread  = std::make_shared<Client>(m_currentHost,query, m_model->item(i,0)->text(),dir,size,i);
            connect(thread.get(), SIGNAL( setProgress(int,double) ), this, SLOT( on_setProgress(int,double) ));
            thread->start();
            return;
        }
    }

    m_downloadButtonClicked = false;
}

void FileSharing_GUI::next(const int row)
{
    QModelIndex index;
    for (int i=row+1; i<m_model->rowCount(); i++)
    {

        index= m_model->index(i,2, QModelIndex());
        if(index.data(Qt::CheckStateRole) == Qt::Checked)
        {
            QString dir = m_setDirGUI->load();
            if (modifyPath(dir)==0)
            {
                   dir = QDir::currentPath();
            }

            QString query = setQuery(i);
            if (query == "")
            {
                continue;
            }

            double sizeDouble = m_model->item(i,3)->text().toDouble();
            qint64 size = sizeDouble *1000;
            std::shared_ptr<Client> thread  = std::make_shared<Client>(m_currentHost, query, m_model->item(i,0)->text(),dir,size,i);
            connect(thread.get(), SIGNAL(setProgress(int,double)), this, SLOT(on_setProgress(int,double)));
            thread->start();
            return;
        }
    }
    m_downloadButtonClicked = false;
}

void FileSharing_GUI::on_setProgress(const int row, const double percentage)
{
    if (percentage == 100 || percentage == -1)
    {
        if (percentage == -1)
        {
            m_model->item(row, 1)->setText("Fail to download");
        }
        else
        {
            m_model->item(row, 1)->setText(QString::number(percentage) + "%");
        }

        next(row);
        return;
    }
    if (percentage<100)
    {
        m_model->item(row, 1)->setText(QString::number(percentage) + "%");
    }
}

//"Scan IP" button
void FileSharing_GUI::on_scanIpButton_clicked()
{
    m_scanIpGUI->show();
}

//When given IP using this program, "scanip_gui.cpp" emit this funtion.
void FileSharing_GUI::on_scanIP(const QString &ip)
{
    if (ip!="")
    {
        addItemToThreeView(ip);
    }
}

//"Select All" button
void FileSharing_GUI::on_selectAllButton_clicked()
{
    for (int i=0; i<m_model->rowCount(); i++)
    {
        m_model->item(i, 2)->setCheckState(Qt::Checked);
    }
}

//"Select None" button
void FileSharing_GUI::on_selectNoneButton_clicked()
{
    for (int i=0; i<m_model->rowCount(); i++)
    {
        m_model->item(i, 2)->setCheckState(Qt::Unchecked);
    }
}
