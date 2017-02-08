#ifndef COUNTDOWNDIALOG_H
#define COUNTDOWNDIALOG_H

#include <QDialog>
#include <QAbstractButton>

namespace Ui {
class CountdownDialog;
}

class CountdownDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CountdownDialog(QWidget *parent = 0);
    ~CountdownDialog();
    int gethour();
    int getminute();
    int getsec();
    int getcentisec();

private slots:
    void on_buttonBox_clicked(QAbstractButton *button);

private:
    Ui::CountdownDialog *ui;
};

#endif // COUNTDOWNDIALOG_H
