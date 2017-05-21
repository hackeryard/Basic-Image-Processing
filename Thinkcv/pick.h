#ifndef PICK_H
#define PICK_H
#include <QApplication>
#include <QLabel>
#include<opencv2/opencv.hpp>

IplImage* org = 0;                              // 原图
IplImage* img = 0;                              // 打上马赛克操作图
IplImage* resize_img = 0;

void on_mouse( int event, int x, int y, int flags, void* ustc)
{
    static CvPoint pre_pt = {-1,-1};            // 矩形区域的第一个点
    static CvPoint cur_pt = {-1,-1};            // 第二个点
    if( event == CV_EVENT_LBUTTONDOWN )
    {
           pre_pt = cvPoint(x,y);
       }
    else if( event == CV_EVENT_LBUTTONUP )//处理四种划取方式
    {
        cur_pt = cvPoint(x,y);
        int width=abs(pre_pt.x-cur_pt.x);
        int height=abs(pre_pt.y-cur_pt.y);
        CvRect rect;
        if(pre_pt.x<cur_pt.x && pre_pt.y<cur_pt.y)
        {
            rect=cvRect(pre_pt.x,pre_pt.y,width,height);
        }
        else if(pre_pt.x>cur_pt.x && pre_pt.y<cur_pt.y)
        {
            rect=cvRect(cur_pt.x,pre_pt.y,width,height);
        }
        else if(pre_pt.x>cur_pt.x && pre_pt.y>cur_pt.y)
        {
            rect=cvRect(cur_pt.x,cur_pt.y,width,height);
        }
        else if(pre_pt.x<cur_pt.x && pre_pt.y>cur_pt.y)
        {
            rect=cvRect(pre_pt.x,cur_pt.y,width,height);
        }
        cvSetImageROI(img,rect);//可以设置一个滑竿进行UI调节
            int W=8;
            int H=8;

            /*------------------------------------------------------------------------------------------------------------
             *  打上马赛克
             *------------------------------------------------------------------------------------------------------------*/
            for(int mi=W;mi<img->roi->width;mi+=W)
                for(int mj=H;mj<img->roi->height;mj+=H)
                {
                    CvScalar tmp=cvGet2D(img,mj-H/2,mi-W/2);//取每个小方框的中心像素点进行赋值
                    for(int mx=mi-W;mx<=mi;mx++)
                        for(int my=mj-H;my<=mj;my++)
                            cvSet2D(img,my,mx,tmp);
                }
        cvResetImageROI(img);

    }
}

#endif // PICK_H
