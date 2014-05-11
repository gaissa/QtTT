#include "startdialog.h"
#include <QApplication>
#include <QTextCodec>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);  

  StartDialog *sd = new StartDialog();

  sd->setModal(true);
  sd->setAttribute(Qt::WA_DeleteOnClose);

  Qt::WindowFlags flags;
  flags = (Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
  sd->setWindowFlags(flags);

  sd->show();

  return a.exec();
}
