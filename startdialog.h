#ifndef STARTDIALOG_H
#define STARTDIALOG_H

#include <QDialog>
#include "filemanager.h"
#include "mainwindow.h"

namespace Ui {
class StartDialog;
}

class StartDialog : public QDialog
{
    Q_OBJECT

public:
    explicit StartDialog(QWidget *parent = 0);
    ~StartDialog();

    class MainWindow *w;    

private slots:
    void on_newButton_clicked();
    void on_loadButton_clicked();

private:
    Ui::StartDialog *ui;
};

#endif // STARTDIALOG_H
