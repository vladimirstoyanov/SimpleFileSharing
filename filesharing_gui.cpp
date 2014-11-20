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
    QMainWindow(parent),
    ui(new Ui::FileSharing_GUI)
{
    ui->setupUi(this);

    //thread pool used to scanning a network
    tp = new QThreadPool(this);
    tp->setMaxThreadCount(50); //check 50 IP addresses at the same time

    m_AddFile_Gui = new AddFile_GUI();

    setGeometryOfWidgets();

    initModelTableView();

    server.StartServer();

    //init menu actions
    initActions();

    current_host = "";

    sd_g = new SetDir_GUI ();
    sn_g = new SetNetwork_GUI();
    sip_g = new ScanIP_GUI ();

    //'scanip_gui.cpp' make connect with this class, when given IP using this program.
    connect(sip_g,SIGNAL(scanIP(QString)),this,SLOT(on_scanIP(QString)));

    ui->treeWidget->setHeaderLabel("Avaliable Hosts");

    //init waiting gif animation
    movie = new QMovie("ajax_waiting.gif");
    l_waiting_gif = new QLabel(this);
    l_waiting_gif->setMovie(movie);

    sn = new scan_network();

    //when found computer
    connect(sn,SIGNAL(foundComputer(QString)),this,SLOT(onFoundComputer(QString)));

    //when a scanner finished
    connect(sn,SIGNAL(finishScan()),this,SLOT(on_finishScan()));

    download_button_clicked = false;
}

FileSharing_GUI::~FileSharing_GUI()
{
    delete ui;
    delete model;
    delete tp;
    sn->deleteLater();
    delete sd_g;
    delete sn_g;
    delete sip_g;
    delete movie;
    delete l_waiting_gif;
}

void FileSharing_GUI::onFoundComputer(QString ip)
{
    addItemToThreeView(ip);
}

void FileSharing_GUI::on_finishScan()
{
    ui->pushButton->setEnabled(true);
    qDebug()<<"The network scan has finished.";
    stopWaitAnimation();
}

//add item to treeview
void FileSharing_GUI::addItemToThreeView(QString name)
{
    for (int i=0; i<ui->treeWidget->topLevelItemCount(); i++)
        if (ui->treeWidget->topLevelItem(i)->text(0) == name)
                return;

    QTreeWidgetItem *item = new QTreeWidgetItem();
    item->setText(0,name);
    ui->treeWidget->addTopLevelItem(item);
}

//set geometry of widgets
void FileSharing_GUI::setGeometryOfWidgets()
{
    ui->pushButton->setGeometry(5,5,ui->pushButton->width(), ui->pushButton->height());
    ui->treeWidget->setGeometry(5,10+ui->pushButton->height(),180, this->height() - (15 + ui->pushButton->height()));
    ui->tableView->setGeometry(ui->treeWidget->width()+10,5,this->width()-(ui->treeWidget->width()+15),this->height()-(15 + ui->pushButton_2->height() + +ui->menuBar->height()));
    ui->pushButton_2->setGeometry(this->width()-(5+ui->pushButton_2->width()),this->height() - (5 + ui->pushButton_2->height() + ui->menuBar->height()),ui->pushButton_2->width(),ui->pushButton_2->height());
}

void FileSharing_GUI::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    ui->treeWidget->setGeometry(5,5,180, this->height() - (20 + ui->pushButton->height()*2 + ui->menuBar->height()));
    ui->pushButton->setGeometry(5,10+ui->treeWidget->height(),ui->pushButton->width(), ui->pushButton->height());
    ui->pushButton_3->setGeometry(5,ui->pushButton->y() + ui->pushButton->height() + 5,ui->pushButton->width(), ui->pushButton->height());
    ui->tableView->setGeometry(ui->treeWidget->width()+10,5,this->width()-(ui->treeWidget->width()+15),this->height()-(15 + ui->pushButton_2->height() + +ui->menuBar->height()));
    ui->pushButton_2->setGeometry(this->width()-(5+ui->pushButton_2->width()),this->height() - (5 + ui->pushButton_2->height()+ui->menuBar->height()),ui->pushButton_2->width(),ui->pushButton_2->height());
    l_waiting_gif->setGeometry((ui->pushButton->width() + ui->treeWidget->width())/2 - 3,ui->pushButton_3->y()-5, 16,16);
    ui->pushButton_4->setGeometry(ui->pushButton_2->x() - (5 + ui->pushButton_4->width()), ui->pushButton_2->y(), ui->pushButton_4->width(),ui->pushButton_4->height());
    ui->pushButton_5->setGeometry(ui->pushButton_4->x() - (5 + ui->pushButton_5->width()), ui->pushButton_2->y(), ui->pushButton_5->width(),ui->pushButton_5->height());
}

void FileSharing_GUI::initModelTableView()
{
    model = new QStandardItemModel(0,4,this);
    model->setHorizontalHeaderItem(0, new QStandardItem(QString("name")));
    model->setHorizontalHeaderItem(1, new QStandardItem(QString("progress")));
    model->setHorizontalHeaderItem(2, new QStandardItem(QString("download")));
    model->setHorizontalHeaderItem(3, new QStandardItem(QString("size (KB)")));
    ui->tableView->setModel(model);
}

int FileSharing_GUI::scanNetwork()
{
    //clear data into treeView
    ui->treeWidget->clear();

    ui->pushButton->setEnabled(false);
    startWaitAnimation();

    QString ip_address = "";
    ip_address = sn_g->load();

    qDebug()<<"IP address: " + ip_address;
    if (ip_address == "")
    {
        ui->pushButton->setEnabled(true);
        stopWaitAnimation();
        return 1;
    }

    QString network_ip ="";
    QString last_num = "";
    int last_num_to_int = 0;
    int count_dots = 0;
    for (int i=0; i<ip_address.length(); i++)
    {
        if (ip_address[i]=='.')
        {
            count_dots+=1;
            if (count_dots == 3)
                break;
        }
        network_ip+=ip_address[i];
    }

    for (int i = ip_address.length()-1; ip_address[i]!='.'; i--)
        last_num =ip_address[i] + last_num;

    if (last_num.length()>3 || last_num.length()<1)
    {
        stopWaitAnimation();
        return 1;
    }
    last_num_to_int = last_num.toInt();

    if (count_dots<3)
    {
        ui->pushButton->setEnabled(true);
        stopWaitAnimation();
        return 1;
    }

    network_ip+='.';
    tp->releaseThread();
    if (sn->current_ip == "")
        sn->current_ip = network_ip;

    if (sn->is_scanned.size()==0)
        for(int i=1; i<256; i++)
        {
            if (i == last_num_to_int)
            {
                sn->is_scanned.append(1);
                continue;
            }
            sn->is_scanned.append(0);
        }
    else
        for(int i=0; i<255; i++)
        {
            if (i == last_num_to_int-1)
            {
                sn->is_scanned[i]=1;
                continue;
            }
            sn->is_scanned[i]=0;
        }


    sn->setAutoDelete(false);
    for (int i=1; i<256; i++)
        tp->start(sn);

    return 0;
}

//'Scan Network' button
void FileSharing_GUI::on_pushButton_clicked()
{
    while (model->rowCount() > 0)
    {
        model->removeRow(0);
    }
    scanNetwork();
}



void FileSharing_GUI::initActions()
{
    ui->actionFilie_or_Directory->setShortcut(QKeySequence::New);
    ui->actionFilie_or_Directory->setStatusTip(tr("File"));
    connect(ui->actionFilie_or_Directory, SIGNAL(triggered()), this, SLOT(menu_addFile()));
    connect(ui->actionDownloaded_Directory, SIGNAL(triggered()), this, SLOT(menu_downloadedDirectory()));
    connect(ui->actionSet_network, SIGNAL(triggered()), this, SLOT(menu_setNetwork()));
}

void FileSharing_GUI::menu_addFile()
{
    m_AddFile_Gui->show();
}

//set dir (downloadedDirectory)
void FileSharing_GUI::menu_downloadedDirectory()
{
    sd_g->show();
}

void FileSharing_GUI::menu_setNetwork()
{
    sn_g->show();
}

void FileSharing_GUI::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
    //change cursor
    QApplication::setOverrideCursor(Qt::WaitCursor);

    //remove data in tableView
    while (model->rowCount() > 0)
        model->removeRow(0);

    qDebug()<<item->text(0);
    current_host = item->text(0);

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

    char *nc_get_list = new char [3];
    nc_get_list[0]=char(20);
    nc_get_list[1]='\t';
    nc_get_list[2]='\n';
    socket.write(nc_get_list,3);

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

void FileSharing_GUI::addDataInTableView(QString file_name, QString size)
{

    model->setRowCount(model->rowCount()+1);

    model->setData(model->index(model->rowCount()-1,0),file_name);
    model->setData(model->index(model->rowCount()-1,1),"");

    //checkBox
    QStandardItem *item0 = new QStandardItem(true);
    item0->setCheckable(true);
    item0->setCheckState(Qt::Unchecked);
    model->setItem(model->rowCount()-1,2,item0);

    double size_num = size.toInt();
    size_num/=1024.0;
    model->setData(model->index(model->rowCount()-1,3),QString::number(size_num));

    //set data not editable
    model->item(model->rowCount()-1,0)->setEditable(false);
    model->item(model->rowCount()-1,1)->setEditable(false);
    model->item(model->rowCount()-1,2)->setEditable(false);
    model->item(model->rowCount()-1,3)->setEditable(false);
}

int FileSharing_GUI::modifyPath(QString &path)
{
    if (path[path.length()-1] == '\\' ||path[path.length()-1] == '/' )
        return 1;

    for (int i = path.length()-1; i>=0; i--)
        if (path[i] == '\\' || path[i] == '/')
        {
            path+=path[i];
            return 1;
        }

    return 0;
}

void FileSharing_GUI::startWaitAnimation()
{
    movie->start();
    l_waiting_gif->show();
}

void FileSharing_GUI::stopWaitAnimation()
{
    movie->stop();
    l_waiting_gif->hide();
}

QString FileSharing_GUI::setQuery(int index)
{
    if (index>=model->rowCount())
        return "";
    QString query = "\x10\t";

    QString filename=model->item(index, 0)->text();
    int num=0;
    for (int i = 0; i<model->rowCount(); i++)
    {
        if (index == i)
            break;

        if (filename == model->item(i,0)->text())
            num++;
    }
    query+=QString::number(num) + "#" + filename+"\n";
    return query;
}

//'Download' button
void FileSharing_GUI::on_pushButton_2_clicked()
{
    if (download_button_clicked)
        return;

    //clear all data in progress column
    for (int i=0; i<model->rowCount(); i++)
        model->item(i, 1)->setText("");

    download_button_clicked = true;
    QModelIndex index;
    for (int i=0; i<model->rowCount(); i++)
    {

        index= model->index(i,2, QModelIndex());
        if(index.data(Qt::CheckStateRole) == Qt::Checked)
        {
            QString dir = sd_g->load();
            if (modifyPath(dir)==0)
                   dir = QDir::currentPath();
            QString query = setQuery(i);
            if (query == "")
                continue;
            double size_double = model->item(i,3)->text().toDouble();
            qint64 size = size_double *1000;
            Client *thread  = new Client(current_host,query/*model->item(i,4)->text()*/,model->item(i,0)->text(),dir,size,i);
            connect(thread, SIGNAL( setProgress(int,double) ), this, SLOT( on_setProgress(int,double) ));
            thread->start();
            return;
        }
    }
    download_button_clicked = false;
}

void FileSharing_GUI::next(int row)
{
    QModelIndex index;
    for (int i=row+1; i<model->rowCount(); i++)
    {

        index= model->index(i,2, QModelIndex());
        if(index.data(Qt::CheckStateRole) == Qt::Checked)
        {
            QString dir = sd_g->load();
            if (modifyPath(dir)==0)
                   dir = QDir::currentPath();

            QString query = setQuery(i);
            if (query == "")
                continue;

            double size_double = model->item(i,3)->text().toDouble();
            qint64 size = size_double *1000;
            Client *thread  = new Client(current_host, query/*model->item(i,4)->text()*/,model->item(i,0)->text(),dir,size,i);
            connect(thread, SIGNAL(setProgress(int,double)), this, SLOT(on_setProgress(int,double)));
            thread->start();
            return;
        }
    }
    download_button_clicked = false;
}

void FileSharing_GUI::on_setProgress(int row, double percentage)
{
    if (percentage == 100 || percentage == -1)
    {
        if (percentage == -1)
            model->item(row, 1)->setText("Fail to download");
        else
            model->item(row, 1)->setText(QString::number(percentage) + "%");
        next(row);
        return;
    }
    if (percentage<100)
        model->item(row, 1)->setText(QString::number(percentage) + "%");
}

//'Scan IP' button
void FileSharing_GUI::on_pushButton_3_clicked()
{
    sip_g->show();
}

//When given IP using this program, 'scanip_gui.cpp' emit this funtion.
void FileSharing_GUI::on_scanIP(QString ip)
{
    if (ip!="")
        addItemToThreeView(ip);
}

//'Select All' button
void FileSharing_GUI::on_pushButton_4_clicked()
{
    for (int i=0; i<model->rowCount(); i++)
        model->item(i, 2)->setCheckState(Qt::Checked);
}

//'Select None' button
void FileSharing_GUI::on_pushButton_5_clicked()
{
    for (int i=0; i<model->rowCount(); i++)
        model->item(i, 2)->setCheckState(Qt::Unchecked);
}
