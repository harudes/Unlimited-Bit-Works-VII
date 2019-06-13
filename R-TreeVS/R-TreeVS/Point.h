#pragma once


#include <vector>
#include <stdlib.h>
#include <iostream>
#include <math.h>

using namespace std;

typedef float coordenada;

class Point {
public:
	vector<coordenada> val;
	Point(vector<coordenada> u);
	Point();
	virtual ~Point();
};

struct Line {
	Line(Point a, Point b);
	Point u, v;
};

ostream& operator<<(ostream& os, Point p);

bool operator==(Point a, Point b);

bool operator!=(Point a, Point b);

float euclidean(Point a, Point b);

