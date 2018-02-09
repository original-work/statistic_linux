/*
* =========================================================
*
*       Filename:  tools.hpp
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
#ifndef __GET_HOST__
#define __GET_HOST__
#include <opencv2/opencv.hpp>
#include <math.h>
#include <time.h>

using namespace cv;
using namespace std;

namespace tools{

	typedef struct {
	    double r;       // a fraction between 0 and 1
	    double g;       // a fraction between 0 and 1
	    double b;       // a fraction between 0 and 1
	} rgb;

	typedef struct {
	    double h;       // angle in degrees
	    double s;       // a fraction between 0 and 1
	    double v;       // a fraction between 0 and 1
	} hsv;

	typedef struct {
	    double x;
	    double y;
	    double width;
	    double height;
	}my4d;

	typedef struct {
	    double a;
	    double b;
	}my2d;
	
	hsv   rgb2hsv(rgb in);
	rgb   hsv2rgb(hsv in);

	bool checkPosition(double boundaryPt1[], double boundaryPt2[], Point2d currPos, String inCriterion);
	double averageSize(deque<double> humanSizeSample);
	bool checkAdd(double x, double y, double w, double h, double maxW, double maxH, double tolerance);
	bool checkErase(double x, double y, double w, double h, double maxW, double maxH, double tolerance);
	Rect getExteriorRect(vector<Point> pts);
	void nothing(int, void*);
	double mod360(double realNum);
	void printWithTime(const char* content);
};

#endif
