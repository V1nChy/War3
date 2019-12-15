// MyTime.h: interface for the MyTime class.
//
//////////////////////////////////////////////////////////////////////
#ifndef _MY_TIME_H_
#define _MY_TIME_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <time.h>

enum TIMER_CMD { TIMER_RESET, TIMER_START, TIMER_STOP, TIMER_ADVANCE,
                     TIMER_GETABSOLUTETIME, TIMER_GETAPPTIME, TIMER_GETELAPSEDTIME };


/** ����ʱ�� */
class MyTime  
{
public:
	MyTime();
	virtual ~MyTime();

	/** �õ���ȥ��ʱ��, �ӳ���������ʼ 
	 *	@param [TIMER_CMD] command: ʱ������
	 *	@remark: TIMER_RESET			: ���³�ʼ��ʱ�䣬��Ϸ�Ŀ�ʼ�������¿�ʼ��ʱ���ã�����Ҫ
	 *			 TIMER_START			: ��ʼ��ʱ	
	 *			 TIMER_STOP				: ֹͣ��ʱ
	 *			 TIMER_ADVANCE			: ���� 0.1 ��
	 *			 TIMER_GETABSOLUTETIME	: �õ�����ʱ�� ����Ҫ
	 *			 TIMER_GETAPPTIME		: �õ��ӵ��� TIMER_RESET �� Ŀǰ��ʱ�� ����Ҫ
	 *			 TIMER_GETELAPSEDTIME	: �õ��õ����ϴε��� TIMER_GETELAPSEDTIME ��Ŀǰ������ʱ�䣬�ͻ�����Ա��Ҫ���ã��������� ����Ҫ
	 */
	static float Timer( TIMER_CMD command );

};

#endif
