#ifndef _POINT2D
#define _POINT2D

/** 泛型二维点,主要用于GUI,屏幕上的点输出 */
template<class T>
class Point2D
{
public:
	T x,y;

	/** 把坐标系中两个坐标点的X与Y各自相加 */
	Point2D operator + (Point2D p) const 
	{
		Point2D result;
		result.x = x + p.x;
		result.y = y + p.y;
		return result;
	}

	/** 把坐标系中两个坐标点的X与Y各自相减 */
	Point2D operator - (Point2D p) const
	{
		Point2D result;
		result.x = x - p.x;
		result.y = y - p.y;
		return result;
	}

	/** 给坐标系中的点赋值 */
	void operator = (Point2D p)
	{
		x = p.x;
		y = p.y;
	}

	/** 比较两个坐标系中的点是否相等 */
	bool operator == (Point2D p) const
	{
		if (x == p.x && y == p.y)
			return true;
		else
			return false;
	}

	/** 比较两个坐标中的点是否不相等 */
	bool operator != (Point2D p) const
	{
		if (x != p.x || y != p.y)
			return true;
		else
			return false;
	}
};

#endif