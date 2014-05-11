#ifndef CATEGORYDIALOG_H
#define CATEGORYDIALOG_H

#include <QDialog>

namespace Ui {
class CategoryDialog;
}

class CategoryDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CategoryDialog(QWidget *parent = 0);
    ~CategoryDialog();

    QPushButton* button;

private:
    Ui::CategoryDialog *ui;
    QString newCategory;
    class FileManager *f;

public slots:    

private slots:
    void on_addButton_clicked();
    void on_deleteButton_clicked();
};

#endif // CATEGORYDIALOG_H
