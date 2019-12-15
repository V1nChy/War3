
#ifndef	GEEVENTHANDLER_H
#define GEEVENTHANDLER_H


/** Event handler */
class MyEventHandler
{
public:
	MyEventHandler(){}
	virtual ~MyEventHandler() = 0 {}
	virtual void Execute() = 0;
};


/** static event handler, which can bind a static funciton */
class MyStaticEventHandler : public MyEventHandler
{
public:
	MyStaticEventHandler(void (*func)())
		: mFunction(func)
	{
	}

	virtual void Execute()
	{
		(*mFunction)();
	}
private:
	void (*mFunction)();
};

/** static event handler, which can bind a static funciton */
template<typename T> class MyInstanceEventHandler : public MyEventHandler
{
public:
	MyInstanceEventHandler(T *object, void (T::*method)()) :
	  mObject(object), mMethod(method)
	{
	}

	~MyInstanceEventHandler()
	{}

	virtual void Execute()
	{
		(mObject->*mMethod)();
	}

private:
	T *mObject;
	void (T::*mMethod)();
};


/** Create a handler used in  */
template<typename T>
static MyEventHandler *CreateInstanceEventHandler(T *object, void (T::*method)())
{
	return new MyInstanceEventHandler<T>(object, method);
}

static MyEventHandler* CreateStaticEventHandler(void (*func)())
{
	return new MyStaticEventHandler(func);
}

#endif