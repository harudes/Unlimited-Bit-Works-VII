#include "TriPoint.h"

ostream& operator<<(ostream& os, Point p){
	os << "(" << p.x << ',' << p.y << ',' << p.z <<")";
	return os;
}

bool operator==(Point a, Point b){
    return (a.x==b.x) && (a.y==b.y);
}

bool operator!=(Point a, Point b){
    return (a.x!=b.x) || (a.y!=b.y);
}


TriPoint::TriPoint()
{
    //ctor
}

