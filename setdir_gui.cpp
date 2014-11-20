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

#include "setdir_gui.h"
#include "ui_setdir_gui.h"
#include <QFileDialog>
#include <QDebug>
#include <QDir>

SetDir_GUI::SetDir_GUI(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SetDir_GUI)
{
    ui->setupUi(this);
    this->setFixedSize(this->width(), this->height());
}

SetDir_GUI::~SetDir_GUI()
{
    delete ui;
}

//'OK' button
void SetDir_GUI::on_pushButton_3_clicked()
{
    save();
    this->hide();
}

//'Choose Directory' button clicked
void SetDir_GUI::on_pushButton_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                 "/home",
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks);
    ui->lineEdit->setText(dir);
}

void SetDir_GUI::save()
{
    QFile file("settings_dir");
    if ( file.open(QIODevice::WriteOnly) )
    {
        QTextStream stream(&file);

        stream<<ui->lineEdit->text();
        file.close();
    }
}

QString SetDir_GUI::load()
{
    QFile file("settings_dir");

    if(!file.open(QIODevice::ReadOnly)) {
        ui->lineEdit->setText(QDir::currentPath());
        return QDir::currentPath();
    }

    QTextStream in(&file);
    QString line = in.readLine();
    file.close();
    if (!QDir(line).exists())
    {
        qDebug()<<"Directory:" + line + " not exist!";
        ui->lineEdit->setText(QDir::currentPath());
        return QDir::currentPath();
    }
    ui->lineEdit->setText(line);
    return line;

}

void SetDir_GUI::showEvent(QShowEvent *event)
{
    load();
}
