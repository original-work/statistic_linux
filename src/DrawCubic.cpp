/*
* =====================================================================================
*
*       Filename:  DrawCubic.cpp
*
*    Description:
*
*        Version:  1.0
*        Created:  2017/9/21 18:34:00
*       Revision:  none
*       Compiler:  gcc
*
*         Author:  wangxx
*        contact:  winer632@qq.com
*
* =====================================================================================
* ============================== CHANGE REPORT HISTORY ================================
* |   VERSION   |   UPDATE DATE   |   UPDATED BY   |   DESCRIPTION OF CHANGE   |*

* =========================== END OF CHANGE REPORT HISTORY ============================
*/

#include "DrawCubic.h"

using namespace cv;
using namespace std;

DrawCubic::DrawCubic(){
}


Mat DrawCubic::Draw(String rtsp_addr, Mat frame, double width, double height){
	//tools::printWithTime("Draw begin !");
	double width_div=width/VERTICAL_NUM;
	double height_div=height/HORIZONTAL_NUM;

	for(int x=0;x<=VERTICAL_NUM;x++){
		for(int y=0;y<=HORIZONTAL_NUM;y++){
			//printf("point[%d][%d] is (%f,%f)\n", x, y, x*width_div, y*height_div);
			Point point(x*width_div, y*height_div);
			drawpoint_[x][y] = point;
		}
		line(frame, drawpoint_[x][0], drawpoint_[x][HORIZONTAL_NUM], Scalar(255, 255, 255), 1);
	}


	for(int y=0;y<=HORIZONTAL_NUM;y++){
		line(frame, drawpoint_[0][y], drawpoint_[VERTICAL_NUM][y], Scalar(255, 255, 255), 1);
	}
	//tools::printWithTime("Draw end !");
	return frame;
}


