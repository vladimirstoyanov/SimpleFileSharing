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

ScanIP_GUI::ScanIP_GUI(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ScanIP_GUI)
{
    ui->setupUi(this);

    this->setFixedSize(this->width(), this->height());
    ui->lineEdit->setAlignment(Qt::AlignCenter);
    ui->lineEdit_2->setAlignment(Qt::AlignCenter);
    ui->lineEdit_3->setAlignment(Qt::AlignCenter);
    ui->lineEdit_4->setAlignment(Qt::AlignCenter);


    QRegExp rx ( "^(0|[1-9]|[1-9][0-9]|1[0-9][0-9]|2([0-4][0-9]|5[0-5]))$" );
    QValidator *validator = new QRegExpValidator(rx, ui->lineEdit);
    ui->lineEdit->setValidator( validator );
    QValidator *validator1 = new QRegExpValidator(rx,  ui->lineEdit_2);
    ui->lineEdit_2->setValidator( validator1 );
    QValidator *validator2 = new QRegExpValidator(rx,  ui->lineEdit_3);
    ui->lineEdit_3->setValidator( validator2 );
    QValidator *validator3 = new QRegExpValidator(rx, ui->lineEdit_4);
    ui->lineEdit_4->setValidator( validator3 );
}

ScanIP_GUI::~ScanIP_GUI()
{
    delete ui;
}

void ScanIP_GUI::on_pushButton_clicked()
{
    //change cursor
    QApplication::setOverrideCursor(Qt::WaitCursor);

    QString ip = ui->lineEdit->text() + "." + ui->lineEdit_2->text() + "." + ui->lineEdit_3->text() + "." + ui->lineEdit_4->text();
    if (ui->lineEdit->text() == "" || ui->lineEdit_2->text() == "" || ui->lineEdit_3->text() == "" || ui->lineEdit_4->text() == "")
    {
        QApplication::restoreOverrideCursor();
        QMessageBox::critical(0,"Error!", "Wrong IP address: " + ip);
        return;
    }

    scan_network sn;
    if (!sn.scanIP(ip))
    {
        emit scanIP(ip);
        this->hide();
    }
    else
    {
        QApplication::restoreOverrideCursor();
        QMessageBox::critical(0,"Error!", "Couldn't connect to: " + ip);
    }
    QApplication::restoreOverrideCursor();

}
