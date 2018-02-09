/*
* =========================================================
*
*       Filename:  DrawCubic.h
*
*    Description:  实现在视频画面上画格子功能
*
*        Version:  1.0
*        Created:  2013.6.14
*       Revision:  none
*       Compiler:  gcc
*
*         Author:  wangxx
*        contact:  winer632@qq.com
*
* =========================================================
* ==================== CHANGE REPORT HISTORY ===================
*|VERSION|UPDATE DATE|UPDATED BY|DESCRIPTION OF CHANGE|*

* ==================== END OF CHANGE REPORT HISTORY ============
*/

#include "opencv2/opencv.hpp"
#include <stdio.h>
#include<string>
#include "common.h"
#include "tools.hpp"

using namespace cv;
using namespace std;

class DrawCubic{
	private:
		Point drawpoint_[30][30];
	public:
		DrawCubic();
		Mat Draw(String rtsp_addr, Mat frame, double width, double height);
};
