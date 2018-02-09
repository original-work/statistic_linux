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
#include "opencv2/opencv.hpp"
#include "cthread.h"
#include <deque>
#include <vector>
#include <string.h>
#include "tools.hpp"
#include "buffer.h"
#include <ctime>
#include <cerrno>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;
using namespace cv;
/*
* =====================================================================================
*        Class:  TcpClientMgrThread
*  Description:
* =====================================================================================
*/
class bufferThread : public CThread
{
public:
	bufferThread();     
	~bufferThread();
	int open(void *args = 0);
	int svc();
	int init(String rtsp_addr, deque<Mat> videoFrame);
	int getFrameSize();
	Mat getFrame();
	int stop();
	double getWidth();
	double getHeight();
	void setWidth(double width);
	void setHeight(double height);
protected:
	void loop_process();
private:
	bool all_stopped_;
	//deque<Mat> videoFrame_;
	Buffer buffer_;
	VideoCapture cap_;
	String rtsp_addr_;
	int capFailCounter_;
	bool capState_;
	double width_;
	double height_;
};

