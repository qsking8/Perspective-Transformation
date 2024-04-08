#include <iostream>
#include<opencv2/opencv.hpp>
using namespace std;
using namespace cv;

Mat srcImg, dstImg;//ԭͼ��Ŀ��ͼ��
Point2f srcPoint[4], dstPoint[4];//ԭͼ���Ŀ��ͼ���4��ӳ���
Point2f beforePlace;//��¼�ƶ�ӳ���֮ǰ��λ��
int radius;//ӳ�����ж��뾶
int pickPoint;//��¼������ĸ���

void mouseHander(int event, int x, int y, int flags, void* p)
{
    if (event == EVENT_LBUTTONDOWN)//�������
    {
        for (int i = 0; i < 4; i++)
        {
            //�ж��Ƿ�ѡ����ĳ��ӳ���
            if (abs(x - dstPoint[i].x) <= radius && abs(y - dstPoint[i].y) <= radius)
            {
                pickPoint = i;
                beforePlace = dstPoint[pickPoint];//��¼ԭ����λ��
                break;
            }
        }
    }
    else if (event == EVENT_MOUSEMOVE && pickPoint >= 0)//������º�ѡȡ��ĳ��������ק
    {
        //����ӳ�������
        dstPoint[pickPoint].x = x, dstPoint[pickPoint].y = y;
        //����ʱͼ����ʵʱ��ʾ����϶�ʱ�γɵ�ͼ��
        //ע�ⲻ��ֱ����dstImg�ϻ�������ử�ܶ��
        Mat tmp = dstImg.clone();
        //ԭ����Բ
        circle(tmp, beforePlace, radius, Scalar(228, 164, 140), -1);
        //����ֱ��
        line(tmp, dstPoint[0], dstPoint[1], Scalar(246, 230, 171), 5, 8);
        line(tmp, dstPoint[1], dstPoint[2], Scalar(246, 230, 171), 5, 8);
        line(tmp, dstPoint[2], dstPoint[3], Scalar(246, 230, 171), 5, 8);
        line(tmp, dstPoint[3], dstPoint[0], Scalar(246, 230, 171), 5, 8);
        //���»���4��Բ
        for (int i = 0; i < 4; i++)
        {
            if (i != pickPoint)
                circle(tmp, dstPoint[i], radius, Scalar(228, 164, 140), -1);
            else
                circle(tmp, dstPoint[i], radius, Scalar(96, 96, 240), -1);
        }
        imshow("͸�ӱ任��", tmp);
    }
    else if (event == EVENT_LBUTTONUP && pickPoint >= 0)//����ɿ�
    {
        //ִ��͸�ӱ任
        Mat Trans = getPerspectiveTransform(srcPoint, dstPoint);//��4��ӳ���õ��任����
        warpPerspective(srcImg, dstImg, Trans, Size(dstImg.cols, dstImg.rows));//ִ��͸�ӱ任
        for (int i = 0; i < 4; i++)//��ʾ4��ӳ����λ��
        {
            //��һ��Բ����ӳ���(ת����)���뾶Ϊ10��������ϸΪ3����ɫ��Բ
            circle(dstImg, dstPoint[i], radius, Scalar(0, 215, 255), 3);
        }
        imshow("͸�ӱ任��", dstImg);
        pickPoint = -1;//����ѡȡ״̬
    }
}

void main()
{
    srcImg = imread("yourpic.jpg");
    if (!srcImg.empty()) // ����Ƿ�ɹ�����ͼƬ
    {
        // ��ȡͼƬ��Ⱥ͸߶�
        int imgWidth = srcImg.cols;
        int imgHeight = srcImg.rows;

        // �ж�ͼƬ��С�Ƿ񳬳���Ļ�ߴ�
        if (imgWidth > 800 || imgHeight > 600)
        {
            // �������ű���
            double scale = min(800 / static_cast<double>(imgWidth), 600 / static_cast<double>(imgHeight));

            // ����ͼƬ
            resize(srcImg, srcImg, Size(), scale, scale);
        }
    }
    else
    {
        cout << "�޷�����ͼƬ��" << endl;
    }

    radius = 10;//�����ĸ����Բ�İ뾶
    pickPoint = -1;
    //ӳ��ǰ��4����
    srcPoint[0] = Point2f(0, 0);
    srcPoint[1] = Point2f(srcImg.cols, 0);
    srcPoint[2] = Point2f(srcImg.cols, srcImg.rows);
    srcPoint[3] = Point2f(0, srcImg.rows);
    //����һ���Դ���ԭͼ��Ļ���
    dstImg = Mat::zeros(Size(2 * radius + 100 + srcImg.cols, 2 * radius + 100 + srcImg.rows), srcImg.type());
    //��ʼӳ����4����
    dstPoint[0] = Point2f(radius + 50, radius + 50);
    dstPoint[1] = Point2f(radius + 50 + srcImg.cols, radius + 50);
    dstPoint[2] = Point2f(radius + 50 + srcImg.cols, radius + 50 + srcImg.rows);
    dstPoint[3] = Point2f(radius + 50, radius + 50 + srcImg.rows);
    Mat Trans = getPerspectiveTransform(srcPoint, dstPoint);//��4��ӳ���õ��任����
    Mat dst_perspective;//�洢Ŀ��͸��ͼ��
    warpPerspective(srcImg, dstImg, Trans, Size(dstImg.cols, dstImg.rows));//ִ��͸�ӱ任

    for (int i = 0; i < 4; i++)//��ʾ��ʼ4��ӳ����λ��
    {
        //��һ��Բ����ӳ���(ת����)���뾶Ϊ10��������ϸΪ3����ɫ��Բ
        circle(dstImg, dstPoint[i], radius, Scalar(95, 180, 243), 3);
    }
    imshow("͸�ӱ任��", dstImg);
    //����¼�
    setMouseCallback("͸�ӱ任��", mouseHander);
    waitKey();
}