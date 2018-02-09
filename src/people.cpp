/*
* =========================================================
*
*       Filename:  people.cpp
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
#include "people.hpp"

using namespace cv;


Scalar People::hsv2rgb_xx(){
	tools::hsv hsv_color;
	hsv_color.h= tools::mod360(_hue);
	hsv_color.s=1;
	hsv_color.v=1;
	tools::rgb rgb_color;
	rgb_color=tools::hsv2rgb(hsv_color);
	return Scalar(rgb_color.r, rgb_color.g, rgb_color.b);
}


People::People(){
}

People::~People(){
}

void People::init(double x, double y, double w, double h, Rect2d roi, double hue, double boundaryPt1[], double boundaryPt2[], int toleranceCountIOStatus, String inCriterion){
	_x = x;
	_y = y;
	_w = w;
	_h = h;
	_roi = Rect2d(roi.x,roi.y,roi.width,roi.height);
	_hue = hue;
	_color=hsv2rgb_xx();
	_center.x = _x + _w / 2;
	_center.y = _y + _h / 2;
	Point2d currPos(_center.x,_center.y);
	_isIn=tools::checkPosition(boundaryPt1, boundaryPt2, currPos, inCriterion);
	_isInChangeFrameCount=toleranceCountIOStatus;
	_speed.x = 0;
	_speed.y = 0;
	_missingCount = 0;

	//ROI - Region of Interest
	_maxRoi = roi;
}

double People::getX(){return _x;}
double People::getY(){return _y;}
double People::getW(){return _w;}
double People::getH(){return _h;}
Rect2d People::getRoi(){return _roi;}
double People::getHue(){return _hue;}
Scalar People::getColor(){return _color;};
Point2d People::getCenter(){return _center;}
Rect2d People::getMaxRoi(){return _maxRoi;}
bool People::getIsIn(){return _isIn;}
Point2d People::getSpeed(){return _speed;}
int People::getMissingCount(){return _missingCount;}
int People::getIsInChangeFrameCount(){return _isInChangeFrameCount;}

void People::setX(double x){_x=x;}
void People::setY(double y){_y=y;}
void People::setW(double w){_w=w;}
void People::setH(double h){_h=h;}
void People::setCenter(Point2d center){
	_center.x=center.x;
	_center.y=center.y;
}
void People::setRoi(Rect2d roi){
	_roi=roi;
	// Automatically update maxRoi as roi is updated
    if(_roi.height*_roi.width > _maxRoi.height*_maxRoi.width){
        _maxRoi = _roi;
    }
}
void People::setSpeed(Point2d speed){_speed=speed;}
void People::setMissingCount(int missingCount){_missingCount=missingCount;}
void People::setIsIn(bool isIn){_isIn=isIn;}
void People::setIsInChangeFrameCount(int isInChangeFrameCount){_isInChangeFrameCount=isInChangeFrameCount;}


