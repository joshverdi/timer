#include <QPushButton>
#include "countdowndialog.h"
#include "ui_countdowndialog.h"

CountdownDialog::CountdownDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CountdownDialog)
{
    ui->setupUi(this);
    ui->spinBoxCenti->setWrapping(true);
    ui->spinBoxSecond->setWrapping(true);
    ui->spinBoxMinute->setWrapping(true);
    ui->spinBoxHour->setWrapping(true);
}

CountdownDialog::~CountdownDialog()
{
    delete ui;
}

void CountdownDialog::on_buttonBox_clicked(QAbstractButton *button)
{
    if (button == ui->buttonBox->button(QDialogButtonBox::Cancel))
    {
        close();
    }
    else if (button == ui->buttonBox->button(QDialogButtonBox::Ok))
    {
        close();
    }
}

int CountdownDialog::gethour()
{
    return ui->spinBoxHour->value();
}

int CountdownDialog::getminute()
{
    return ui->spinBoxMinute->value();
}

int CountdownDialog::getsec()
{
    return ui->spinBoxSecond->value();
}

int CountdownDialog::getcentisec()
{
    return ui->spinBoxCenti->value();
}
