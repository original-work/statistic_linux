/*
* =========================================================
*
*       Filename:  position.hpp
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

using namespace cv;

class Position{
private:
	double _x;
	double _y;
	double _w;
	double _h;

public:
	Position(double _x, double _y, double _w, double _h);
	double getX();
	double getY();
	double getW();
	double getH();
};
