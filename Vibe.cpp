#include "Vibe.h"
 
using namespace std;

Vibe::Vibe(void)
{
	g_Radius=20;
	g_MinMatch=2;	
	g_SampleNum=20;
	g_offset=(WINSIZE-1)/2;
    mPixCount = 0;
 
}
 
Vibe::Vibe(cv::Mat img)
{
    this->g_Height=img.rows;
    this->g_Width=img.cols;
 
    g_Radius=5;
	g_MinMatch=2;	
    g_SampleNum=5;
    g_threshold=3;
	g_offset=(WINSIZE-1)/2;
 
    g_ForeImg=cv::Mat(img.size(), CV_8UC1);
    g_Gray=cv::Mat(img.size(), CV_8UC1);
    g_Edge=cv::Mat(img.size(), CV_8UC1);
    g_SegementMask=cv::Mat(img.size(), CV_8UC1);
    g_UpdateMask=cv::Mat(img.size(), CV_8UC1);
 
    element=cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5,5)/*, cv::Point(1, 1)*/);

	// 以上完成相关的初始化操作
	/**********************  以下实现第一帧在每个像素的8邻域内的采样功能，建立对应的背景模型*****************************/
    img.copyTo(g_Gray);
	int i=0,j=0,k=0;
	g_Model=new unsigned char**[g_SampleNum];
	for (k=0;k<g_SampleNum;k++)
	{
		g_Model[k]=new unsigned char *[g_Height];
		for(i=0;i<g_Height;i++)
		{
			g_Model[k][i]=new unsigned char [g_Width];
			for (j=0;j<g_Width;j++)
				g_Model[k][i][j]=0;
		}
	}
	
	// 采样进行背景建模	
	double dVal;
	int ri=0,rj=0; //随机采样的值
	for (i=g_offset;i<g_Height-g_offset;i++)
	{
		for (j=g_offset;j<g_Width-g_offset;j++)
		{
			// 周围3*3的邻域内进行采样
			for(k=0;k<g_SampleNum;k++)
			{
				ri=GetRandom(i);
				rj=GetRandom(j);
                dVal=g_Gray.at<uchar>(ri, rj);
				g_Model[k][i][j]=dVal;							
			}
		}
	}
 
	// 初始化前景点掩膜的生命长度
	LifeLength=new int *[g_Height];
	for (i=0;i<g_Height;i++)
	{
		LifeLength[i]=new int [g_Width];
		for(j=0;j<g_Width;j++)
			LifeLength[i][j]=0;
	}
}
 
 
void Vibe::Detect(cv::Mat img)
{
    g_ForeImg=cv::Mat::zeros(g_ForeImg.rows, g_ForeImg.cols, g_ForeImg.type());
    img.copyTo(g_Gray);
	int i=0,j=0,k=0;
	double dModVal,dCurrVal;
	int tmpCount=0;// 距离比较在阈值内的次数
	double tmpDist=0;	
	int iR1,iR2;//产生随机数
	int Ri,Rj; // 产生邻域内X和Y的随机数
 
    cv::Mat bmsk=cv::Mat(img.size(), CV_8UC1);
    g_Gray.copyTo(bmsk);

    cv::compare(bmsk,0,bmsk, cv::CMP_NE);

	for (i=0;i<g_Height;i++)
	{		
		for (j=0;j<g_Width;j++)
		{			
			if( i < g_offset || j < g_offset || i> g_Height - g_offset || j> g_Width - g_offset )
			{
                g_ForeImg.at<uchar>(i,j)=0;
				continue;
			}
			else
			{
				tmpCount=0;
                dCurrVal=g_Gray.at<uchar>(i, j);
				for (k=0;k<g_SampleNum && tmpCount<g_MinMatch  ;k++)
				{					
					dModVal=g_Model[k][i][j];
					tmpDist=fabs(dModVal-dCurrVal);									
					if (tmpDist<g_Radius)
						tmpCount++;				
				}
 
				//判断是否匹配上
				if (tmpCount>=g_MinMatch)
				{
                    g_ForeImg.at<uchar>(i,j)=0;
					// 背景模型的更新					
					iR1=GetRandom(0,15);
					if (iR1==0)
					{
						iR2=GetRandom();
						g_Model[iR2][i][j]=dCurrVal;						
					}
 
					//进一步更新邻域模型
					
					iR1=GetRandom(0,15);
					if (iR1==0)
					{
						Ri=GetRandom(i);
						Rj=GetRandom(j);
						iR2=GetRandom();
						g_Model[iR2][Ri][Rj]=dCurrVal;						
					}						
				}
				else
                    g_ForeImg.at<uchar>(i,j)=255;
			}
        }
	}		
    cv::multiply(g_ForeImg,bmsk,g_ForeImg,1);

    ForegroundCombineEdge();
	ClearLongLifeForeground();
    PostProcess();
//    DeleteSmallAreaInForeground(1000);
    mPixCount = cv::countNonZero(g_ForeImg);
}
 
double Vibe::AreaDense(cv::Mat pFr,int AI,int AJ,int W,int H)
{
	if (AI<=2 || AJ<=2 || AJ>=(g_Width-2) || AI>=(g_Height-2))
	{
		return 0;
	}
	int Num=0,i=0,j=0;
	double dVal=0,dense=0;
	int Total=(2*H+1)*(2*W+1);
	for (i=AI-H;i<=AI+H;i++)
	{
		for (j=AJ-W;j<=AJ+W;j++)
		{
            dVal=pFr.at<uchar>(i, j);
			if (dVal>200)
				Num++;
		}
	}
	dense=(double)Num/(double)Total;
	return dense;
}
 
void Vibe::ForegroundCombineEdge()
{	
    g_Edge=cv::Mat::zeros(g_Edge.rows, g_Edge.cols, g_Edge.type());
    cv::Canny(g_Gray, g_Edge, 30, 200, 3);
	int i=0,j=0;
	double dense;
	double dVal;
	for (i=g_offset;i<g_Height-g_offset;i++)
	{
		for (j=g_offset;j<g_Width-g_offset;j++)
		{
			dense=AreaDense(g_ForeImg,i,j,2,2);
            dVal=g_Edge.at<uchar>(i, j);
			if (dense>0.2 && dVal>200)
                g_ForeImg.at<uchar>(i, j)=255;
		}
	}
 
}
 
 
void Vibe::ClearLongLifeForeground(int i_lifeLength/* =200 */)
{
	int i=0,j=0;
	double dVal=0;
	double dLife=0;
	int iR1,iR2=0;
	double dCurrVal=0;
	for (i=g_offset;i<g_Height-g_offset;i++)
	{
		for (j=g_offset;j<g_Width-g_offset;j++)
		{
            dVal=g_ForeImg.at<uchar>(i, j);
			dLife=LifeLength[i][j];
			if (dLife>i_lifeLength)
			{
				LifeLength[i][j]=0;
                dCurrVal=g_Gray.at<uchar>(i, j);
				// 更新背景模型
				iR1=GetRandom();
				iR2=GetRandom();
				g_Model[iR1][i][j]=dCurrVal;
				g_Model[iR2][i][j]=dCurrVal;
			}
			else
				LifeLength[i][j]=dLife+1;
		}
	}
}
 

 
void Vibe::PostProcess()
{
    g_SegementMask=cv::Mat::zeros(g_SegementMask.rows, g_SegementMask.cols, g_SegementMask.type());
    cv::morphologyEx(g_ForeImg, g_SegementMask, cv::MORPH_OPEN, element);
}
 
//算颜色畸变
double Vibe::CalcuColorDist(cv::Scalar bkCs, cv::Scalar curCs)
{
	double r,g,b,br,bg,bb;
	r=curCs.val[0];
	g=curCs.val[1];
	b=curCs.val[2];
 
	br=bkCs.val[0];
	bg=bkCs.val[1];
	bb=bkCs.val[2];
 
	double curDist=r*r+g*g*b*b; 
	double bkDist=br*br+bg*bg+bb*bb;
 
	double curBK=r*br+g*bg+b*bb;
	double curbkDist=curBK*curBK;
	double SquareP;
	if (bkDist==0.0)
		SquareP=0;
	else
		SquareP=curbkDist/bkDist;
	double dist=sqrtf(curDist-SquareP);
	return dist;	
}
 
double Vibe::CalcPixelDist(cv::Scalar bkCs,cv::Scalar curCs)
{
	double tmpDist=pow(bkCs.val[0]-curCs.val[0],2)+pow(bkCs.val[1]-curCs.val[1],2)+pow(bkCs.val[2]-curCs.val[2],2);
	return sqrtf(tmpDist);
}
 
int Vibe::GetRandom()
{
	int val = g_SampleNum * 1.0 * rand() / RAND_MAX;	
	if( val == g_SampleNum )
		return val - 1;
	else
		return val;
}
 
int Vibe::GetRandom(int random)
{
	int val=random-g_offset+rand()%(2*g_offset);
	if (val<random-g_offset)
		val=random-g_offset;
	if (val>random+g_offset)
		val=random+g_offset;
	return val;	
}
 
int Vibe::GetRandom(int istart,int iend)
{
	int val=istart+rand()%(iend-istart);
	return val;
}
 
 
Vibe::~Vibe(void)
{

}
