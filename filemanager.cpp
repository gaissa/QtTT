#include "filemanager.h"

FileManager::FileManager(QWidget *parent) :
             QMainWindow(parent)
{
}

// The constructor.
FileManager::FileManager(int year, int month, QString filename, bool totals)
{    
    totalCheck = totals;    
    openFile = filename;
    appendData(year, month);
}

// The destructor. Does nothing much.
FileManager::~FileManager()
{
    //qDebug() << "FileManager DELETED";
}

// Read the file.
QStringList FileManager::readFile()
{
    QFile file(openFile);

    if(!file.open(QIODevice::ReadOnly))
    {
        //qDebug() << "Error opening file: " << file.error();
    }

    QTextStream instream(&file);
    //instream.setCodec("UTF-8");

    QStringList strings;   

    while (!instream.atEnd())
    {
        strings += instream.readLine();
    }   

    QString temp;

    for (int i = 0; i < strings.count(); i++)
    {
        temp = temp + strings[i];

        if (i < strings.count()-1)
        {
            temp = temp + "\n";
        }
    }   

    if (totalCheck == true)
    {
        today = QDate::currentDate().day();
        readMonthlyTotals(temp);

    }
    file.close();    

    return strings;
}

// Populate the months...
void FileManager::populateMonth(QString year, QString month)
{    
    QStringList result = readFile().filter(year);

    for (int i = 0; i < result.length(); i++)
    {
        QString temp = result[i].split(',')[0];
        QString temp2 = temp.split('-')[1];

        if (temp2 == month)
        {
            QList<QString> q2;
            q2.append(result[i].split(',')[1]);
            days = result[i].split(',').length()-1;

            for (int j = 2; j <= days; j++)
            {
                q2.append(result[i].split(',')[j]);
            }
            project.append(q2);
        }
    }
}

// Append some data.
void FileManager::appendData(int year, int month)
{
     if (month < 10)
     {
         m = "0" + QString::number(month);
     }
     else
     {
         m = QString::number(month);
     }
     populateMonth(QString::number(year), m);
}

// return the categories.
QString FileManager::categories(int i)
{
    return project[i][0];
}

// Count the active categories.
int FileManager::categoryCount()
{
    return project.count();
}

// Append and return the data.
QVector<double> FileManager::data(int i)
{
    QVector<double> d;

    for (int day = 1; day < days; day++)
    {
        d.append(project[i][day].toDouble());
    }

    return d;
}

// Start the data update.
void FileManager::updateData(int year, int month, int day, QString category, double amount)
{
    QString temp;

    today = day;

    if (month < 10)
    {
        temp = QString::number(year) + "-0" + QString::number(month);
    }
    else
    {
        temp = QString::number(year) + "-" + QString::number(month);
    }

    QStringList q = readFile().filter(temp).filter(category);

    if (!q.isEmpty())
    {        
        q = q[0].split(',');

        oldTemporaryData = q;        

        double t = q[day+1].toDouble() + amount;

        q[day+1] = QString::number(t);

        writeData(q, true);
    }
    else
    {
        QStringList q;
        QString temp;

        temp = "\n";

        if (month < 10)
        {
            temp += QString::number(year) + "-0" + QString::number(month) + "," + category;
        }
        else
        {
            temp += QString::number(year) + "-" + QString::number(month) + "," + category;
        }       

        for (int i = 1; i <= 31; i++)
        {
            if (i == day )
            {
                temp += "," + QString::number(amount);
            }
            else
            {
                temp += ",0";
            }
        }
        q.append(temp);
        writeData(q, false);
    }
}

// Write the new data to a file.
void FileManager::writeData(QStringList q, bool checkStatus)
{   
    QFile file(openFile);

    if (checkStatus == true)
    {
        projectTotal = 0.0;

        for (int i = 2; i <= oldTemporaryData.count()-1; i++)
        {
           projectTotal = projectTotal + oldTemporaryData[i].toDouble();
        }

        //qDebug() << "PROJECT TOTAL" << projectTotal;

        QString oldData = oldTemporaryData.join(",");
        QString newData = q.join(",");

        pTotal = q;

        QTextStream in(&file);

        if (!file.open(QIODevice::ReadWrite | QIODevice::Text))
        {
            return;
        }

        QString text = in.readAll();

        readMonthlyTotals(text);

        text = text.replace(QString(oldData), QString(newData)); // replace text in string
        //qDebug() << "NEW" << text << "END";

        file.close();

        if (!file.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Truncate))
        {
           return;
        }

        QTextStream out(&file);
        out << text;
        file.close();
    }
    else
    {
        QString text = q[0];

        if (!file.open(QIODevice::Append | QIODevice::Text))
        {
           return;
        }

        QTextStream out(&file);
        out << text;
        file.close();
    }
}

double FileManager::getCategoryTotal(bool check, QString activeCategory, int year, int month)
{
    //qDebug() << month;

    if (check)
    {
       QString temp;       

       if (month < 10)
       {
           temp = QString::number(year) + "-0" + QString::number(month);
       }
       else
       {
           temp = QString::number(year) + "-" + QString::number(month);
       }

       QStringList q = readFile().filter(temp).filter(activeCategory)[0].split(",");

       double temp2 = 0.0;
       for (int i = 2; i <= q.count()-1; i++)
       {
          temp2 = temp2 + q[i].toDouble();
       }

       return temp2;
    }    

    double temp2 = 0.0;
    return temp2;
}

// Read the monthy total project times.
void FileManager::readMonthlyTotals(QString data)
{
    double total = 0.0;
    QVector<QStringList> *list = new QVector<QStringList>;
    QStringList temp = data.split("\n");

    for (int i = 0; i < temp.count(); i++)
    {
         QStringList qq = temp[i].split(",");
         qq.removeFirst();
         qq.removeFirst();
         list->append(qq);
    }
    for (int i = 0; i < list->count(); i++)
    {
        total = total + list->at(0)[today-1].toDouble();
    }
    totalAmount = total;
}

// Return the total amount for the day.
double FileManager::getTotal()
{
    return totalAmount;
}

// Add new category.
void FileManager::updateSettings(QString category)
{

    QString c = category + '\n';
    //qDebug() << category.toUtf8().constData();

    QFile file("./settings/settings.ttt");
    file.open(QFile::Append);
    file.write(c.toUtf8().constData());   
    file.close();
}

// Delete a category.
void FileManager::deleteCategory(QStringList categories)
{
    QFile file("./settings/settings.ttt");
    file.open(QFile::WriteOnly);

    //qDebug() << categories;

    file.close();

    file.open(QFile::Append);

    for (int i = 0; i < categories.count(); i++)
    {
        QString c = categories[i] + '\n';
        file.write(c.toUtf8().constData());
    }

    file.close();
}

// Clear the data.
void FileManager::clearData()
{
    project.clear();
}
