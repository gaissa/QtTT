#include "categorydialog.h"
#include "ui_categorydialog.h"
#include "filemanager.h"
#include <QDebug>
#include <QMessageBox>
#include <QPushButton>
#include <QColorDialog>


CategoryDialog::CategoryDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CategoryDialog)
{
    ui->setupUi(this);

    f = new FileManager(1, 0, "./settings/settings.ttt", false);
    QStringList temp = f->readFile();

    for (int i = 0; i < temp.count(); i++)
    {
        ui->categoryList->addItem(temp[i].split(",")[0].toLocal8Bit());
        ui->categoryList->sortItems();
    }
}

CategoryDialog::~CategoryDialog()
{
    //qDebug() << "DELETED!";
    delete ui;
}

// Choose the colors.
void CategoryDialog::on_colorButton_clicked()
{
    QColor color = QColorDialog::getColor(Qt::black, this, "Text Color");

    if(color.isValid())
    {
        QString red = QString::number(color.red());
        QString green = QString::number(color.green());
        QString blue = QString::number(color.blue());

        QMessageBox::information(this, "Text Color", "You selected " + red);
    }
}

// Add new category.
void CategoryDialog::on_addButton_clicked()
{
    if (ui->addLineEdit->text() != "")
    {
        f->updateSettings(ui->addLineEdit->text());
        newCategory = ui->addLineEdit->text();
        ui->categoryList->addItem(newCategory);
        ui->categoryList->sortItems();
    }
}

// Delete a category.
void CategoryDialog::on_deleteButton_clicked()
{
    QStringList temp;
    qDeleteAll(ui->categoryList->selectedItems());

    for (int i = 0; i < ui->categoryList->count(); i++)
    {
        temp.append(ui->categoryList->item(i)->text());
    }

    f->deleteCategory(temp);
}
