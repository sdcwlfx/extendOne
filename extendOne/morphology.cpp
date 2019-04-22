#include <opencv2/opencv.hpp>
#include "stdio.h"
#include "math.h"
using namespace cv;


/**
形态学处理：结构体：大小、形状、中心
腐蚀：缩小物体且断开连接，合适的结构体可以去燥
膨胀：填充隙缝，保持形状
开操作：平滑轮廓（内平滑），断开较窄的窄颈并消除较细的突出物
比操作：平滑轮廓（外平滑），弥合较窄的间断，消除较小的孔洞，填充轮廓中的断裂
https://blog.csdn.net/futurewu/article/details/10047409
*/
IplImage *src = 0;
IplImage* dst = 0;

IplConvKernel* element = 0;
int element_shape = CV_SHAPE_RECT;

int max_iters = 10;
int open_close_pos = 0;
int erode_dilate_pos = 0;









//开/闭操作跟踪条回调方法
void OpenClose(int pos)
{
	int n = open_close_pos - max_iters;
	int an = n > 0 ? n : -n;
	element = cvCreateStructuringElementEx(an*2+1,an*2+1,an,an,element_shape,0);
	if (n < 0)
	{
		//先腐蚀再膨胀
		cvErode(src,dst,element,1);
		cvDilate(dst,dst,element,1);
	}
	else {
		//先膨胀、再腐蚀
		cvDilate(src,dst,element,1);
		cvErode(dst,dst,element,1);
	}
	cvReleaseStructuringElement(&element);
	cvShowImage("Open/Close",dst);

}

//腐蚀/膨胀回调方法
void ErodeDilate(int pos)
{
	int n = erode_dilate_pos - max_iters;
	int an = n > 0 ? n : -n;
	element = cvCreateStructuringElementEx(an*2+1,an*2+1,an,an,element_shape,0);
	if (n < 0)
	{
		//腐蚀
		cvErode(src,dst,element,1);
		
	}
	else
	{
		//膨胀
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
	//IplImage转Mat
	//Mat Img = cvarrToMat(src);
	//src = SelectMaxRegion(Img);



	//创建图像输出窗口
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
		//e键
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
	//释放资源
	cvReleaseImage(&src);
	cvReleaseImage(&dst);

	//销毁窗口
	cvDestroyAllWindows();

	return 0;
}
