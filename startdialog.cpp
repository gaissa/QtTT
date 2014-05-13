#include "mainwindow.h"
#include "ui_startdialog.h"


// The constructor
StartDialog::StartDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StartDialog)
{
    ui->setupUi(this);

    w = new MainWindow;
    w->setAttribute(Qt::WA_DeleteOnClose);
    QObject::connect(ui->newButton, SIGNAL(clicked()), w, SLOT(nproject()));
    QObject::connect(ui->loadButton, SIGNAL(clicked()), w, SLOT(lproject()));

    // Create folder for settings if does not exists.
    QDir dir("./settings");

    if (!dir.exists())
    {
        dir.mkpath(".");
    }
}

// The destructor.
StartDialog::~StartDialog()
{
    delete ui;
}

// Create a new project.
void StartDialog::on_newButton_clicked()
{
    w->showMaximized();
    this->close();
}

// Load a new project.
void StartDialog::on_loadButton_clicked()
{    
    w->showMaximized();
    this->close();
}

// About dialog.
void StartDialog::on_aboutButton_clicked()
{
    AboutDialog *a = new AboutDialog();

    a->setAttribute(Qt::WA_DeleteOnClose);
    Qt::WindowFlags flags;
    flags = (Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
    a->setWindowFlags(flags);

    a->exec();
}
