#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStringListModel>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>

#include "graphicsview.h"
#include "countdowndialog.h"
#include "aboutdialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *_parent = 0);
    ~MainWindow();

private slots:
    void on_actionClose_triggered();

    void on_pushButtonReset_clicked();

    void on_pushButtonLap_clicked();

    void on_actionCountdown_triggered();

    void on_actionAbout_triggered();

private:
    Ui::MainWindow *ui;
    GraphicsView* gview;
    CountdownDialog* cddialog;
    AboutDialog* about;
    QStringListModel* model;

    std::shared_ptr<NTouchClock> ntclock;
};

#endif // MAINWINDOW_H
