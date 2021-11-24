#ifndef VIBE_H
#define VIBE_H
#define  WINSIZE 3
#include <opencv2/opencv.hpp>
 
class Vibe
{
public:
	Vibe(void);
    Vibe(cv::Mat img);
	void SetMinMatch(int nthreshold){g_MinMatch=nthreshold;}
	void SetRadius(int radius){g_Radius=radius;}
	void SetSampleNum(int num){g_SampleNum=num;}
	void SetThreshold(double t){g_threshold=t;}
    cv::Mat GetForeground(){return g_ForeImg;}
    cv::Mat GetSegMask(){return g_SegementMask;}
    void Detect(cv::Mat img);
	void ForegroundCombineEdge(); // 结合边缘信息
	// 实现后处理，主要用形态学算子
	void PostProcess();

    int GetPixCount(){return mPixCount;}
 
public:
	~Vibe(void);
 
private:	
	void ClearLongLifeForeground(int i_lifeLength=200); // 清除场景中存在时间较长的像素，i_lifeLength用于控制允许存在的最长时间
    double AreaDense(cv::Mat pFr,int AI,int AJ,int W,int H); //计算(i,j)处邻域大小为W×H的密度
	int GetRandom(int istart,int iend); // 默认istart=0,iend=15
	int GetRandom(int random);
	int GetRandom();// 产生一个随机数
	// 计算两个像素之间的欧式距离
    double CalcPixelDist(cv::Scalar bkCs, cv::Scalar curCs);
	// 按照Kim的方法来计算颜色畸变
    double CalcuColorDist(cv::Scalar bkCs, cv::Scalar curCs);
	int g_SampleNum;// Sample number for the models，默认为20
	int g_MinMatch; // 当前像素与背景模型匹配的最少个数，默认为2
	int g_Height;
	int g_Width;
	int g_Radius;// 球体的半径，默认为20
	int g_offset; //边界的宽和高
	double g_threshold; // 距离度量的阈值
	unsigned char ***g_Model;// 保存背景模型	
    cv::Mat g_ForeImg;// 保存前景图
    cv::Mat g_Edge;
 
    cv::Mat element;
 
    cv::Mat g_SegementMask; //分割掩膜
    cv::Mat g_UpdateMask; // 更新掩膜
    cv::Mat g_Gray;
    int ** LifeLength; // 记录前景点的生命长度,如果前景点的生命长度到达一定的阈值，则将其融入背景中去,且要随机两次。
    int mPixCount;
};

#endif
