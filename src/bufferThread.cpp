/*
* =====================================================================================
*
*       Filename:  bufferThread.cpp
*
*    Description:
*
*        Version:  1.0
*        Created:  2017/8/7 10:00:00
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
#include "bufferThread.h"

bufferThread::bufferThread()
{
}

bufferThread::~bufferThread()
{
}

int bufferThread::open(void *args)
{
	all_stopped_ = false;

	if (start() == -1)
	{
		return -1;
	}

	return 0;
}

int bufferThread::svc()
{
	for (;;)
	{
		if (test_cancel_thread() == 1)
		{
			all_stopped_ = true;
			break;
		}
		else
		{
			loop_process();
			usleep(2);
		}
	}
	return 0;
}

double bufferThread::getWidth(){
	return width_;
}

double bufferThread::getHeight(){
	return height_;
}

void bufferThread::setWidth(double width){
	width_=width;
}

void bufferThread::setHeight(double height){
	height_=height;
}

int bufferThread::init(String rtsp_addr, deque<Mat> videoFrame)
{
	VideoCapture cap(rtsp_addr);
	cap_ = cap;
	//videoFrame_ = videoFrame;
	rtsp_addr_ = rtsp_addr;
	capFailCounter_ = 0;
	capState_ = true;

	double fps = cap.get(CAP_PROP_FPS);
	printf("fps is %f\n", fps);
	double width = cap.get(CAP_PROP_FRAME_WIDTH);
	double height = cap.get(CAP_PROP_FRAME_HEIGHT);
	setWidth(width);
	setHeight(height);
	printf("width is %f\n", width);
	printf("height is %f\n", height);

	time_t now = time(NULL);
	struct tm* timenow= localtime(&now);
	
	printf("startTime: %d-%02d-%02d %02d:%02d:%02d\n",  timenow->tm_year + 1900,
		timenow->tm_mon + 1,
		timenow->tm_mday,
		timenow->tm_hour,
		timenow->tm_min,
		timenow->tm_sec);
	
	return 0;
}		

int bufferThread::getFrameSize()
{
	return buffer_.getFrameSize();
}

Mat bufferThread::getFrame()
{
	//printf("getFrameSize() is %zd\n", getFrameSize());
	Mat	frame = buffer_.getAndRemoveFrame();
	//printf("after pop_front, getFrameSize() is %zd\n", getFrameSize());
	return frame;
}

void bufferThread::loop_process()
{
	Mat frame;
	char const * str1 = "Reinitialized success! NetCamera is working normally!";
	char const * str2 = "Reinitialized success! NetCamera is working normally!";
	if (cap_.isOpened()) {
		cap_ >> frame;
	}
	if (!frame.empty()){
		if (capState_ == false) {
			capState_ = true;
			tools::printWithTime(str1);
		}
		buffer_.addFrame(frame);
	}
	else {
		capFailCounter_++;
		if (capFailCounter_ > 1000) {
			capState_ = false;
			tools::printWithTime(str2);
			VideoCapture cap(rtsp_addr_);
			cap_ = cap;
			capFailCounter_ = 0;
		}
		
	}
}

int bufferThread::stop()
{
	cancel_thread();
	while (!all_stopped_) {
		usleep(1);
	}
	return 0;
}

