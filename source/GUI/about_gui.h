#ifndef ABOUT_GUI_H
#define ABOUT_GUI_H

#include <memory>

#include <QDesktopWidget>
#include <QScreen>
#include <QWidget>

namespace Ui {
class About_GUI;
}

class About_GUI : public QWidget
{
    Q_OBJECT

public:
    explicit About_GUI(QWidget *parent = nullptr);
    ~About_GUI();

private:
    std::shared_ptr<Ui::About_GUI> m_ui;

    void setupGui ();
};

#endif // ABOUT_GUI_H
