#include "mainwindow.h"
#include <QApplication>
#include <QImageReader>
#include <QPrinter>
#include <QPrintDialog>
#include <QTextCodec>
#include <vector>
#include <opencv/cv.h>

int main(int argc, char *argv[])
{
    // kesilikle dosya acmayi kolaylastımak lazım
    // parse edilen bilgileri veri tabanına eklemek icin buton
    // foreign keyler tutarlılık icin onemli kesinlikle ekle
    // exceptionları dusun not al
    // diger class ve wrapperleri haricten gelistir buraya ekle

    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf-8"));

    QApplication a(argc, argv);
    MainWindow w;

    w.show();

    return a.exec();
}
