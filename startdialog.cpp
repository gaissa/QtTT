#include "mainwindow.h"
#include "ui_startdialog.h"


StartDialog::StartDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StartDialog)
{
    ui->setupUi(this);

    w = new MainWindow;
    w->setAttribute(Qt::WA_DeleteOnClose);
    QObject::connect(ui->newButton, SIGNAL(clicked()), w, SLOT(nproject()));
    QObject::connect(ui->loadButton, SIGNAL(clicked()), w, SLOT(lproject()));
}

StartDialog::~StartDialog()
{
    //qDebug() << "startDialog DELETED";
    delete ui;
}

void StartDialog::on_newButton_clicked()
{
    w->showMaximized();
    this->close();
}

void StartDialog::on_loadButton_clicked()
{    
    w->showMaximized();
    this->close();
}

void StartDialog::on_aboutButton_clicked()
{
    AboutDialog *a = new AboutDialog();

    a->setAttribute(Qt::WA_DeleteOnClose);
    Qt::WindowFlags flags;
    flags = (Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
    a->setWindowFlags(flags);

    a->exec();
}
