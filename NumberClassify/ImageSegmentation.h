#ifndef IMAGESEGMENTATION_H
#define IMAGESEGMENTATION_H

#include "CImg.h"
#include <vector>
#include <list>
#include <iostream>
#include "fstream"
using namespace std;
using namespace cimg_library;

#define BinaryGap 135            //图像二值化全局阈值
#define BoundaryRemoveGap 7      //边缘全设为白色的距离
#define Infinite 249480          //视作无穷大

#define SingleNumberImgBoundary 5                //单张数字图像边缘填充宽度
#define YHistogramValleyMaxPixelNumber 0         //求Y方向直方图，谷的最少黑色像素个数
#define XHistogramValleyMaxPixelNumber 4         //求X方向直方图，谷的最少黑色像素个数
#define XHistogramLargerScaleThanAvgDist 3       //X方向直方图，分割点所在间距比平均间距大的倍数

#define SubImgBlackPixelPercentage 0.001         //一张子图内黑色像素超过一定百分比才算有数字
#define NumberImageBlackPixelPercentage 0.35     //单张数字图像黑色像素个数超过所有数字图像
												 //黑色像素个数均值的一定比例才算作有数字

struct PointPos {
	int x;
	int y;
	PointPos() : x(-1), y(-1) {}
	PointPos(int posX, int posY) : x(posX), y(posY) {}
};

class ImageSegmentation {
public:
	ImageSegmentation(const CImg<int>& _SrcImg);
	~ImageSegmentation();

	void processBinaryImage();                    //处理成二值图像
	CImg<int> getBinaryImage();                   //获取二值图像

	//处理数字字符划分
	void numberSegmentationMainProcess(const string baseAddress);
	CImg<int> getHistogramImage();                //获取y方向的直方图
	CImg<int> getImageWithDividingLine();         //获取带划分线的二值图
	CImg<int> getColoredNumberDividedImg();       //获取划分成不同颜色的字符图片
	CImg<int> getNumberDividedCircledImg();       //获取数字用框框分割的图片

private:
	//做y方向的直方图，找到行与行之间的分割线
	void findDividingLine();

	//通过分割线，将图片划分为一行行的数字
	void divideIntoBarItemImg();

	//对每一张划分的图的数字，做扩张
	void doDilationForEachBarItemImg(int barItemIndex);

	//连通区域标记算法
	void connectedRegionsTagging();
	void connectedRegionsTaggingOfBarItemImg(int barItemIndex);

	//添加新的类tag
	void addNewClass(int x, int y, int barItemIndex);

	//在正上、左上、左中、左下这四个邻点中找到最小的tag
	void findMinTag(int x, int y, int &minTag, PointPos &minTagPointPos, int barItemIndex);

	//合并某个点(x,y)所属类别
	void mergeTagImageAndList(int x, int y, const int minTag, const PointPos minTagPointPos, int barItemIndex);

	//存储图片，并输出一个txt的图像列表
	void saveSingleNumberImageAndImglist(int barItemIndex);

	//获取单个数字的包围盒
	void getBoundingOfSingleNum(int listIndex, int& xMin, int& xMax, int& yMin, int& yMax);


private:
	CImg<int> SrcGrayImg, BinaryImg, TagImage, HistogramImage, DividingImg;
	vector<CImg<int>> subImageSet;     //一行行数字图像
	int imgW, imgH;

	vector<int> inflectionYPointSet;    //拐点
	vector<PointPos> divideLinePointSet;    //直方图峰值划分线点集

	int tagAccumulate = -1;            //类别tag累加值

	vector<int> classTagSet;                   //类别tag列表
	vector<list<PointPos>> pointPosListSet;    //装载类别tag对应的所有点的位置的list的列表
	vector<list<PointPos>> pointPosListSetForDisplay;

	string basePath;    //单个数字图片生成、预测结果文本存放路径
	string imglisttxt = "";
};



#endif
