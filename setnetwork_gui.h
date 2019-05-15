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

#ifndef SETNETWORK_GUI_H
#define SETNETWORK_GUI_H

#include <memory>

#include <QDialog>

/*
  menu: "Settings"->"Set network"
*/

namespace Ui {
class SetNetwork_GUI;
}

class SetNetwork_GUI : public QDialog
{
    Q_OBJECT

public:
    explicit SetNetwork_GUI(QWidget *parent = 0);
    virtual ~SetNetwork_GUI();

    QString load();

private slots:
    void addNetworksToComboBox();
    void on_okButton_clicked(); //"OK" button
    void on_cancelButton_clicked(); //"Cancel" button

private:
    std::shared_ptr<Ui::SetNetwork_GUI> m_ui;

private:
    void showEvent(QShowEvent *);
    void save ();

};

#endif // SETNETWORK_GUI_H
