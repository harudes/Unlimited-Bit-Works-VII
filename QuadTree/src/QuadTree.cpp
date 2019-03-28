#include <iostream>
#include "QuadTree.h"

bool findPoint(vector<Point> vec, Point p){
    for(size_t i=0;i<vec.size();++i){
        if(vec[i]==p)
            return 1;
    }
    return 0;
}

QuadTree::QuadTree(coordenada xMin, coordenada xMax, coordenada yMin, coordenada yMax, int maxP):minX(xMin),maxX(xMax),minY(yMin),maxY(yMax),maxPoints(maxP){
    midX=(minX+maxX)/2;
    midY=(minY+maxY)/2;
    for(int i=0;i<4;++i)
        regions[i]=NULL;
}

QuadTree::QuadTree(Point leftup, Point rightdown, int maxP):minX(leftup.getX()),maxX(rightdown.getX()),minY(rightdown.getY()),maxY(leftup.getY()),maxPoints(maxP){
    for(int i=0;i<4;++i)
        regions[i]=NULL;
}

void QuadTree::split(){
    QuadTree* p;
    regions[0]=new QuadTree(minX,midX,midY,maxY,maxPoints);
    regions[1]=new QuadTree(midX,maxX,midY,maxY,maxPoints);
    regions[2]=new QuadTree(minX,midX,minY,midY,maxPoints);
    regions[3]=new QuadTree(midX,maxX,minY,midY,maxPoints);
    for(unsigned int i=0;i<=maxPoints;++i){
        p=regions[points[i].region(midX,midY)];
        p->insertPoint(points[i],p);
    }
    points.clear();
}

bool QuadTree::searchPoint(Point punto, QuadTree *&qt){
    if(!regions[0])
        return findPoint(points,punto);
    else{
        qt=(regions[punto.region(midX,midY)]);
        return regions[punto.region(midX,midY)]->searchPoint(punto,qt);
    }
}

void QuadTree::insertPoint(coordenada x, coordenada y, QuadTree * p){
    Point punto(x,y);
    if(!searchPoint(punto,p)){
        p->points.push_back(punto);
            if(p->points.size()>maxPoints)
                p->split();
    }
}

void QuadTree::insertPoint(Point punto, QuadTree * p){
    if(!searchPoint(punto,p)){
        p->points.push_back(punto);
            if(p->points.size()>maxPoints)
                p->split();
    }
}

Point QuadTree::leftup() {
    return Point(minX,maxY);
}
Point QuadTree::rightup() {
    return Point(maxX, maxY);
}
Point QuadTree::leftdown() {
    return Point(minX, minY);
}
Point QuadTree::rightdown() {
    return Point(minX, minY);
}

bool QuadTree::inRegion(Point punto, float distancia) {
    bool c1 = punto.getX() >= minX && punto.getX() <= maxX && punto.getY() >= minY && punto.getY() <= maxY;
    if (!c1) {
        c1 = (punto.getX()<minX && punto.getX()+distancia>minX) || (punto.getX() > maxX && punto.getX() - distancia < maxX) || (punto.getY() < minY && punto.getY() + distancia > minY) || (punto.getY() > maxY && punto.getY() - distancia < maxY);
        bool c2 = euclidean(punto, leftup()) <= distancia || euclidean(punto, leftdown()) <= distancia || euclidean(punto, rightup()) <= distancia || euclidean(punto, rightdown()) <= distancia;
        c1 = c1 || c2;
    }
    return c1;
}

vector<Point> QuadTree::getPoints(){
    vector<Point> puntos;
    if (!regions[0]) {
        for (size_t i = 0; i < points.size(); ++i) {
                puntos.push_back(points[i]);
        }
    }
    else {
        vector<Point> aux;
        for (int i = 0; i < 4; ++i) {
            aux = regions[i]->getPoints();
            puntos.insert(puntos.end(),aux.begin(),aux.end());
        }
    }
	return puntos;
}

bool QuadTree::onCircle(Point punto, float distancia){
    return euclidean(punto, leftup()) <= distancia && euclidean(punto, leftdown()) <= distancia && euclidean(punto, rightup()) <= distancia && euclidean(punto, rightdown()) <= distancia;
}

vector<Point> QuadTree::cercanos(coordenada x, coordenada y, float radio) {
    Point centro(x, y);
    vector<Point> puntos;
    if(onCircle(centro,radio)){
        return getPoints();
    }
    else if (inRegion(centro, radio)) {
        if (!regions[0]) {
            for (size_t i = 0; i < points.size(); ++i) {
                if (euclidean(points[i], centro) <= radio)
                    puntos.push_back(points[i]);
            }
        }
        else {
            vector<Point> aux;
            for (int i = 0; i < 4; ++i) {
                aux = regions[i]->cercanos(x, y, radio);
                puntos.insert(puntos.end(),aux.begin(),aux.end());
            }
        }
    }
    return puntos;
}

vector<Line> QuadTree::getLines(){
    vector<Line> lines;
    Point
        a(maxX,midY),
        b(minX,midY),
        c(midX,maxY),
        d(midX,minY);
    if(regions[0]){
        lines.push_back(Line(a,b));
        lines.push_back(Line(c,d));
        for(int i=0;i<4;++i){
            vector<Line> aux=regions[i]->getLines();
            lines.insert(lines.end(),aux.begin(),aux.end());
        }
    }
    return lines;
}

QuadTree::~QuadTree()
{
    //dtor
}
