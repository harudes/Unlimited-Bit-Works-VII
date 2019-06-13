#pragma once
#include "rectangle.h"
#include <stack>
#include <chrono>

using namespace std;

class RTree
{
	int dimension, maxP;
	rectangle space;
	vector<RTree*> children;
	vector<Point> points;
public:
	RTree(int d, int m);
	float enlargement(Point E);
	bool findPoint(Point E);
	bool chooseLeaf(Point E, RTree *&p, stack<RTree*> &parents);
	void insertPoint(Point E, RTree *p);
	void pickNext(rectangle &NR, rectangle &NNR, vector<Point> &NP, vector<Point> &NNP, vector<bool> &flags);
	void pickNext(rectangle &NR, rectangle &NNR, vector<RTree*> &NP, vector<RTree*> &NNP, vector<bool> &flags);
	void pickSeeds(rectangle &NR, rectangle &NNR, vector<Point> &NP, vector<Point> &NNP, vector<bool> &flags);
	void pickSeeds(rectangle &NR, rectangle &NNR, vector<RTree*> &NP, vector<RTree*> &NNP, vector<bool> &flags);
	void splitBranch(stack<RTree*> parents);
	void split(stack<RTree*> parents);
	vector<Point> getPoints();
	vector<Line> getLines();
	vector<Line> getLines2();
	rectangle getSpace();
	bool onCircle(Point punto, float distancia);
	bool inRegion(Point punto, float distancia);
	bool inCube(Point punto);
	bool outCube(Point punto, float distancia);
	bool corner(Point punto, float distancia);
	vector<Point> cercanos(Point centro, float radio);
	virtual ~RTree();
};

