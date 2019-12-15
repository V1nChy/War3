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


/** 管理时间 */
class MyTime  
{
public:
	MyTime();
	virtual ~MyTime();

	/** 得到逝去的时间, 从程序启动开始 
	 *	@param [TIMER_CMD] command: 时间命令
	 *	@remark: TIMER_RESET			: 重新初始化时间，游戏的开始或是重新开始的时候用，！重要
	 *			 TIMER_START			: 开始计时	
	 *			 TIMER_STOP				: 停止计时
	 *			 TIMER_ADVANCE			: 步进 0.1 秒
	 *			 TIMER_GETABSOLUTETIME	: 得到绝对时间 ！重要
	 *			 TIMER_GETAPPTIME		: 得到从调用 TIMER_RESET 到 目前的时间 ！重要
	 *			 TIMER_GETELAPSEDTIME	: 得到得到从上次调用 TIMER_GETELAPSEDTIME 到目前经过的时间，客户程序员不要调用，引擎会调用 ！重要
	 */
	static float Timer( TIMER_CMD command );

};

#endif
