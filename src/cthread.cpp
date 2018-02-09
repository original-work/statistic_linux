/*
* =========================================================
*
*       Filename:  cthread.cpp
*
*    Description:  实现多线程功能，为多线程创建提供基础框架
*
*        Version:  1.0
*        Created:  2013.6.14
*       Revision:  none
*       Compiler:  gcc
*
*         Author:  wangxx
*        contact:  winer632@qq.com
*
* ==========================================================
* ================== CHANGE REPORT HISTORY ==================
*|VERSION|UPDATE DATE|UPDATED BY|DESCRIPTION OF CHANGE|*

* ================== END OF CHANGE REPORT HISTORY ===========
*/

#include "cthread.h"


CThread::CThread() {
	started_ = false;
}

CThread::~CThread() {
}

int CThread::start(bool detached) {
	int ret;
	detached_ = detached;

	ret = pthread_attr_init(&thread_attr_);
	if (ret != 0) {
		return -1;
	}

	if (detached_) {
		ret = pthread_attr_setdetachstate(&thread_attr_, PTHREAD_CREATE_DETACHED);
	}
	else {
		ret = pthread_attr_setdetachstate(&thread_attr_, PTHREAD_CREATE_JOINABLE);
	}
	if (ret != 0) {
		return -1;
	}

	ret = pthread_create(&thread_id_, &thread_attr_, CThread::run, (void*)this);
	if (ret != 0) {
		return -1;
	}

	started_ = true;
	stopped_ = 0;
	pthread_attr_destroy(&thread_attr_);
	return 0;
}

int CThread::cancel_thread() {
	stopped_ = 1;
	started_ = false;
	return 0;
}

void* CThread::run(void *args) {
	CThread* ptask = (CThread*)args;
	ptask->svc();
	return 0;
}