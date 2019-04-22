#include <opencv2/opencv.hpp>
#include "stdio.h"
#include "math.h"
using namespace cv;


/**
��̬ѧ�����ṹ�壺��С����״������
��ʴ����С�����ҶϿ����ӣ����ʵĽṹ�����ȥ��
���ͣ����϶�죬������״
��������ƽ����������ƽ�������Ͽ���խ��խ����������ϸ��ͻ����
�Ȳ�����ƽ����������ƽ�������ֺϽ�խ�ļ�ϣ�������С�Ŀ׶�����������еĶ���
https://blog.csdn.net/futurewu/article/details/10047409
*/
IplImage *src = 0;
IplImage* dst = 0;

IplConvKernel* element = 0;
int element_shape = CV_SHAPE_RECT;

int max_iters = 10;
int open_close_pos = 0;
int erode_dilate_pos = 0;









//��/�ղ����������ص�����
void OpenClose(int pos)
{
	int n = open_close_pos - max_iters;
	int an = n > 0 ? n : -n;
	element = cvCreateStructuringElementEx(an*2+1,an*2+1,an,an,element_shape,0);
	if (n < 0)
	{
		//�ȸ�ʴ������
		cvErode(src,dst,element,1);
		cvDilate(dst,dst,element,1);
	}
	else {
		//�����͡��ٸ�ʴ
		cvDilate(src,dst,element,1);
		cvErode(dst,dst,element,1);
	}
	cvReleaseStructuringElement(&element);
	cvShowImage("Open/Close",dst);

}

//��ʴ/���ͻص�����
void ErodeDilate(int pos)
{
	int n = erode_dilate_pos - max_iters;
	int an = n > 0 ? n : -n;
	element = cvCreateStructuringElementEx(an*2+1,an*2+1,an,an,element_shape,0);
	if (n < 0)
	{
		//��ʴ
		cvErode(src,dst,element,1);
		
	}
	else
	{
		//����
		cvDilate(src,dst,element,1);
	}
	cvReleaseStructuringElement(&element);
	cvShowImage("Erode/Dilate",dst);
}

int main()
{

	src = cvLoadImage("F:/ustcsse/AdvancedImageProcessing/experiment/picture/natali.jpg");
	//src = imread("F:/ustcsse/AdvancedImageProcessing/experiment/picture/natali.jpg");
	//if (src.empty())
	if(src==0)
	{
		return -1;
	}
	printf("Hot keys:\n"
	"\tESC-quit the program\n"
	"\tr-use rectangle structuring element\n"
	"\te-use elliptic structuring element\n"
	"\tc-use cross-shaped structuring element\n"
	"\tENTER-loop through all the options\n");
	
	
	dst = cvCloneImage(src);
	//cvCvtColor(dst, dst, CV_BGR2GRAY);
	//IplImageתMat
	//Mat Img = cvarrToMat(src);
	//src = SelectMaxRegion(Img);



	//����ͼ���������
	cvNamedWindow("Open/Close");
	cvNamedWindow("Erode/Dilate");

	open_close_pos = erode_dilate_pos = max_iters;

	cvCreateTrackbar("iterations","Open/Close",&open_close_pos,max_iters*2+1,OpenClose);
	cvCreateTrackbar("iterations","Erode/Dilate",&erode_dilate_pos,max_iters*2+1,ErodeDilate);

	for (;;)
	{
		int c;
		OpenClose(open_close_pos);
		ErodeDilate(erode_dilate_pos);
		c = cvWaitKey(0);

		//esc
		if ((char)c == 27)
		{
			break;
		}
		//e��
		if ((char)c == 'e')
		{
			element_shape = CV_SHAPE_ELLIPSE;
		}
		else if ((char)c == 'r')
		{
			element_shape = CV_SHAPE_RECT;
		}
		else if ((char)c == 'c')
		{
			element_shape = CV_SHAPE_CROSS;
		}
		else if ((char)c == '\n')
		{
			element_shape = (element_shape+1)%3;
		}
	}
	//�ͷ���Դ
	cvReleaseImage(&src);
	cvReleaseImage(&dst);

	//���ٴ���
	cvDestroyAllWindows();

	return 0;
}
