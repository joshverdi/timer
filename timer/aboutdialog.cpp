#include "aboutdialog.h"
#include "ui_aboutdialog.h"

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
    //ui->labelLogo->setStyleSheet("background-image: url(:/img/timer_icon_07.png);");
    ui->labelLogo->setPixmap(QPixmap(QString::fromUtf8(":/img/icon128.png")));
    ui->labelNtLogo->setPixmap(QPixmap(QString::fromUtf8(":/img/ntlogo192.png")));
}

AboutDialog::~AboutDialog()
{
    delete ui;
}

void AboutDialog::on_pushButtonClose_clicked()
{
    close();
}
