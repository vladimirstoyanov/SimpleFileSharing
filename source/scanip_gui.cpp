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

#include "scanip_gui.h"
#include "ui_scanip_gui.h"
#include <QMessageBox>
#include <QFrame>
#include <QLineEdit>
#include <QIntValidator>
#include <QHBoxLayout>
#include <QFont>
#include <QLabel>
#include <QKeyEvent>

#include <memory>

ScanIP_GUI::ScanIP_GUI(QWidget *parent) :
    QDialog(parent)
    , m_ui(std::make_shared<Ui::ScanIP_GUI> ())
{
    m_ui->setupUi(this);

    this->setFixedSize(this->width(), this->height());
    m_ui->lineEdit->setAlignment(Qt::AlignCenter);
    m_ui->lineEdit_2->setAlignment(Qt::AlignCenter);
    m_ui->lineEdit_3->setAlignment(Qt::AlignCenter);
    m_ui->lineEdit_4->setAlignment(Qt::AlignCenter);


    QRegExp rx ( "^(0|[1-9]|[1-9][0-9]|1[0-9][0-9]|2([0-4][0-9]|5[0-5]))$" );
    std::shared_ptr<QValidator> validator = std::make_shared<QRegExpValidator>(rx, m_ui->lineEdit);
    m_ui->lineEdit->setValidator( validator.get() );
    std::shared_ptr<QValidator> validator1 = std::make_shared<QRegExpValidator>(rx,  m_ui->lineEdit_2);
    m_ui->lineEdit_2->setValidator( validator1.get() );
    std::shared_ptr<QValidator> validator2 = std::make_shared<QRegExpValidator>(rx,  m_ui->lineEdit_3);
    m_ui->lineEdit_3->setValidator( validator2.get() );
    std::shared_ptr<QValidator> validator3 = std::make_shared<QRegExpValidator>(rx, m_ui->lineEdit_4);
    m_ui->lineEdit_4->setValidator( validator3.get() );
}

ScanIP_GUI::~ScanIP_GUI()
{
}

void ScanIP_GUI::on_okButton_clicked()
{
    //change cursor
    QApplication::setOverrideCursor(Qt::WaitCursor);

    QString ip = m_ui->lineEdit->text() + "." + m_ui->lineEdit_2->text() + "." + m_ui->lineEdit_3->text() + "." + m_ui->lineEdit_4->text();
    if (m_ui->lineEdit->text() == "" || m_ui->lineEdit_2->text() == "" || m_ui->lineEdit_3->text() == "" || m_ui->lineEdit_4->text() == "")
    {
        QApplication::restoreOverrideCursor();
        QMessageBox::critical(nullptr,"Error!", "Wrong IP address: " + ip);
        return;
    }

    ScanNetwork sn;
    if (!sn.scanIP(ip))
    {
        emit scanIP(ip);
        this->hide();
    }
    else
    {
        QApplication::restoreOverrideCursor();
        QMessageBox::critical(nullptr,"Error!", "Couldn't connect to: " + ip);
    }
    QApplication::restoreOverrideCursor();
}
