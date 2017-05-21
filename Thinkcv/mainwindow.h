#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QFileDialog>
#include <opencv2/opencv.hpp>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void value(int);

private slots:

    void on_actionOpen_triggered();

    void on_actionReset_triggered();

    void on_actionSave_as_triggered();

    void on_action_triggered();//触发水平百叶窗

    void on_action_3_triggered();//马赛克

    void on_action_2_triggered();//触发竖直百叶窗

    void on_pushButton_clicked();//旋转

    void on_actionSave_triggered();

 //   void on_horizontalSlider_actionTriggered(int action);

private:
    Ui::MainWindow *ui;
    cv::Mat img_ori;//original image in this class
    cv::Mat img_process; // CV_8UC3
    QString FileName;
    int count=0;
    QImage convertOpenCVMatToQtQImage(cv::Mat mat);//declare
    //QImage image1;

};
#endif // MAINWINDOW_H
