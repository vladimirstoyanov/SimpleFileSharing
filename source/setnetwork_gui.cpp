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
    m_ui(std::make_shared <Ui::SetNetwork_GUI> ())
{
    m_ui->setupUi(this);
    addNetworksToComboBox ();
    this->setFixedSize(this->width(), this->height());
}

SetNetwork_GUI::~SetNetwork_GUI()
{
}

//"OK" button
void SetNetwork_GUI::on_okButton_clicked()
{
    save();
    this->hide();
}

//"Cancel" button
void SetNetwork_GUI::on_cancelButton_clicked()
{
    this->hide();
}

//Get avaliable networks and add it to combo box
void SetNetwork_GUI::addNetworksToComboBox()
{
    //clear data into treeView
    m_ui->comboBox->clear();

    //get IP address
    foreach (const QHostAddress &address, QNetworkInterface::allAddresses())
    {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != QHostAddress(QHostAddress::LocalHost))
        {
             QString ipAddress = address.toString();
             if (ipAddress.length()<3)
             {
                 continue;
             }
             if (ipAddress[0]=='1' && ipAddress[1]=='6' && ipAddress[2]=='9')
             {
                 continue;
             }
             m_ui->comboBox->addItem(address.toString());
        }
    }
}

void SetNetwork_GUI::showEvent(QShowEvent *)
{
    m_ui->comboBox->clear();
    load();
}

void SetNetwork_GUI::save()
{
    QFile file( "settings1" );
    if ( file.open(QIODevice::ReadWrite) )
    {
        QTextStream stream(&file);
        stream<<m_ui->comboBox->currentText();
        file.close();
    }
}

QString SetNetwork_GUI::load()
{
    addNetworksToComboBox();
    QFile file("settings1");
    if(!file.open(QIODevice::ReadOnly))
    {
        if (m_ui->comboBox->count()>0)
        {
            m_ui->comboBox->setCurrentIndex(0);
            return m_ui->comboBox->itemText(0);
        }
        return "";
    }

    QTextStream in(&file);
    QString line = in.readLine();
    file.close();

    //it checks for exist hosts
    for (int i=0; i<m_ui->comboBox->count(); i++)
    {
        if (m_ui->comboBox->itemText(i) == line)
        {
            m_ui->comboBox->setCurrentIndex(i);
            return line;
        }
    }

    if (m_ui->comboBox->count()>0)
    {
        m_ui->comboBox->setCurrentIndex(0);
        return m_ui->comboBox->itemText(0);
    }

    return "";
}
