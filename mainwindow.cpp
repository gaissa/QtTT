#include "mainwindow.h"
#include "ui_mainwindow.h"


// The constructor.
MainWindow::MainWindow(QWidget *parent) :
            QMainWindow(parent),
            ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->centralWidget->hide();

    tCounter = 0;
    sCounter = 0;
    mCounter = 0;
    hCounter = 0;

    ui->pushButton_2->setEnabled(false);
    ui->pushButton_3->setEnabled(false);

    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());
    today = QDate::currentDate();

    tempMonth = today.month();
    tempYear = today.year();

    ui->plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes |
                                    QCP::iSelectLegend | QCP::iSelectPlottables);

    ui->plot->plotLayout()->addElement(1, 0, new QCPPlotTitle(ui->plot, returnMonth()));

    connect(ui->actionSAVE_AS, SIGNAL(triggered()), this, SLOT(nproject()));
    connect(ui->actionLOAD_3, SIGNAL(triggered()), this, SLOT(lproject()));

    connect(ui->plot, SIGNAL(selectionChangedByUser()), this, SLOT(selectionChanged()));

    //connect(ui->plot,
    //        SIGNAL(plottableClick(QCPAbstractPlottable*, QMouseEvent*)),
    //        this,
    //        SLOT(graphClicked(QCPAbstractPlottable*)));

    //connect(ui->plot,
    //        SIGNAL(itemClick(QCPAbstractItem*, QMouseEvent*)),
    //        this,
    //        SLOT(itemClicked(QCPAbstractItem*)));
}

// The destructor.
MainWindow::~MainWindow()
{
    delete f;
    delete ui;
}

QString MainWindow::returnMonth()
{
    QDate date;
    date.setDate(tempYear, tempMonth, 1);

    QLocale locale  = QLocale(QLocale::C, QLocale::UnitedStates); // set the locale you want here
    QString englishDate = locale.toString(date, "MMMM yyyy");

    return englishDate;
}

// Open the FileManager.
void MainWindow::readData(QString filename)
{
    f = new FileManager(tempYear, tempMonth, filename, true);
}

// Random number between low and high.
int MainWindow::randInt(int low, int high)
{
    return qrand() % ((high + 1) - low) + low;
}

// Set up the plot.
void MainWindow::setupPlot(int year, int month, int day, bool firstLaunch, QString filename)
{
    readData(filename);

    list.clear();
    ui->plot->clearPlottables();

    date.setDate(year, month, day);

    ui->plot->setBackground(QColor(225, 225, 225));
    ui->plot->axisRect()->setBackground(Qt::white);

    pen.setWidthF(0.1);

    // Create empty bar chart objects:
    for (int i = 0; i < f->categoryCount(); i++)
    {
        bar = new QCPBars(ui->plot->xAxis, ui->plot->yAxis);
        list.append(bar);
        ui->plot->addPlottable(list[i]);
        list[i]->moveAbove(list[0]);
    }

    if (list.count() > 1)
    {
        list[0]->moveAbove(list[1]);
    }

    // Check for the first launch.
    if (firstLaunch)
    {
        // Setup legend:
        ui->plot->legend->setVisible(true);

        FileManager *settingsFile = new FileManager(1, 0, "./settings/settings.ttt", false);
        QStringList temp = settingsFile->readFile();

        for (int i = 0; i < temp.count(); i++)
        {
            ui->comboBox->addItem(temp[i].split(",")[0].toLocal8Bit());
        }
        ui->spinBox->setValue(today.month());
        ui->spinBox_2->setValue(today.year());

        ticks.clear();
        ticksY.clear();
    }

    for (int i = 0; i < f->categoryCount(); i++)
    {
        list[i]->setName(f->categories(i));

        pen.setColor(QColor(0, 0, 0));

        list[i]->setPen(pen);
        list[i]->setSelectedPen(pen);

        list[i]->setBrush(QColor(i*35, i*35, i*35, 255));
        list[i]->setSelectedBrush(QColor(0, 100, 50, 200));

        ui->plot->legend->setSelectedBrush(QColor(0, 100, 50, 200));
        ui->plot->legend->setSelectedBorderPen(pen);

        pen.setColor(QColor(0, 100, 50, 200));
        pen.setWidth(2);
        ui->plot->legend->setSelectedIconBorderPen(pen);
        pen.setWidthF(0.1);
    }

    // Prepare x-axis with labels:
    for (int i = 1; i <= date.daysInMonth(); i++)
    {
        ticks.append(i);
    }
    // Prepare y-axis with labels:
    for (int i = 0; i <= 24; i++)
    {
        ticksY.append(i);
    }

    // Prepare x-axis
    ui->plot->xAxis->setAutoTicks(false);
    ui->plot->xAxis->setAutoTickLabels(true);
    ui->plot->xAxis->setTickVector(ticks);    
    ui->plot->xAxis->setSubTickCount(0);
    ui->plot->xAxis->setTickLength(0, 4);
    ui->plot->xAxis->grid()->setVisible(true);
    ui->plot->xAxis->setRange(0, date.daysInMonth()+4);

    // Prepare y-axis
    ui->plot->yAxis->setAutoTicks(false);
    ui->plot->yAxis->setAutoTickLabels(true);
    ui->plot->yAxis->setTickVector(ticksY);
    ui->plot->yAxis->setRange(0, 24);
    ui->plot->yAxis->setSubTickCount(6);
    ui->plot->yAxis->setPadding(5);
    ui->plot->yAxis->setLabel("Time spent in HOURS");
    ui->plot->yAxis->grid()->setSubGridVisible(true);
    QPen gridPen;
    gridPen.setStyle(Qt::SolidLine);
    gridPen.setColor(QColor(0, 0, 0, 25));
    ui->plot->yAxis->grid()->setPen(gridPen);
    gridPen.setStyle(Qt::DotLine);
    ui->plot->yAxis->grid()->setSubGridPen(gridPen);

    // Add data.
    for (int i = 0; i < f->categoryCount(); i++)
    {
        list[i]->setData(ticks, f->data(i));
    }
}

// The graph is clicked.
//void MainWindow::graphClicked(QCPAbstractPlottable *plottable)
//{
    //ui->statusBar->showMessage(QString("CHOSEN CATEGORY: '%1'.").arg(plottable->name()), 15000);
    //ui->plot->x
    //qDebug() << plottable;
//}

// The graph is clicked.
//void MainWindow::itemClicked(QCPAbstractItem *item)
//{
    //qDebug() << item;
//}

// Change the month.
void MainWindow::on_spinBox_valueChanged(int arg1)
{
      tempMonth = arg1;
      ticks.clear();
      ticksY.clear();
      setupPlot(tempYear, arg1, 1, false, loadedFile);

      ui->plot->plotLayout()->removeAt(1);
      ui->plot->plotLayout()->addElement(1, 0, new QCPPlotTitle(ui->plot, returnMonth()));

      ui->plot->replot();
}

// Change the year.
void MainWindow::on_spinBox_2_valueChanged(int arg1)
{
    tempYear = arg1;
    ticks.clear();
    ticksY.clear();
    setupPlot(arg1, tempMonth, 1, false, loadedFile);

    ui->plot->plotLayout()->removeAt(1);
    ui->plot->plotLayout()->addElement(1, 0, new QCPPlotTitle(ui->plot, returnMonth()));

    ui->plot->replot();
}

// FIX TO F11
//void MainWindow::mouseDoubleClickEvent(QMouseEvent *e)
//{
//    QWidget::mouseDoubleClickEvent(e);

//    if(isFullScreen())
//    {
//       this->setWindowState(Qt::WindowMaximized);
//    }
//    else
//    {
//       this->setWindowState(Qt::WindowFullScreen);
//    }
//}

// Update the progress bar.
void MainWindow::updater()
{
    QTime q;   

    if (QTime::currentTime().toString() == "23:59:59")
    {
        ui->pushButton_2->click();

        QTimer *timer = new QTimer;
        timer->setSingleShot(true);

        connect(timer, SIGNAL(timeout()), this, SLOT(yourSlot()));

        // !!
        timer->start(2000);               
    }
    else
    {
        tCounter++;
        sCounter++;

        if (sCounter == 59)
        {
            sCounter = 1;
            mCounter++;
            ui->label_5->setText(QString::number(mCounter));

            if (mCounter == 59)
            {
                mCounter = 1;
                hCounter++;
                ui->label_8->setText(QString::number(hCounter));
            }
        }

        if (sCounter < 59)
        {
            ui->label_3->setText("0" + QString::number(sCounter));
        }
        else
        {
            ui->label_3->setText(QString::number(sCounter));
        }
        ui->progressBar->setValue(tCounter);
    }
}

// !!!!!
void MainWindow::yourSlot()
{
    qDebug() << "terve";
    today = QDate::currentDate();

    qDebug() << today;

    ui->pushButton->click();
}

// Start the time tracking.
void MainWindow::on_pushButton_clicked()
{
    ui->stoppedOrTracking->setStyleSheet("color: green; background-color: rgb(245, 245, 245)");
    ui->stoppedOrTracking->setText("TRACKING");

    ui->menuFILE->setDisabled(true);

    ui->menuSETTINGS->setDisabled(true);

    today = QDate::currentDate();

    tempDay = today.day();

    //qDebug() << "TOTAL: " << f->getTotal();

    ui->pushButton->setText("TRACK");
    ui->pushButton->setEnabled(false);
    ui->pushButton_2->setEnabled(true);
    ui->pushButton_3->setEnabled(true);

    ui->comboBox->setEnabled(false);

    // Create a new timer.
    timer = new QTimer(this);

    // Connect timer to the slot.
    connect(timer, SIGNAL(timeout()), this, SLOT(updater()));

    // Run timer once in a second.
    timer->start(1000);
}

// Stop the timer button.
void MainWindow::on_pushButton_2_clicked()
{
    ui->stoppedOrTracking->setStyleSheet("color: darkRed; background-color: rgb(245, 245, 245)");
    ui->stoppedOrTracking->setText("STOPPED");

    ui->menuFILE->setEnabled(true);
    ui->menuSETTINGS->setEnabled(true);

    ui->spinBox->setValue(today.month());
    ui->spinBox_2->setValue(today.year());

    ui->progressBar->setValue(0);

    timer->stop();

    ui->pushButton->setEnabled(true);
    ui->pushButton_2->setEnabled(false);
    ui->pushButton_3->setEnabled(false);

    ui->comboBox->setEnabled(true);

    double amount = tCounter/3600.0;

    sCounter = 0;
    mCounter = 0;
    hCounter = 0;
    tCounter = 0;

    ui->label_3->setText("0" + QString::number(sCounter));
    ui->label_5->setText("0" + QString::number(mCounter));
    ui->label_8->setText("0" + QString::number(hCounter));

    f->updateData(today.year(), today.month(), today.day(), activeCategory, amount);    

    if (ui->spinBox->value() == today.month() && ui->spinBox_2->value() == today.year())
    {
        ticks.clear();
        ticksY.clear();

        setupPlot(today.year(), today.month(), today.day(), false, loadedFile);
        ui->plot->replot();
    }    
}

// Pause the timer function.
void MainWindow::on_pushButton_3_clicked()
{
    ui->stoppedOrTracking->setStyleSheet("color: darkCyan; background-color: rgb(245, 245, 245)");
    ui->stoppedOrTracking->setText("PAUSED");

    timer->stop();
    ui->pushButton->setText("CONTINUE");
    ui->pushButton->setEnabled(true);
    ui->pushButton_2->setEnabled(false);
    ui->pushButton_3->setEnabled(false);
}

// Choose the category on combo box.
void MainWindow::on_comboBox_activated(const QString &arg1)
{
    activeCategory = arg1;
}

// New Project at start dialog slot.
void MainWindow::nproject()
{
    ticks.clear();
    ticksY.clear();

    QString fileName = QFileDialog::getSaveFileName(this,
             tr("Create a new project"), "",
             tr("Project Files (*.ttt);;All Files (*)"));

    if (fileName.isEmpty())
    {
        return;
    }
    else
    {
        ui->centralWidget->show();
        ui->comboBox->clear();
        QFile file(fileName);

        if (!file.open(QIODevice::WriteOnly))
        {
            QMessageBox::information(this, tr("Unable to open file"), file.errorString());
            return;
        }

        QTextStream out(&file);

        out << "0000-00,Default,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0";

        file.close();
    }

    loadedFile = fileName;
    QStringList title = loadedFile.split("/");
    ui->projectNameLabel->setText(title[title.count()-1]);

    setupPlot(today.year(), today.month(), today.day(), true, loadedFile);
    ui->comboBox->setCurrentIndex(0);
    activeCategory = ui->comboBox->currentText();
    ui->plot->replot();
}

// Load Project at start dialog slot.
void MainWindow::lproject()
{
    ticks.clear();
    ticksY.clear();

    loadedFile = QFileDialog::getOpenFileName(this,
                     tr("Open project file"), "./", tr("Project files (*.ttt)"));

    if (loadedFile != NULL)
    {
        ui->centralWidget->show();
        ui->comboBox->clear();

        //ui->spinBox->setValue(today.month());
        //ui->spinBox_2->setValue(today.year());

        QStringList title = loadedFile.split("/");
        ui->projectNameLabel->setText(title[title.count()-1]);

        setupPlot(today.year(), today.month(), today.day(), true, loadedFile);
        ui->comboBox->setCurrentIndex(0);
        activeCategory = ui->comboBox->currentText();
        ui->plot->replot();
    }
}

// Synchronize selection of graphs with selection of corresponding legend items.
void MainWindow::selectionChanged()
{
    for (int i = 0; i < ui->plot->plottableCount(); ++i)
    {
        QCPPlottableLegendItem *item = ui->plot->legend->itemWithPlottable(ui->plot->plottable(i));

        if (item->selected() || ui->plot->plottable(i)->selected())
        {
            double val = f->getCategoryTotal(true, ui->plot->plottable(i)->name(), tempYear, tempMonth);            

            QString str;            

            if (val < 10.0)
            {
                str = "0" + QString::number(val, 'f', 2);
            }
            else
            {
                str = QString::number(val, 'f', 2);
            }

            ui->statusBar->showMessage(QString("Clicked on graph '%1'.").arg( ui->plot->plottable(i)->name()), 5000);

            ui->label_12->setText(str + " h");
            ui->plot->plottable(i)->setSelected(true);
            item->setSelected(true);
            break;
        }
        else if (!(item->selected()) && !(ui->plot->plottable(i)->selected()))
        {
            ui->label_12->setText("00.00 h");
            //break;
        }
    }
}

// Quit the app.
void MainWindow::on_actionQUIT_triggered()
{
     QMessageBox msgBox;
     msgBox.setText("Really quit want to quit?");

     QPushButton *connectButton = msgBox.addButton(tr("Quit"), QMessageBox::ActionRole);
     QPushButton *abortButton = msgBox.addButton(QMessageBox::Abort);

     msgBox.exec();

     if (msgBox.clickedButton() == connectButton)
     {
         qApp->exit();
     }
     else if (msgBox.clickedButton() == abortButton)
     {
         // abort
     }
}

// Save plot as PDF.
void MainWindow::on_actionEXPORT_AS_PDF_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this,
             tr("Export as PDF"), "",
             tr("PDF Files (*.pdf);;All Files (*)"));

    if (fileName.isEmpty())
    {
        return;
    }
    else
    {
        bool pdf = ui->plot->savePdf(fileName, true, 0, 0, "", "");

        if (pdf == true)
        {
            QMessageBox msgBox;
            msgBox.setText("PDF has been saved succesfully!");
            msgBox.exec();
        }
        else if (pdf == false)
        {
            QMessageBox msgBox;
            msgBox.setText("PDF was not saved succesfully!");
            msgBox.exec();
        }
    }
}

// Save plot as PNG.
void MainWindow::on_actionEXPORT_AS_PNG_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this,
             tr("Export as PNG"), "",
             tr("PNG Files (*.png);;All Files (*)"));

    if (fileName.isEmpty())
    {
        return;
    }
    else
    {
        bool png = ui->plot->savePng(fileName, 0, 0, 1.0, 100);

        if (png == true)
        {
            QMessageBox msgBox;
            msgBox.setText("PNG has been saved succesfully!");
            msgBox.exec();
        }
        else if (png == false)
        {
            QMessageBox msgBox;
            msgBox.setText("PNG was not saved succesfully!");
            msgBox.exec();
        }
    }
}

// Edit colors (not implemented).
void MainWindow::on_actionEDIT_CATEGORY_COLORS_triggered()
{
//    FileManager *settingsFile = new FileManager(1, 0, "./settings/c_settings.ttt", false);
//    QStringList temp = settingsFile->readFile();

//    QColor color = QColorDialog::getColor(Qt::black, this, "Text Color");

//    if(color.isValid())
//    {
//        QString red = QString::number(color.red());
//        QString green = QString::number(color.green());
//        QString blue = QString::number(color.blue());

//        QMessageBox::information(this, "Text Color", "You selected " + red);
//    }
}

// Edit categories.
void MainWindow::on_actionEDIT_CATEGORIES_triggered()
{
    CategoryDialog *cd = new CategoryDialog;
    cd->setAttribute(Qt::WA_DeleteOnClose);
    Qt::WindowFlags flags;
    flags = (Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
    cd->setWindowFlags(flags);
    cd->setModal(true);
    cd->exec();

    ui->comboBox->clear();

    FileManager *settingsFile = new FileManager(1, 0, "./settings/settings.ttt", false);
    QStringList temp = settingsFile->readFile();

    for (int i = 0; i < temp.count(); i++)
    {
        ui->comboBox->addItem(temp[i].split(",")[0].toLocal8Bit());
    }
    ui->comboBox->update();

    ui->comboBox->setCurrentIndex(ui->comboBox->count()-1);
    activeCategory = ui->comboBox->currentText();
}

// Show about dialog.
void MainWindow::on_actionABOUT_QtTT_triggered()
{
    AboutDialog *a = new AboutDialog();

    a->setAttribute(Qt::WA_DeleteOnClose);
    Qt::WindowFlags flags;
    flags = (Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
    a->setWindowFlags(flags);

    a->exec();
}
