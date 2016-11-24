#include "mainwindow.h"
#include <QApplication>
#include <QDesktopWidget>
#include <login.h>
int main(int argc, char *argv[])
{


    QApplication a(argc, argv);

    Login l;

    l.move ((QApplication::desktop()->width() - l.width())/2,(QApplication::desktop()->height() - l.height())/2);


    if(l.exec()==QDialog::Accepted){

    l.hide();

    MainWindow w;
     w.move ((QApplication::desktop()->width() - w.width())/2,(QApplication::desktop()->height() - w.height())/2);

     w.btnClick();

    w.show();
    return a.exec();
    }else{
        return 0;
    }

}
