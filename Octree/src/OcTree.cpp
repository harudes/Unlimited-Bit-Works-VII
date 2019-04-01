#include "OcTree.h"

bool findPoint(vector<Point> vec, Point p){
    for(size_t i=0;i<vec.size();++i){
        if(vec[i]==p)
            return true;
    }
    return false;
}


OcTree::OcTree(coordenada xMin, coordenada xMax, coordenada yMin, coordenada yMax, coordenada zMin, coordenada zMax, int maxP):minX(xMin),maxX(xMax),minY(yMin),maxY(yMax),minZ(zMin),maxZ(zMax),maxPoints(maxP){
    midX=(minX+maxX)/2;
    midY=(minY+maxY)/2;
    midZ=(minZ+maxZ)/2;
    for(int i=0;i<8;++i)
        regions[i]=NULL;
}

void OcTree::split(){
    OcTree* p;
    regions[0]=new OcTree(minX,midX,midY,maxY,minZ,midZ,maxPoints);
    regions[1]=new OcTree(midX,maxX,midY,maxY,minZ,midZ,maxPoints);
    regions[2]=new OcTree(minX,midX,minY,midY,minZ,midZ,maxPoints);
    regions[3]=new OcTree(midX,maxX,minY,midY,minZ,midZ,maxPoints);
    regions[4]=new OcTree(minX,midX,midY,maxY,midZ,maxZ,maxPoints);
    regions[5]=new OcTree(midX,maxX,midY,maxY,midZ,maxZ,maxPoints);
    regions[6]=new OcTree(minX,midX,minY,midY,midZ,maxZ,maxPoints);
    regions[7]=new OcTree(midX,maxX,minY,midY,midZ,maxZ,maxPoints);
    for(unsigned int i=0;i<=maxPoints;++i){
        p=regions[points[i].region(midX,midY,midZ)];
        p->insertPoint(points[i],p);
    }
    points.clear();
}

bool OcTree::searchPoint(Point punto, OcTree *&ot){
    if(!regions[0])
        return findPoint(points,punto);
    else{
        return ot->searchPoint(punto,ot);
    }
}

void OcTree::insertPoint(Point punto, OcTree * p){
    if(!searchPoint(punto,p)){
        p->points.push_back(punto);
            if(p->points.size()>maxPoints)
                p->split();
    }
}

vector<Point> OcTree::getPoints(){
    vector<Point> puntos;
    if (!regions[0]) {
        puntos.insert(puntos.end(),points.begin(),points.end());
    }
    else {
        vector<Point> aux;
        for (int i = 0; i < 8; ++i) {
            aux = regions[i]->getPoints();
            puntos.insert(puntos.end(),aux.begin(),aux.end());
        }
    }
	return puntos;
}

vector<Line> OcTree::getPointLines(){
    vector<Line> lines;
    if (!regions[0]) {
        for (size_t i = 0; i < points.size(); ++i) {
                lines.push_back(Line(points[i],Point(midX,midY,midZ)));
        }
    }
    else {
        vector<Line> aux;
        for (int i = 0; i < 8; ++i) {
            aux = regions[i]->getPointLines();
            lines.insert(lines.end(),aux.begin(),aux.end());
        }
    }
	return lines;
}

vector<Line> OcTree::getLines(){
    vector<Line> lines;
    Point
        a(maxX,midY,midZ),
        b(minX,midY,midZ),
        c(midX,maxY,midZ),
        d(midX,minY,midZ),
        e(midX,midY,maxZ),
        f(midX,midY,minZ);
    if(regions[0]){
        lines.push_back(Line(a,b));
        lines.push_back(Line(c,d));
        lines.push_back(Line(e,f));
        for(int i=0;i<8;++i){
            vector<Line> aux=regions[i]->getLines();
            lines.insert(lines.end(),aux.begin(),aux.end());
        }
    }
    return lines;
}

OcTree::~OcTree(){
    //dtor
}
