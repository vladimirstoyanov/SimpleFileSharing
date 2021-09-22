#include "about_gui.h"
#include "ui_about_gui.h"

About_GUI::About_GUI(QWidget *parent) :
    QWidget(parent),
    m_ui(std::make_shared<Ui::About_GUI> ())
{
    setupGui();
}

About_GUI::~About_GUI()
{
}

void About_GUI::setupGui ()
{
    m_ui->setupUi(this);

    this->adjustSize();
    this->move(QApplication::desktop()->screen()->rect().center() - this->rect().center());

    m_ui->textBrowser->setGeometry(0,0,this->width(), this->height());
    m_ui->textBrowser->setHtml("Simple File Sharing written by Vladimir Stoyanov.<p>e-mail: <a href=\"mailto:vlado_stoyanov@yahoo.com\">vlado_stoyanov@yahoo.com</p>");
    m_ui->textBrowser->setOpenLinks(1);
    m_ui->textBrowser->setOpenExternalLinks(1);
}
