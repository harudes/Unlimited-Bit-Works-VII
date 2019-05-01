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
        ot=(regions[punto.region(midX,midY,midZ)]);
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

vector<Line> OcTree::getBorderLines(){
    vector<Line> lines;
    Point
        a(maxX,minY,minZ),
        b(minX,minY,minZ),
        c(maxX,maxY,minZ),
        d(minX,maxY,minZ),
        e(maxX,minY,maxZ),
        f(minX,minY,maxZ),
        g(maxX,maxY,maxZ),
        h(minX,maxY,maxZ);
    if(regions[0]){
        lines.push_back(Line(a,b));
        lines.push_back(Line(c,d));
        lines.push_back(Line(e,f));
        lines.push_back(Line(g,h));
        lines.push_back(Line(a,e));
        lines.push_back(Line(b,f));
        lines.push_back(Line(c,g));
        lines.push_back(Line(d,h));
        lines.push_back(Line(a,c));
        lines.push_back(Line(b,d));
        lines.push_back(Line(e,g));
        lines.push_back(Line(f,h));
        for(int i=0;i<8;++i){
            vector<Line> aux=regions[i]->getBorderLines();
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

bool OcTree::onCircle(Point punto, float distancia){
    bool result=true;
    vector<vector<coordenada> > ps;
    vector<coordenada> p1={minX,minY,minZ};
    vector<coordenada> p2={maxX,maxY,maxZ};
    ps.push_back(p1);
    ps.push_back(p2);
    for(int i=0; i<2;++i){
        for(int j=0;j<2;++j){
            for(int k=0; k<2; ++k){
                Point p(ps[i][0],ps[j][1],ps[k][2]);
                result = result && (euclidean(punto,p)<=distancia);
            }
        }
    }
    return result;
}

bool OcTree::corner(Point punto, float distancia){
    bool result=false;
    vector<vector<coordenada> > ps;
    vector<coordenada> p1={minX,minY,minZ};
    vector<coordenada> p2={maxX,maxY,maxZ};
    ps.push_back(p1);
    ps.push_back(p2);
    for(int i=0; i<2;++i){
        for(int j=0;j<2;++j){
            for(int k=0; k<2; ++k){
                Point p(ps[i][0],ps[j][1],ps[k][2]);
                result = result || (euclidean(punto,p)<=distancia);
            }
        }
    }
    return result;
}

bool OcTree::inCube(Point punto){
    bool result=true;
    vector<vector<coordenada> > ps;
    vector<coordenada> p1={minX,minY,minZ};
    vector<coordenada> p2={maxX,maxY,maxZ};
    vector<coordenada> p={punto.x,punto.y,punto.z};
    ps.push_back(p1);
    ps.push_back(p2);
    for(int i=0; i<ps[0].size();++i){
        result = result && (p[i]>p1[i]);
        result = result && (p[i]<p2[i]);
    }
    return result;
}

bool OcTree::outCube(Point punto, float distancia){
    bool result=false;
    vector<vector<coordenada> > ps;
    vector<coordenada> p1={minX,minY,minZ};
    vector<coordenada> p2={maxX,maxY,maxZ};
    vector<coordenada> p={punto.x,punto.y,punto.z};
    ps.push_back(p1);
    ps.push_back(p2);
    for(int i=0; i<ps[0].size();++i){
        result = result || (p[i]<p1[i] && (p[i]+distancia)>p1[i]);
        result = result || (p[i]>p2[i] && (p[i]-distancia)<p2[i]);
    }
    return result;
}

bool OcTree::inRegion(Point punto, float distancia){
    bool result=false;
    if(corner(punto,distancia)){
        result=true;
    }
    else{
        if(inCube(punto)){
            result = true;
        }
        else{
            if(outCube(punto,distancia)){
                result = true;
            }
        }
    }
    return result;
}

vector<Point> OcTree::cercanos(Point centro, float radio){
    vector<Point> puntos;
    if(onCircle(centro,radio)){
        return getPoints();
    }
    else if(inRegion(centro, radio)){
        if (!regions[0]) {
            for (size_t i = 0; i < points.size(); ++i) {
                if (euclidean(points[i], centro) <= radio)
                    puntos.push_back(points[i]);
            }
        }
        else {
            vector<Point> aux;
            for (int i = 0; i < 8; ++i) {
                aux = regions[i]->cercanos(centro, radio);
                puntos.insert(puntos.end(),aux.begin(),aux.end());
            }
        }
    }
    return puntos;
}


OcTree::~OcTree(){
    //dtor
}
