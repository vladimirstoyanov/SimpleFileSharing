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

#ifndef SETDIR_GUI_H
#define SETDIR_GUI_H

#include <memory>

#include <QDialog>

namespace Ui
{
    class SetDir_GUI;
}

/*
  menu: "Settings"->"Download directory"
*/

class SetDir_GUI : public QDialog
{
    Q_OBJECT

public:
    explicit SetDir_GUI(QWidget *parent = 0);
    virtual ~SetDir_GUI();
    QString load();

private:
    std::shared_ptr<Ui::SetDir_GUI> m_ui;

    void save ();
    void showEvent( QShowEvent *event );

private slots:
    void on_chooseDirectoryButton_clicked();
    void on_okButton_clicked(); //"OK" button
};

#endif // SETDIR_GUI_H
