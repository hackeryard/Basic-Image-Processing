#include "mainwindow.h"
#include <QApplication>
#include <QLabel>
#include <QTextCodec>
#include <QSplashScreen>
#include <QDesktopWidget>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSplashScreen *splash = new QSplashScreen;
    splash->setPixmap(QPixmap("C:/Users/hjkmin/Documents/11/Thinkcv/space1.jpg"));
    splash->show();
    //让对话框延迟一段时间显示
    for(int i=0;i<1000;i++)//2000
    {
        splash->repaint();
    }

    MainWindow w;
    w.setWindowTitle("Image Basic Processing!");


    //将窗口移动到屏幕的中央
    w.move ((QApplication::desktop()->width() - w.width())/2,(QApplication::desktop()->height() - w.height())/2);
    splash->finish(&w);
    delete splash;

    w.show();
    return a.exec();

}
