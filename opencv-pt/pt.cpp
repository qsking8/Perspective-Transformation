#include <iostream>
#include<opencv2/opencv.hpp>
using namespace std;
using namespace cv;

Mat srcImg, dstImg;//原图像、目标图像
Point2f srcPoint[4], dstPoint[4];//原图像和目标图像的4个映射点
Point2f beforePlace;//记录移动映射点之前的位置
int radius;//映射点的判定半径
int pickPoint;//记录点击了哪个点

void mouseHander(int event, int x, int y, int flags, void* p)
{
    if (event == EVENT_LBUTTONDOWN)//左键按下
    {
        for (int i = 0; i < 4; i++)
        {
            //判断是否选择了某个映射点
            if (abs(x - dstPoint[i].x) <= radius && abs(y - dstPoint[i].y) <= radius)
            {
                pickPoint = i;
                beforePlace = dstPoint[pickPoint];//记录原本的位置
                break;
            }
        }
    }
    else if (event == EVENT_MOUSEMOVE && pickPoint >= 0)//左键按下后选取了某个点且拖拽
    {
        //更改映射后坐标
        dstPoint[pickPoint].x = x, dstPoint[pickPoint].y = y;
        //在临时图像上实时显示鼠标拖动时形成的图像
        //注意不能直接在dstImg上画，否则会画很多次
        Mat tmp = dstImg.clone();
        //原本的圆
        circle(tmp, beforePlace, radius, Scalar(228, 164, 140), -1);
        //绘制直线
        line(tmp, dstPoint[0], dstPoint[1], Scalar(246, 230, 171), 5, 8);
        line(tmp, dstPoint[1], dstPoint[2], Scalar(246, 230, 171), 5, 8);
        line(tmp, dstPoint[2], dstPoint[3], Scalar(246, 230, 171), 5, 8);
        line(tmp, dstPoint[3], dstPoint[0], Scalar(246, 230, 171), 5, 8);
        //重新绘制4个圆
        for (int i = 0; i < 4; i++)
        {
            if (i != pickPoint)
                circle(tmp, dstPoint[i], radius, Scalar(228, 164, 140), -1);
            else
                circle(tmp, dstPoint[i], radius, Scalar(96, 96, 240), -1);
        }
        imshow("透视变换后", tmp);
    }
    else if (event == EVENT_LBUTTONUP && pickPoint >= 0)//左键松开
    {
        //执行透视变换
        Mat Trans = getPerspectiveTransform(srcPoint, dstPoint);//由4组映射点得到变换矩阵
        warpPerspective(srcImg, dstImg, Trans, Size(dstImg.cols, dstImg.rows));//执行透视变换
        for (int i = 0; i < 4; i++)//显示4组映射点的位置
        {
            //画一个圆心在映射点(转换后)，半径为10，线条粗细为3，黄色的圆
            circle(dstImg, dstPoint[i], radius, Scalar(0, 215, 255), 3);
        }
        imshow("透视变换后", dstImg);
        pickPoint = -1;//重置选取状态
    }
}

void main()
{
    srcImg = imread("yourpic.jpg");
    if (!srcImg.empty()) // 检查是否成功加载图片
    {
        // 获取图片宽度和高度
        int imgWidth = srcImg.cols;
        int imgHeight = srcImg.rows;

        // 判断图片大小是否超出屏幕尺寸
        if (imgWidth > 800 || imgHeight > 600)
        {
            // 计算缩放比例
            double scale = min(800 / static_cast<double>(imgWidth), 600 / static_cast<double>(imgHeight));

            // 缩放图片
            resize(srcImg, srcImg, Size(), scale, scale);
        }
    }
    else
    {
        cout << "无法加载图片！" << endl;
    }

    radius = 10;//设置四个点的圆的半径
    pickPoint = -1;
    //映射前的4个点
    srcPoint[0] = Point2f(0, 0);
    srcPoint[1] = Point2f(srcImg.cols, 0);
    srcPoint[2] = Point2f(srcImg.cols, srcImg.rows);
    srcPoint[3] = Point2f(0, srcImg.rows);
    //创建一张略大于原图像的画布
    dstImg = Mat::zeros(Size(2 * radius + 100 + srcImg.cols, 2 * radius + 100 + srcImg.rows), srcImg.type());
    //初始映射后的4个点
    dstPoint[0] = Point2f(radius + 50, radius + 50);
    dstPoint[1] = Point2f(radius + 50 + srcImg.cols, radius + 50);
    dstPoint[2] = Point2f(radius + 50 + srcImg.cols, radius + 50 + srcImg.rows);
    dstPoint[3] = Point2f(radius + 50, radius + 50 + srcImg.rows);
    Mat Trans = getPerspectiveTransform(srcPoint, dstPoint);//由4组映射点得到变换矩阵
    Mat dst_perspective;//存储目标透视图像
    warpPerspective(srcImg, dstImg, Trans, Size(dstImg.cols, dstImg.rows));//执行透视变换

    for (int i = 0; i < 4; i++)//显示初始4组映射点的位置
    {
        //画一个圆心在映射点(转换后)，半径为10，线条粗细为3，黄色的圆
        circle(dstImg, dstPoint[i], radius, Scalar(95, 180, 243), 3);
    }
    imshow("透视变换后", dstImg);
    //鼠标事件
    setMouseCallback("透视变换后", mouseHander);
    waitKey();
}