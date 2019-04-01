#ifndef TRIPOINT_H
#define TRIPOINT_H
#include <iostream>
#include <math.h>

using namespace std;

typedef float coordenada;

class TriPoint
{
    public:
        coordenada x,y,z;
        TriPoint(coordenada a, coordenada b, coordenada c);
        TriPoint();
        int region(coordenada xMid, coordenada yMid, coordenada zMid);
};

ostream& operator<<(ostream& os, TriPoint p);

bool operator==(TriPoint a, TriPoint b);

bool operator!=(TriPoint a, TriPoint b);

float euclidean(float a, float b);

float euclidean(TriPoint a, TriPoint b);

struct Line{
    Line(Point a, Point b);
    Point u,v;
};

#endif // TRIPOINT_H
