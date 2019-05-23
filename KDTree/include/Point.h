#ifndef POINT_H
#define POINT_H
#include <vector>
#include <stdlib.h>
#include <iostream>

using namespace std;

typedef float coordenada;

class Point{
public:
    vector<coordenada> val;
    Point(vector<coordenada> u);
    Point();
    virtual ~Point();
};

struct Line{
    Line(Point a, Point b);
    Point u,v;
};

ostream& operator<<(ostream& os, Point p);

bool operator==(Point a, Point b);

bool operator!=(Point a, Point b);

struct Color{
    double R,G,B;
    Color(int a, int b, int c);
    Color();
};

#endif // POINT_H
