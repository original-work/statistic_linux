/*
* =====================================================================================
*
*       Filename:  buffer.h
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
#include <mutex>
#include <deque>
#include "opencv2/opencv.hpp"
#include <condition_variable>  

using namespace std;
using namespace cv;

class Buffer
{
public:
	void addFrame(Mat frame);
	Mat getAndRemoveFrame();
	int getFrameSize();
	Buffer();
private:
	// Add them as member variables here
	std::mutex mu;
	std::condition_variable cond;

	// Your normal variables here
	deque<Mat> buffer_;
	const unsigned int size_ = 10;
};