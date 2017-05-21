#include "mainwindow.h"
#include "ui_mainwindow.h"
//UI class defined
#include"pick.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    QObject::connect(ui->horizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(value(int)));

}

MainWindow::~MainWindow()
{
    delete ui;
}


QImage MainWindow::convertOpenCVMatToQtQImage(cv::Mat mat) {
    if(mat.channels() == 1) // if grayscale image
    {
        return QImage((uchar*)mat.data, mat.cols, mat.rows, mat.step, QImage::Format_Indexed8);     // declare and return a QImage
    }
    else if(mat.channels() == 3) // if 3 channel color image
    {
        cv::Mat temp; // make the same cv::Mat
        cvtColor(mat, temp,CV_BGR2RGB); // cvtColor Makes a copt, that what i need
        QImage dest((const uchar *) temp.data, temp.cols, temp.rows, temp.step, QImage::Format_RGB888);
        dest.bits(); // enforce deep copy, see documentation
        // of QImage::QImage ( const uchar * data, int width, int height, Format format )
        return dest;
    }
    else
    {
       // qDebug() << "in convertOpenCVMatToQtQImage, image was not 1 channel or 3 channel, should never get here";
    }
    return QImage();        // return a blank QImage if the above did not work
}


cv::Mat QImage2cvMat(QImage image)
{
    cv::Mat mat;
    //qDebug() << image.format();
    switch(image.format())
    {
    case QImage::Format_ARGB32:
    case QImage::Format_RGB32:
    case QImage::Format_ARGB32_Premultiplied:
        mat = cv::Mat(image.height(), image.width(), CV_8UC4, (void*)image.constBits(), image.bytesPerLine());
        break;
    case QImage::Format_RGB888:
        mat = cv::Mat(image.height(), image.width(), CV_8UC3, (void*)image.constBits(), image.bytesPerLine());
        cv::cvtColor(mat, mat, CV_BGR2RGB);
        break;
    case QImage::Format_Indexed8:
        mat = cv::Mat(image.height(), image.width(), CV_8UC1, (void*)image.constBits(), image.bytesPerLine());
        break;
    }
    return mat;
}

//打开
void MainWindow::on_actionOpen_triggered()
{
    // Verify and Load the image
       FileName = QFileDialog::getOpenFileName();
       if(FileName == "")
       {
           ui->LE_img_path->setText("file not chosen");
           return;
       }

       this->img_ori = cv::imread(FileName.toStdString()); // load image by openCV

       if(this->img_ori.empty())
       {
           ui->LE_img_path->setText("error: image not read from file");
           return;
       }
       else
           ui->LE_img_path->setText(FileName);  // update label with file name


       this->img_ori.copyTo(this->img_process);
       QImage image = convertOpenCVMatToQtQImage(this->img_ori);

      // image=image.scaled(ui->QL_imgview->width(),ui->QL_imgview->height());
       ui->QL_imgview->setPixmap(QPixmap::fromImage(image));

      // QImage image1 = convertOpenCVMatToQtQImage(this->img_ori);//debug

       //初始化滑动条
       ui->horizontalSlider->setValue(10);//debug
}

//保存
void MainWindow::on_actionSave_triggered()
{
    if(FileName == "")
    {
        ui->LE_img_path->setText("file not chosen when try to save");
        return;
    }
    cv::imwrite(FileName.toStdString(),this->img_process);
}

//重置
void MainWindow::on_actionReset_triggered()
{
    QImage qimg_ori = convertOpenCVMatToQtQImage(this->img_ori);  // convert openCV images to QImage
    ui->QL_imgview->clear();
    ui->QL_imgview->setPixmap(QPixmap::fromImage(qimg_ori));   // show original images on labels
    this->img_ori.copyTo(this->img_process);
    //ui->horizontalSlider->setValue(10);
}

//另存为
void MainWindow::on_actionSave_as_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),"C:\\",tr("Images (*.png *.bmp *.jpg)"));
    if(!fileName.isEmpty()){
    cv::imwrite(fileName.toStdString(),this->img_process);
    }
}

//水平百叶窗
void MainWindow::on_action_triggered()
{
    //判断图像是否有效
    if(this->img_ori.empty())
    {
        ui->LE_img_path->setText("error: image not read from file");
        return;
    }

    cv::Mat  Image_in;    //暂存img_process
    this->img_process.copyTo(Image_in);

    cv::Mat Image_out(Image_in.size(), CV_8UC3);//复制image_in
    Image_in.convertTo(Image_out, CV_8UC3); 
    cv::Mat temp;

    int H_shade=8;      //百叶窗的窗口宽度
    int Inter=15;       //两个窗口之间的间隔

    int Num;
    Num=Image_in.rows/(H_shade+Inter);

    cv::Mat Mask(Image_in.size(), CV_8UC3);
    Mask.setTo(cv::Scalar(1,1,1));          //设置mask，初始为全白

    int row_begin=0;

    for (int i=0; i<=Num; i++)
    {
        if(i<Num)
        {
            row_begin=i*(H_shade+Inter);
            temp=Mask.rowRange(row_begin, row_begin+H_shade);
            temp.setTo(cv::Scalar(0,0,0));      //每个窗口宽度部分设置成全黑
        }
        else//边缘处理
        {
            row_begin=i*(H_shade+Inter);
            temp=Mask.rowRange(row_begin, Image_in.rows-1);//防止出界
            temp.setTo(cv::Scalar(0,0,0));
        }
    }

    cv::multiply(Mask,Image_out,Image_out);     //将mask与image_out合成百叶窗效果

    //保存到img_process，供后续处理
    Image_out.copyTo(this->img_process);
    QImage ximg_ori = convertOpenCVMatToQtQImage(Image_out);//转换格式
    ui->QL_imgview->setPixmap(QPixmap::fromImage(ximg_ori));//显示处理后的图像
}

//马赛克
void MainWindow::on_action_3_triggered()
{
    ui->LE_img_path->setText("mosaic");
    IplImage hjk=IplImage(this->img_process);
    org=&hjk;


    if(org->width > 1000)                       // 太大缩放
        {
            CvSize dst_cvsize;
            dst_cvsize.width=org->width*0.2;
            dst_cvsize.height=org->height*0.2;
            img = cvCreateImage( dst_cvsize, org->depth, org->nChannels);
            resize_img=cvCloneImage(org);
            cvResize(resize_img, img, CV_INTER_LINEAR);
        }
        else{
            img=cvCloneImage(org);
            }

    cvNamedWindow("Mosaic",CV_WINDOW_AUTOSIZE);

    //跟踪”mosaic“窗口，执行on_mouse函数
    cvSetMouseCallback( "Mosaic", on_mouse, 0 );

    while(1){
        cvShowImage("Mosaic",img);
        cv::Mat Image_out=img;

        //保存处理后到img_process
        Image_out.copyTo(this->img_process);
        QImage ximg_ori = convertOpenCVMatToQtQImage(Image_out);
        ui->QL_imgview->setPixmap(QPixmap::fromImage(ximg_ori));
        char c=(char)cv::waitKey(30);

        //如果输入ESC按键，马赛克子程序退出
        if(c==27){
            ui->LE_img_path->setText(FileName);     //恢复显示路径
            break;
        }

    }
    cvDestroyAllWindows();
    cvReleaseImage(&img);
}

//竖直百叶窗
void MainWindow::on_action_2_triggered()
{
    if(this->img_ori.empty())
    {
        ui->LE_img_path->setText("error: image not read from file");
        return;
    }

    cv::Mat  Image_in;
    this->img_process.copyTo(Image_in);

    cv::Mat Image_out(Image_in.size(), CV_8UC3);
    Image_in.convertTo(Image_out, CV_8UC3);
   // Show_Image(Image_out, Img_name);//对于浮点图像如果不在0-1范围内，则不能正常打开

    cv::Mat temp;

    int H_shade=8;
    int Inter=15;

    int Num;
    Num=Image_in.cols/(H_shade+Inter);
    //cout<<Num;

    cv::Mat Mask(Image_in.size(), CV_8UC3);
    Mask.setTo(cv::Scalar(1,1,1));//global fill out;

    int col_begin=0;

    for (int i=0; i<=Num; i++)
    {
        //rowRange--row_begin,row_end
        if(i<Num)
        {
            col_begin=i*(H_shade+Inter);
            temp=Mask.colRange(col_begin, col_begin+H_shade);//mat copy
            temp.setTo(cv::Scalar(0,0,0));
        }
        else//边缘处理
        {
            col_begin=i*(H_shade+Inter);
            temp=Mask.colRange(col_begin, Image_in.cols-1);//防止出界
            temp.setTo(cv::Scalar(0,0,0));
        }
    }

    cv::multiply(Mask,Image_out,Image_out);

    //复制处理后图像到process
    Image_out.copyTo(this->img_process);
    QImage ximg_ori = convertOpenCVMatToQtQImage(Image_out);
    ui->QL_imgview->setPixmap(QPixmap::fromImage(ximg_ori));

}

//旋转
void MainWindow::on_pushButton_clicked()
{
    count+=90;
    count=count%360;//对连续旋转操作计数
    QImage* imgRotate = new QImage;
    QMatrix matrix;
    matrix.rotate(count);//传入旋转角度
    QImage image = convertOpenCVMatToQtQImage(this->img_process);
    *imgRotate = image.transformed(matrix);//向transformed函数传入QMatrix对象
    //显示图像
    ui->QL_imgview->setPixmap(QPixmap::fromImage(*imgRotate));
    //cv::imshow("up",this->img_process);
   // ui->horizontalSlider->setValue(10);
}

//缩放
void MainWindow::value(int a){


    QImage* imgScaled = new QImage;

    //QImage image;
    //image.load(FileName);
    //暂存处理图
    QImage image = convertOpenCVMatToQtQImage(this->img_process);
    //因为滑竿初始值为10，滑竿范围为1-40，可最大放大4倍或者缩小十倍
    int width=image.width()*a/10;//设置比例
    int height=image.height()*a/10;
    *imgScaled=image.scaled(width,height,Qt::KeepAspectRatio);//保持长宽比
    ui->QL_imgview->setPixmap(QPixmap::fromImage(*imgScaled));
    //Qt::SmoothTransformation
    // this->img_process=QImage2cvMat(*imgScaled);
    //cv::imshow("you",this->img_process);//debug
    //delete imgScaled;

}

