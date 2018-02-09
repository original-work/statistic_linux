/*
* =====================================================================================
*
*       Filename:  buffer.cpp
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
#include "buffer.h"
using namespace std;


/*
https://msdn.microsoft.com/en-us/library/hh874752.aspx#condition_variable__wait_method

condition_variable::wait Method
Blocks a thread.
void wait(
unique_lock<mutex>& Lck);

template <class Predicate>
void wait(
unique_lock<mutex>& Lck,
Predicate Pred);

Parameters
Lck
A unique_lock<mutex> object.

Pred
Any expression that returns true or false.

Remarks
The first method blocks until the condition_variable object is signaled by a call to notify_one or notify_all. It can also wake up spuriously.
In effect, the second method executes the following code.

C++
	while(!Pred())
	wait(Lck);

*/


void Buffer::addFrame(Mat frame) {
	while (true) {
		std::unique_lock<std::mutex> locker(mu);
		cond.wait(locker, [this]() {return buffer_.size() < size_; });
		buffer_.push_back(frame);
		locker.unlock();
		cond.notify_all();
		return;
	}
}

Mat Buffer::getAndRemoveFrame() {
	while (true)
	{
		std::unique_lock<std::mutex> locker(mu);
		cond.wait(locker, [this]() {return buffer_.size() > 0; });
		Mat front = buffer_.front();
		buffer_.pop_front();
		locker.unlock();
		cond.notify_all();
		return front;
	}
}

int Buffer::getFrameSize() {
	while (true)
	{
		std::unique_lock<std::mutex> locker(mu);
		cond.wait(locker, [this]() {return buffer_.size() > 0; });
		int size = buffer_.size();
		locker.unlock();
		cond.notify_all();
		return size;
	}
}

Buffer::Buffer() {
}

