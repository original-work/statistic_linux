/*
* =========================================================
*
*       Filename:  cthread.h
*
*    Description:  ʵ�ֶ��̹߳��ܣ�Ϊ���̴߳����ṩ�������
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
	* ��Ҫ���������б�ʵ�֣�
	* �������������߳�ǰ����ʼ��������
	* Ҫʵ�ֶ��̹߳�����Ҫ�ڸú����е���start����
	*/
	virtual int open(void *args = 0) = 0;
	/*
	* ��Ҫ���������б�ʵ�֣�
	* �����̵߳�ʵ������ʵ��
	*/
	virtual int svc() = 0;
	pthread_t thread_id() const { return thread_id_; }
protected:
	/*
	* ���������߳�����
	* Ĭ����������ʽ�̣߳�
	* ��������detached����Ϊfalse�������Ƿ���ʽ�߳�
	*/
	int start(bool detached = true);
	/* ���ڼ���߳������Ƿ���� */
	int test_cancel_thread() const { return stopped_; }
	/* ���ڽ����߳����� */
	int cancel_thread();
private:
	static void* run(void *args);
private:
	/* ����߳��Ƿ����� trueΪ��������falseΪδ����*/
	bool started_;
	/* ����߳��Ƿ�Ϊ����ʽ�� trueΪ����ʽ�ģ�falseΪ�Ƿ����*/
	bool detached_;
	/* ����߳��Ƿ��ѽ��� 1Ϊ��ֹͣ��0Ϊδֹͣ*/
	int stopped_;
	pthread_t thread_id_;
	pthread_attr_t thread_attr_;
};

#endif  //_SAUAPP_COMMON_CTHREAD_H
