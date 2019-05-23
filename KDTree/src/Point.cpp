#include "Point.h"

ostream& operator<<(ostream& os, Point p){
	os << '(';
	for(size_t i=0;i<p.val.size();++i){
        os<<p.val[i];
        if(i<p.val.size()-1)
            os<<',';
	}
	os << ')' ;
	return os;
}

bool operator==(Point a, Point b){
    if(a.val.size()!=b.val.size())
        return false;
    bool result=1;
    for(size_t i=0;i<a.val.size() && result;++i){
        result &= a.val[i] == b.val[i];
    }
    return result;
}

bool operator!=(Point a, Point b){
    if(a.val.size()!=b.val.size())
        return false;
    bool result=0;
    for(size_t i=0;i<a.val.size() && !result;++i){
        result |= a.val[i] != b.val[i];
    }
    return result;
}

Point::Point(vector<coordenada> u):val(u){
    //ctor
}

Point::Point():val(){
    vector<coordenada> p={0.0,0.0,0.0};
    val=p;
}

Color::Color(int a, int b, int c):R(a),G(b),B(c){}

Color::Color(){
    R=rand()/double(RAND_MAX);
    G=rand()/double(RAND_MAX);
    B=rand()/double(RAND_MAX);
}

Point::~Point(){
    //dtor
}

Line::Line(Point a, Point b):u(a),v(b){}
