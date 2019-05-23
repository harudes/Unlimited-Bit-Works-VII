#include "KDTree.h"

bool findPoint(vector<Point> vec, Point p){
    for(size_t i=0;i<vec.size();++i){
        if(vec[i]==p){
            return true;
        }
    }
    return false;
}

Point Partition(vector<Point> &A, vector<Point> &L, vector<Point> &R, int p, int axis){
	for(int i=0; i<A.size();i++){
		if(i==p)
			continue;
		else{
			if(A[i].val[axis]<=A[p].val[axis])
				L.push_back(A[i]);
			else
				R.push_back(A[i]);
		}
	}
	return A[p];
}

Point select_k(vector<Point> A, int k, int axis){
	if(A.size()==1)
		return A[0];
	int p=A.size()/2;
	vector<Point> L,R;
	A[p]=Partition(A,L,R,p,axis);
	if(L.size()==k)
		return A[p];
	else if(L.size()>k)
		return select_k(L,k,axis);
	else
		return select_k(R,k-L.size()-1,axis);
}

KDTree::KDTree(int d, int maxP,int ax):dimension(d),maxPoint(maxP),axis(ax){
    points.reserve(maxP);
    sons[0]=NULL;
    sons[1]=NULL;
    col=Color();
    father=this;
}

bool KDTree::searchPoint(Point p, KDTree *&kt){
    bool result;
    if(!kt->sons[0]){
        auto start = high_resolution_clock::now();
        result=findPoint(points,p);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(stop - start);
        cout<< duration.count()<<endl;
        return false;
    }
    else{
        kt=sons[p.val[axis]>median];
        return kt->searchPoint(p,kt);
    }
}

void KDTree::split(){
    if(points.size()>maxPoint){
        median=select_k(points,ceil(maxPoint/2),axis).val[axis];
        sons[0]=new KDTree(dimension,maxPoint,(axis+1)%dimension);
        sons[1]=new KDTree(dimension,maxPoint,(axis+1)%dimension);
        for(size_t i=0;i<points.size();++i){
            sons[points[i].val[axis]>median]->points.push_back(points[i]);
        }
        points.clear();
    }
}

void KDTree::insertPoint(Point p, KDTree* kt){
    if(!searchPoint(p,kt)){
        kt->points.push_back(p);
        if(kt->points.size()>maxPoint)
            kt->split();
    }
}

vector<Line> KDTree::getLines(coordenada xMin, coordenada xMax, coordenada yMin, coordenada yMax, coordenada zMin, coordenada zMax){
    vector<Line> lines;
    if(sons[0]){
        vector<coordenada> a={xMin,yMin,zMin},c={xMax,yMax,zMax};
        a[axis]=median;
        c[axis]=median;
        vector<coordenada> b,d;
        switch(axis){
            case 0: b={median,a[1],c[2]};d={median,c[1],a[2]};break;
            case 1: b={a[0],median,c[2]};d={c[0],median,a[2]};break;
            case 2: b={a[0],c[1],median};d={c[0],a[1],median};break;
        }
        lines.push_back(Line(Point(a),Point(b)));
        lines.push_back(Line(Point(b),Point(c)));
        lines.push_back(Line(Point(c),Point(d)));
        lines.push_back(Line(Point(d),Point(a)));
        vector<Line> temp;
        switch(axis){
            case 0:
                temp=sons[0]->getLines(xMin,median,yMin,yMax,zMin,zMax);
                lines.insert(lines.end(),temp.begin(),temp.end());
                temp=sons[1]->getLines(median,xMax,yMin,yMax,zMin,zMax);
                lines.insert(lines.end(),temp.begin(),temp.end());
                break;
            case 1:
                temp=sons[0]->getLines(xMin,xMax,yMin,median,zMin,zMax);
                lines.insert(lines.end(),temp.begin(),temp.end());
                temp=sons[1]->getLines(xMin,xMax,median,yMax,zMin,zMax);
                lines.insert(lines.end(),temp.begin(),temp.end());
                break;
            case 2:
                temp=sons[0]->getLines(xMin,xMax,yMin,yMax,zMin,median);
                lines.insert(lines.end(),temp.begin(),temp.end());
                temp=sons[1]->getLines(xMin,xMax,yMin,yMax,median,zMax);
                lines.insert(lines.end(),temp.begin(),temp.end());
                break;
        }
    }
    return lines;
}

vector<Point> KDTree::getPoints(){
    vector<Point> puntos;
    if (!sons[0]) {
        return points;
    }
    else {
        vector<Point> aux;
        for (int i = 0; i < 2; ++i) {
            aux = sons[i]->getPoints();
            puntos.insert(puntos.end(),aux.begin(),aux.end());
        }
    }
	return puntos;
}

KDTree::~KDTree(){
    //dtor
}
