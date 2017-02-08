#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QCloseEvent>

MainWindow::MainWindow(QWidget *_parent) :
    QMainWindow(_parent),
    ui(new Ui::MainWindow)
{
    ntclock = std::make_shared<NTouchClock>();
    ui->setupUi(this);
    cddialog = new CountdownDialog(this);
    gview = new GraphicsView(ntclock, this);
    about =new AboutDialog(this);
    model = new QStringListModel(this);
    ui->listView->setModel(model);
    ui->verticalLayoutGViewer->addWidget(gview);
    setWindowIcon(QIcon(":/img/icon.png"));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionClose_triggered()
{
    QApplication::quit();
}


void MainWindow::on_pushButtonReset_clicked()
{
    gview->reset();
    model->removeRows(0, model->rowCount());
}

void MainWindow::on_pushButtonLap_clicked()
{
    if (gview->getchrono()==true) // Chrono ON
    {
        model->insertRow(model->rowCount());
        QModelIndex index = model->index(model->rowCount()-1);
        model->setData(index, QString(gview->lap().c_str()));
    }
}

void MainWindow::on_actionCountdown_triggered()
{
    //cddialog->show();
    if (cddialog->exec()==QDialog::Accepted)
    {
        gview->init_countdown(cddialog->gethour(), cddialog->getminute(), cddialog->getsec(), cddialog->getcentisec());
        gview->setcountdown(true);
    }
}

void MainWindow::on_actionAbout_triggered()
{
    about->show();
}
