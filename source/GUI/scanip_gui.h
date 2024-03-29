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

#include <memory>

#include <QDialog>
#include <QLabel>
#include <QMovie>

#include "network_manager.h"
#include "scan_network_thread.h"

/*
 This gui class is shown when the user clicked on 'Scan IP' button from main window.
*/

namespace Ui
{
    class ScanIP_GUI;
}

class ScanIP_GUI : public QDialog
{
    Q_OBJECT

public:
    explicit ScanIP_GUI(QWidget *parent = 0);
    virtual ~ScanIP_GUI();

private:
    unsigned int                     m_maxHostCount;
    std::shared_ptr <Ui::ScanIP_GUI> m_ui;

private slots:
    void on_okButton_clicked();

signals:
    void scanIP(const QString &);
};

#endif // SCANIP_GUI_H
