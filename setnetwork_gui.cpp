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

#include "setnetwork_gui.h"
#include "ui_setnetwork_gui.h"
#include <QHostAddress>
#include <QNetworkInterface>
#include <QFile>

SetNetwork_GUI::SetNetwork_GUI(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SetNetwork_GUI)
{
    ui->setupUi(this);
    addNetworksToComboBox ();
    this->setFixedSize(this->width(), this->height());
}

SetNetwork_GUI::~SetNetwork_GUI()
{
    delete ui;
}

//'OK' button
void SetNetwork_GUI::on_pushButton_clicked()
{
    save();
    this->hide();
}

//'Cancel' button
void SetNetwork_GUI::on_pushButton_2_clicked()
{
    this->hide();
}

//Get avaliable networks and add it to combo box
void SetNetwork_GUI::addNetworksToComboBox()
{
    //clear data into treeView
    ui->comboBox->clear();

    //get IP address
    foreach (const QHostAddress &address, QNetworkInterface::allAddresses()) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != QHostAddress(QHostAddress::LocalHost))
        {
             QString ip_address = address.toString();
             if (ip_address.length()<3)
                 continue;
             if (ip_address[0]=='1' && ip_address[1]=='6' && ip_address[2]=='9')
                 continue;
             ui->comboBox->addItem(address.toString());
        }
    }
}

void SetNetwork_GUI::showEvent(QShowEvent *)
{
    ui->comboBox->clear();
    load();
}

void SetNetwork_GUI::save()
{
    QFile file( "settings1" );
    if ( file.open(QIODevice::ReadWrite) )
    {
        QTextStream stream(&file);
        stream<<ui->comboBox->currentText();
        file.close();
    }
}

QString SetNetwork_GUI::load()
{
    addNetworksToComboBox();
    QFile file("settings1");
    if(!file.open(QIODevice::ReadOnly)) {
        if (ui->comboBox->count()>0)
        {
            ui->comboBox->setCurrentIndex(0);
            return ui->comboBox->itemText(0);
        }
        return "";
    }

    QTextStream in(&file);
    QString line = in.readLine();
    file.close();

    //check for existing ip
    for (int i=0; i<ui->comboBox->count(); i++)
    {
        if (ui->comboBox->itemText(i) == line)
        {
            ui->comboBox->setCurrentIndex(i);
            return line;
        }
    }
    if (ui->comboBox->count()>0)
    {
        ui->comboBox->setCurrentIndex(0);
        return ui->comboBox->itemText(0);
    }

    return ""; //It haven't any network
}
