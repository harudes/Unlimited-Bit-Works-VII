#include "Point.h"

ostream& operator<<(ostream& os, Point p){
	os << "(" << p.getX() << ',' << p.getY()<<")";
	return os;
}

bool operator==(Point a, Point b){
    return (a.x==b.x) && (a.y==b.y);
}

bool operator!=(Point a, Point b){
    return (a.x!=b.x) || (a.y!=b.y);
}

Line::Line(Point a, Point b):u(a),v(b){}

Point::Point(coordenada a, coordenada b):x(a),y(b){

}//dtor

Point::Point(){

}

float euclidean(Point a, Point b) {
	return sqrt((pow(a.getX() - b.getX(), 2)) + (pow(a.getY() - b.getY(), 2)));
}

coordenada Point::getX(){return x;}
coordenada Point::getY(){return y;}

int Point::region(coordenada xMid, coordenada yMid){
    int rpta=0;
    if(x>xMid)
        rpta|=1;
    if(y<yMid)
        rpta|=2;
    return rpta;
}
