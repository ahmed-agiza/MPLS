#include "mainwindow.h"
#include <QApplication>
#include <QMessageBox>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFile qssFile(":qdarkstyle/style.qss");

    if (!qssFile.exists())
    {
        qDebug() << "QSS Error";
        QMessageBox::critical(0, "Error", "Failed to load application theme");
        exit(1);
    }
    else
    {
        qssFile.open(QFile::ReadOnly | QFile::Text);
        QTextStream qssTS(&qssFile);
        qApp->setStyleSheet(qssTS.readAll());
    }

    MainWindow w;
    w.showMaximized();

    return a.exec();
}
