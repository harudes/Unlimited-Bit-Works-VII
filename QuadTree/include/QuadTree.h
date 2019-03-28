#ifndef QUADTREE_H
#define QUADTREE_H
#include <vector>
#include <Point.h>

using namespace std;

bool findPoint(vector<Point> vec, Point p);



class QuadTree{
	coordenada minX,maxX,minY,maxY;
	coordenada midX,midY;
	int maxPoints;
	QuadTree* regions[4];
	vector<Point> points;
public:
	QuadTree(coordenada xMin, coordenada xMax, coordenada yMin, coordenada yMax, int maxP);
	QuadTree(Point leftup, Point rightdown, int maxP);
	void split();
	bool searchPoint(Point p, QuadTree *&qt);
	void insertPoint(Point punto, QuadTree * p);
	void insertPoint(coordenada x, coordenada y, QuadTree * p);
	Point leftup();
	Point rightup();
	Point leftdown();
	Point rightdown();
	vector<Point> getPoints();
	bool onCircle(Point punto, float distancia);
	bool inRegion(Point punto, float distancia);
	vector<Point> cercanos(coordenada x, coordenada y, float radio);
	vector<Line> getLines();
	~QuadTree();
};


#endif // QUADTREE_H
