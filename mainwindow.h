#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "./qcustomplot/qcustomplot.h"
#include "filemanager.h"
#include "startdialog.h"
#include "categorydialog.h"


namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
  
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_spinBox_valueChanged(int arg1);
    void graphClicked(QCPAbstractPlottable *plottable);
    void on_pushButton_clicked();
    void updater();
    void yourSlot();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();    

    void on_comboBox_activated(const QString &arg1);   

    void on_spinBox_2_valueChanged(int arg1);

    void selectionChanged();

    void on_actionQUIT_triggered();

    void on_actionEXPORT_AS_PDF_triggered();

    void on_actionEXPORT_AS_PNG_triggered();

    void on_actionEDIT_CATEGORY_COLORS_triggered();

    void on_actionEDIT_CATEGORIES_triggered();

    void on_actionABOUT_QtTT_triggered();

public slots:
    void nproject();
    void lproject();

private:
    QString returnMonth();
    Ui::MainWindow *ui;
    QDate date;
    QDate today;
    QTimer *timer;
    QPen pen;
    QList <QCPBars*>list;
    QCPBars *bar;
    QVector<double> ticks;
    QVector<double> ticksY;
    QVector<QString> labels;
    void setupPlot(int year, int month, int day, bool firstLaunch, QString filename);
    //void mouseDoubleClickEvent(QMouseEvent *e);
    void readData(QString filename);
    int tCounter;
    int sCounter;
    int mCounter;
    int hCounter;
    FileManager *f;
    int randInt(int low, int high);
    int tempDay;
    int tempMonth;
    int tempYear;
    QString loadedFile;
    QString activeCategory;
    QStringList colorHolder;
};

#endif // MAINWINDOW_H
