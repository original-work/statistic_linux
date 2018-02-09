/*
* =========================================================
*
*       Filename:  people.hpp
*
*    Description:  
*
*        Version:  1.0
*        Created:  2017.07.27
*       Revision:  none
*       Compiler:  gcc
*
*         Author:  wangxx
*        Contact: winer632@qq.com
*
* ==========================================================
* ================== CHANGE REPORT HISTORY ==================
*|VERSION|UPDATE DATE|UPDATED BY|DESCRIPTION OF CHANGE|*

* ================== END OF CHANGE REPORT HISTORY ===========
*/
#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include "tools.hpp"

using namespace cv;

class People{
private:
	double _x;
	double _y;
	double _w;
	double _h;
	Rect2d  _roi;
	double _hue;
	Scalar _color;
	Point2d _center;
	Rect2d _maxRoi;
	bool _isIn;
	Point2d _speed;
	int _missingCount;
	int _isInChangeFrameCount;
	Scalar hsv2rgb_xx();
	
public:
	People(); 
	~People();
	void init(double x, double y, double w, double h, Rect2d roi, double hue, double boundaryPt1[], double boundaryPt2[], int toleranceCountIOStatus, String inCriterion);
	double getX();
	double getY();
	double getW();
	double getH();
	Rect2d getRoi();
	double getHue();
	Scalar getColor();
	Point2d getCenter();
	Rect2d getMaxRoi();
	bool getIsIn();
	Point2d getSpeed();
	int getMissingCount();
	int getIsInChangeFrameCount();
	void setX(double x);
	void setY(double x);
	void setW(double w);
	void setH(double h);
	void setCenter(Point2d center);
	void setRoi(Rect2d roi);
	void setSpeed(Point2d speed);
	void setMissingCount(int missingCount);
	void setIsIn(bool isIn);
	void setIsInChangeFrameCount(int isInChangeFrameCount);
};
