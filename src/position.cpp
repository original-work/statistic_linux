/*
* =========================================================
*
*       Filename:  position.cpp
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
#include "position.hpp"

Position::Position(double x, double y, double w, double h){
	_x = x;
	_y = y;
	_w= w;
	_h = h;
}

double Position::getX(){
	return _x;
}

double Position::getY(){
	return _y;
}

double Position::getW(){
	return _w;
}

double Position::getH(){
	return _h;
}
