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

#ifndef SCANIP_GUI_H
#define SCANIP_GUI_H

#include <QDialog>
#include <QMovie>
#include <QLabel>

#include "scan_network.h"

/*
 This gui class appear when click on 'Scan IP' button from main window.
*/

namespace Ui {
class ScanIP_GUI;
}

class ScanIP_GUI : public QDialog
{
    Q_OBJECT

public:
    explicit ScanIP_GUI(QWidget *parent = 0);
    ~ScanIP_GUI();

private:
    Ui::ScanIP_GUI *ui;

signals:
    void scanIP(QString);
private slots:
    void on_pushButton_clicked();
};

#endif // SCANIP_GUI_H
