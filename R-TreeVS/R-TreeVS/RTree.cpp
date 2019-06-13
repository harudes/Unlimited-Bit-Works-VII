#include "RTree.h"

RTree::RTree(int d, int m) :dimension(d), maxP(m) {
	//ctor
}

bool RTree::findPoint(Point E) {
	for (size_t i = 0; i < points.size(); ++i) {
		if (points[i] == E)
			return true;
	}
	return false;
}

float RTree::enlargement(Point E) {
	float result = 0;
	rectangle newSpace(space, E);
	return newSpace.area() - space.area();

}

rectangle RTree::getSpace() {
	return space;
}

bool RTree::chooseLeaf(Point E, RTree *&p, stack<RTree*> &parents) {
	if (p->children.size() > 0) {
		parents.push(p);
		p = children[0];
		float minEnlargement = children[0]->enlargement(E), temp;
		vector<RTree*> posibleChild;
		for (size_t i = 0; i < children.size(); ++i) {
			temp = children[i]->enlargement(E);
			if (temp < minEnlargement) {
				minEnlargement = temp;
				posibleChild.clear();
				posibleChild.push_back(children[i]);
				//p=children[i];
			}
			if (temp == minEnlargement)
				posibleChild.push_back(children[i]);
		}
		if (posibleChild.size() > 0) {
			p = posibleChild[0];
			float minArea = posibleChild[0]->space.area(), temp;
			for (size_t i = 1; i < posibleChild.size(); ++i) {
				temp = posibleChild[i]->space.area();
				if (temp < minArea) {
					minArea = temp;
					p = posibleChild[i];
				}
			}
		}
		return p->chooseLeaf(E, p, parents);
	}
	else {
		return findPoint(E);
	}
}

void RTree::pickSeeds(rectangle &NR, rectangle &NNR, vector<RTree*> &NP, vector<RTree*> &NNP, vector<bool> &flags) {
	float maxD = 0, temp;
	int i1, i2;
	for (size_t i = 0; i < children.size(); ++i) {
		for (size_t j = i + 1; j < children.size(); ++j) {
			temp = rectangle(children[i]->space, children[j]->space).area() - (children[i]->space.area() + children[j]->space.area());
			if (temp > maxD) {
				maxD = temp;
				i1 = i;
				i2 = j;
			}
		}
	}
	NR = children[i1]->space;
	NNR = children[i2]->space;
	NP.push_back(children[i1]);
	NNP.push_back(children[i2]);
	flags[i1] = false;
	flags[i2] = false;
}

void fillNode(rectangle &R, vector<RTree*> &P, vector<bool> flags, vector<RTree*> points) {
	for (size_t i = 0, top = points.size(); i < top; ++i) {
		if (flags[i]) {
			P.push_back(points[i]);
			R = rectangle(R, points[i]->getSpace());
		}
	}
}

void RTree::pickNext(rectangle &NR, rectangle &NNR, vector<RTree*> &NP, vector<RTree*> &NNP, vector<bool> &flags) {
	float maxD = 0, temp;
	int i1;
	for (size_t i = 0; i < children.size(); ++i) {
		if (flags[i]) {
			temp = abs(NR.enlargement(children[i]->space) - NNR.enlargement(children[i]->space));
			if (temp > maxD) {
				maxD = temp;
				i1 = i;
			}
		}
	}
	if (NR.enlargement(children[i1]->space) < NNR.enlargement(children[i1]->space)) {
		NP.push_back(children[i1]);
		NR = rectangle(NR, children[i1]->space);
	}
	else {
		NNP.push_back(children[i1]);
		NNR = rectangle(NNR, children[i1]->space);
	}
	flags[i1] = false;
}

void RTree::splitBranch(stack<RTree*> parents) {
	parents.pop();
	vector<bool> flags(children.size(), true);
	rectangle NR, NNR;
	vector<RTree*> NP, NNP;
	pickSeeds(NR, NNR, NP, NNP, flags);
	while (NP.size() <= floor(maxP / 2) && NNP.size() <= floor(maxP / 2)) {
		pickNext(NR, NNR, NP, NNP, flags);
	}
	if (NP.size() > floor(maxP / 2)) {
		fillNode(NNR, NNP, flags, children);
	}
	else {
		fillNode(NR, NP, flags, children);
	}
	if (parents.empty()) {
		RTree *N = new RTree(dimension, maxP), *NN = new RTree(dimension, maxP);
		N->space = NR;
		N->children = NP;
		NN->space = NNR;
		NN->children = NNP;
		children.clear();
		children.push_back(N);
		children.push_back(NN);
	}
	else {
		RTree *NN = new RTree(dimension, maxP);
		space = NR;
		children = NP;
		NN->space = NNR;
		NN->children = NNP;
		parents.top()->children.push_back(NN);
		if (parents.top()->children.size() > maxP)
			parents.top()->splitBranch(parents);
	}
}

void RTree::pickSeeds(rectangle &NR, rectangle &NNR, vector<Point> &NP, vector<Point> &NNP, vector<bool> &flags) {
	float maxD = 0, temp;
	int i1, i2;
	for (size_t i = 0; i < points.size(); ++i) {
		for (size_t j = i + 1; j < points.size(); ++j) {
			temp = rectangle(points[i], points[j]).area();
			if (temp > maxD) {
				maxD = temp;
				i1 = i;
				i2 = j;
			}
		}
	}
	NR = rectangle(points[i1]);
	NNR = rectangle(points[i2]);
	NP.push_back(points[i1]);
	NNP.push_back(points[i2]);
	flags[i1] = false;
	flags[i2] = false;
}

void fillNode(rectangle &R, vector<Point> &P, vector<bool> flags, vector<Point> points) {
	for (size_t i = 0, top = points.size(); i < top; ++i) {
		if (flags[i]) {
			P.push_back(points[i]);
			R = rectangle(R, points[i]);
		}
	}
}

void RTree::pickNext(rectangle &NR, rectangle &NNR, vector<Point> &NP, vector<Point> &NNP, vector<bool> &flags) {
	float maxD = 0, temp;
	int i1;
	for (size_t i = 0; i < points.size(); ++i) {
		if (flags[i]) {
			temp = abs(NR.enlargement(points[i]) - NNR.enlargement(points[i]));
			if (temp > maxD) {
				maxD = temp;
				i1 = i;
			}
		}
	}
	if (NR.enlargement(points[i1]) < NNR.enlargement(points[i1])) {
		NP.push_back(points[i1]);
		NR = rectangle(NR, points[i1]);
	}
	else {
		NNP.push_back(points[i1]);
		NNR = rectangle(NNR, points[i1]);
	}
	flags[i1] = false;
}

void RTree::split(stack<RTree*> parents) {
	vector<bool> flags(points.size(), true);
	rectangle NR, NNR;
	vector<Point> NP, NNP;
	pickSeeds(NR, NNR, NP, NNP, flags);
	while (NP.size() <= floor(maxP / 2) && NNP.size() <= floor(maxP / 2)) {
		pickNext(NR, NNR, NP, NNP, flags);
	}
	if (NP.size() > floor(maxP / 2)) {
		fillNode(NNR, NNP, flags, points);
	}
	else {
		fillNode(NR, NP, flags, points);
	}
	if (parents.empty()) {
		RTree *N = new RTree(dimension,maxP), *NN = new RTree(dimension,maxP);
		N->space = NR;
		N->points = NP;
		NN->space = NNR;
		NN->points = NNP;
		points.clear();
		children.push_back(N);
		children.push_back(NN);
	}
	else {
		RTree *NN = new RTree(dimension, maxP);
		space = NR;
		points = NP;
		NN->space = NNR;
		NN->points = NNP;
		parents.top()->children.push_back(NN);
		if (parents.top()->children.size() > maxP)
			parents.top()->splitBranch(parents);
	}
}

void RTree::insertPoint(Point E, RTree *p) {
	RTree *root = p;
	stack<RTree*> parents;
	if (!chooseLeaf(E, p, parents)) {
		p->points.push_back(E);
		if (p->points.size() == 1)
			p->space = rectangle(E);
		else
			p->space = rectangle(p->space, E);
		if (p->points.size() > maxP) {
			p->split(parents);
		}
		while (!parents.empty()) {
			parents.top()->space = rectangle(parents.top()->space,E);
			parents.pop();
		}
	}
}

vector<Point> RTree::getPoints() {
	vector<Point> puntos;
	if (children.size() == 0) {
		puntos.insert(puntos.end(), points.begin(), points.end());
	}
	else {
		vector<Point> aux;
		for (size_t i = 0; i < children.size(); ++i) {
			aux = children[i]->getPoints();
			puntos.insert(puntos.end(), aux.begin(), aux.end());
		}
	}
	return puntos;
}

vector<Line> RTree::getLines() {
	vector<Line> lines;
	if (children.size() > 0) {
		lines = space.getFigure();
	}
	for (size_t i = 0; i < children.size(); ++i) {
		vector<Line> aux = children[i]->getLines();
		lines.insert(lines.end(), aux.begin(), aux.end());
	}
	return lines;
}

vector<Line> RTree::getLines2() {
	vector<Line> lines;
	if (children.size() == 0) {
		lines = space.getFigure();
	}
	for (size_t i = 0; i < children.size(); ++i) {
		vector<Line> aux = children[i]->getLines2();
		lines.insert(lines.end(), aux.begin(), aux.end());
	}
	return lines;
}

bool RTree::onCircle(Point punto, float distancia) {
	bool result = true;
	vector<vector<coordenada> > ps;
	vector<coordenada> p1 = space.getMins();
	vector<coordenada> p2 = space.getMaxs();
	ps.push_back(p1);
	ps.push_back(p2);
	for (int i = 0; i < 2; ++i) {
		for (int j = 0; j < 2; ++j) {
			for (int k = 0; k < 2; ++k) {
				vector<coordenada> val = { ps[i][0], ps[j][1], ps[k][2] };
				Point p(val);
				result = result && (euclidean(punto, p) <= distancia);
			}
		}
	}
	return result;
}

bool RTree::corner(Point punto, float distancia) {
	bool result = false;
	vector<vector<coordenada> > ps;
	vector<coordenada> p1 = space.getMins();
	vector<coordenada> p2 = space.getMaxs();
	ps.push_back(p1);
	ps.push_back(p2);
	for (int i = 0; i < 2; ++i) {
		for (int j = 0; j < 2; ++j) {
			for (int k = 0; k < 2; ++k) {
				vector<coordenada> val = { ps[i][0], ps[j][1], ps[k][2] };
				Point p(val);
				result = result || (euclidean(punto, p) <= distancia);
			}
		}
	}
	return result;
}

bool RTree::inCube(Point punto) {
	bool result = true;
	vector<vector<coordenada> > ps;
	vector<coordenada> p1 = space.getMins();
	vector<coordenada> p2 = space.getMaxs();
	vector<coordenada> p = { punto.val[0],punto.val[1],punto.val[2] };
	ps.push_back(p1);
	ps.push_back(p2);
	for (int i = 0; i < ps[0].size(); ++i) {
		result = result && (p[i] > p1[i]);
		result = result && (p[i] < p2[i]);
	}
	return result;
}

bool RTree::outCube(Point punto, float distancia) {
	bool result = false;
	vector<vector<coordenada> > ps;
	vector<coordenada> p1 = space.getMins();
	vector<coordenada> p2 = space.getMaxs();
	vector<coordenada> p = { punto.val[0],punto.val[1],punto.val[2] };
	ps.push_back(p1);
	ps.push_back(p2);
	for (int i = 0; i < ps[0].size(); ++i) {
		result = result || (p[i]<p1[i] && (p[i] + distancia)>p1[i]);
		result = result || (p[i] > p2[i] && (p[i] - distancia) < p2[i]);
	}
	return result;
}

bool RTree::inRegion(Point punto, float distancia) {
	bool result = false;
	if (corner(punto, distancia)) {
		result = true;
	}
	else {
		if (inCube(punto)) {
			result = true;
		}
		else {
			if (outCube(punto, distancia)) {
				result = true;
			}
		}
	}
	return result;
}

vector<Point> RTree::cercanos(Point centro, float radio) {
	vector<Point> puntos;
	if (onCircle(centro, radio)) {
		return getPoints();
	}
	else if (inRegion(centro, radio)) {
		if (children.size()==0) {
			for (size_t i = 0; i < points.size(); ++i) {
				if (euclidean(points[i], centro) <= radio)
					puntos.push_back(points[i]);
			}
		}
		else {
			vector<Point> aux;
			for (int i = 0; i < children.size(); ++i) {
				aux = children[i]->cercanos(centro, radio);
				puntos.insert(puntos.end(), aux.begin(), aux.end());
			}
		}
	}
	return puntos;
}

RTree::~RTree()
{
	//dtor
}
