#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QMainWindow>
#include <QFile>
#include <QDebug>
#include <QDate>

class FileManager : public QMainWindow
{
    Q_OBJECT

public:   
    FileManager(QWidget *parent = 0);
    FileManager(int year, int month, QString filename, bool totals);
    ~FileManager();

    QStringList pTotal;
    double projectTotal;

    void deleteCategory(QStringList categories);
    bool totalCheck;
    int today;
    void clearData();
    int categoryCount();
    double getCategoryTotal(bool check, QString activeCategory, int year, int month);
    QString categories(int i);   
    QVector<double> data(int i);
    void updateData(int year, int month, int day, QString category, double amount);
    void writeData(QStringList q, bool checkStatus);
    QStringList readFile();
    void readMonthlyTotals(QString data);   
    double getTotal();
    void updateSettings(QString category);

private:    
    QList<QList<QString> > project;
    QList<QString> q;
    QString m;
    QString openFile;
    QStringList oldTemporaryData;
    double totalAmount;
    int days;
    void appendData(int year, int month);
    void populateMonth(QString year, QString month);

signals:

public slots:

};

#endif // FILEMANAGER_H
