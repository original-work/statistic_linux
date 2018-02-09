/*
* =========================================================
*
*       Filename:  cthread.h
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
* =========================================================
* ==================== CHANGE REPORT HISTORY ===================
*|VERSION|UPDATE DATE|UPDATED BY|DESCRIPTION OF CHANGE|*

* ==================== END OF CHANGE REPORT HISTORY ============
*/

#if !defined(_SAUAPP_COMMON_CTHREAD_H)
#define _SAUAPP_COMMON_CTHREAD_H

#include <pthread.h>

class CThread {
public:
	CThread();
	virtual ~CThread();
	/*
	* 需要在派生类中被实现，
	* 可用于在启动线程前做初始化工作，
	* 要实现多线程功能需要在该函数中调用start方法
	*/
	virtual int open(void *args = 0) = 0;
	/*
	* 需要在派生类中被实现，
	* 可用线程的实际任务实现
	*/
	virtual int svc() = 0;
	pthread_t thread_id() const { return thread_id_; }
protected:
	/*
	* 用于启动线程任务，
	* 默认启动分离式线程，
	* 可以设置detached参数为false来创建非分离式线程
	*/
	int start(bool detached = true);
	/* 用于检测线程任务是否结束 */
	int test_cancel_thread() const { return stopped_; }
	/* 用于结束线程任务 */
	int cancel_thread();
private:
	static void* run(void *args);
private:
	/* 标记线程是否启动 true为已启动；false为未启动*/
	bool started_;
	/* 标记线程是否为分离式的 true为分离式的；false为非分离的*/
	bool detached_;
	/* 标记线程是否已结束 1为已停止；0为未停止*/
	int stopped_;
	pthread_t thread_id_;
	pthread_attr_t thread_attr_;
};

#endif  //_SAUAPP_COMMON_CTHREAD_H
