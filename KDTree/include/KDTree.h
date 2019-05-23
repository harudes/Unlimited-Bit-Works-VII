#ifndef KDTREE_H
#define KDTREE_H
#include <Point.h>
#include <algorithm>
#include <utility>
#include <chrono>

using namespace std::chrono;

bool findPoint(vector<Point> vec, Point p);

class KDTree
{
    int dimension, maxPoint, axis;
    coordenada median,minX,maxX,minY,maxY,minZ,maxZ;
    Color col;
    vector<Point> points;
    KDTree* sons[2];
    KDTree* father;
public:
    KDTree(int d, int maxP,int ax);
    bool searchPoint(Point p, KDTree *&kt);
    void insertPoint(Point p, KDTree* kt);
    void split();
    vector<Line> getLines(coordenada xMin, coordenada xMax, coordenada yMin, coordenada yMax, coordenada zMin, coordenada zMax);
    vector<Point> getPoints();
    virtual ~KDTree();
};

#endif // KDTREE_H
