#pragma once
#include "Point.h"

class rectangle
{
	Point mins, maxs;
public:
	rectangle();
	rectangle(Point p);
	rectangle(Point u, Point v);
	rectangle(rectangle a, rectangle b);
	rectangle(rectangle r, Point p);
	float enlargement(Point p);
	float enlargement(rectangle r);
	vector<Line> getFigure();
	vector<coordenada> getMins();
	vector<coordenada> getMaxs();
	float area();
	virtual ~rectangle();
};

