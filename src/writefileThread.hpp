/*
* =====================================================================================
*
*       Filename:  bufferThread.h
*
*    Description:
*
*        Version:  1.0
*        Created:  2017/8/7 9:40:11
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
#include <opencv2/opencv.hpp>
#include "cthread.h"
#include <fstream>
#include "tools.hpp"
#include <time.h>
#include <ctime>
#include <cerrno>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

using namespace cv;


class writefileThread : public CThread
{
public:
	writefileThread();     
	~writefileThread();
	int open(void *args = 0);
	int svc();
	int stop();
	void writefile(Point2d pos, double conArea);
protected:
	void loop_process();
private:
	bool all_stopped_;
	std::ofstream outfile_;
	char filename_[100];
	char old_filename_[100];
	char time_s_[100];
	time_t time_;
};

