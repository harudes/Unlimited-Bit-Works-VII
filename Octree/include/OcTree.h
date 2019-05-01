#ifndef OCTREE_H
#define OCTREE_H
#include <vector>
#include <Point.h>

using namespace std;

bool findPoint(vector<Point> vec, Point p);

class OcTree{
    coordenada minX,maxX,minY,maxY,minZ,maxZ,midX,midY,midZ;
    int maxPoints;
    OcTree* regions[8];
    vector<Point> points;
public:
    OcTree(coordenada xMin, coordenada xMax, coordenada yMin, coordenada yMax, coordenada zMin, coordenada zMax, int maxP);
    ocTree(Point leftup, Point rightdown, int maxP);
    void split();
    bool searchPoint(Point p, OcTree *&ot);
    void insertPoint(Point punto, OcTree * p);
    vector<Point> getPoints();
    vector<Line> getLines();
    vector<Line> getBorderLines();
    bool onCircle(Point punto, float distancia);
	bool inRegion(Point punto, float distancia);
	bool inCube(Point punto);
	bool outCube(Point punto, float distancia);
	bool corner(Point punto, float distancia);
	vector<Point> cercanos(Point centro, float radio);
    virtual ~OcTree();
};

#endif // OCTREE_H
