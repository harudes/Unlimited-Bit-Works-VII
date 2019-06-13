#include "rectangle.h"

rectangle::rectangle() {

}

rectangle::rectangle(Point p) {
	mins = p;
	maxs = p;
}

rectangle::rectangle(Point u, Point v) {
	if (u.val.size() == v.val.size()) {
		vector<coordenada> a(u.val.size()), b(u.val.size());
		for (size_t i = 0; i < u.val.size(); ++i) {
			if (u.val[i] < v.val[i]) {
				a[i] = u.val[i];
				b[i] = v.val[i];
			}
			else {
				a[i] = v.val[i];
				b[i] = u.val[i];
			}
		}
		mins.val = a;
		maxs.val = b;
	}
}

rectangle::rectangle(rectangle a, rectangle b) {
	if (a.mins.val.size() == b.mins.val.size()) {
		vector<coordenada> u(a.mins.val.size()), v(a.mins.val.size());
		for (size_t i = 0; i < a.mins.val.size(); ++i) {
			if (a.mins.val[i] < b.mins.val[i])
				u[i] = a.mins.val[i];
			else
				u[i] = b.mins.val[i];
			if (a.maxs.val[i] > b.maxs.val[i])
				v[i] = a.maxs.val[i];
			else
				v[i] = b.maxs.val[i];
		}
		mins.val = u;
		maxs.val = v;
	}
}

rectangle::rectangle(rectangle r, Point p) {
	mins = r.mins;
	maxs = r.maxs;
	if (r.mins.val.size() == p.val.size()) {
		for (size_t i = 0; i < p.val.size(); ++i) {
			if (p.val[i] < r.mins.val[i])
				mins.val[i] = p.val[i];
			if (p.val[i] > r.maxs.val[i])
				maxs.val[i] = p.val[i];
		}
	}
}

float rectangle::enlargement(Point p) {
	rectangle newR = rectangle(rectangle(mins, maxs), p);
	return newR.area() - area();
}

float rectangle::enlargement(rectangle r) {
	rectangle newR = rectangle(rectangle(mins, maxs), r);
	return newR.area() - area();
}

float rectangle::area() {
	float result = 1;
	for (size_t i = 0; i < mins.val.size(); ++i) {
		result *= maxs.val[i] - mins.val[i];
	}
	return result;
}

vector<Line> rectangle::getFigure() {
	vector<Line> lines;
	vector<coordenada> temp;
	temp.push_back(maxs.val[0]); temp.push_back(maxs.val[1]); temp.push_back(maxs.val[2]); Point a(temp); temp.clear();
	temp.push_back(maxs.val[0]); temp.push_back(maxs.val[1]); temp.push_back(mins.val[2]); Point b(temp); temp.clear();
	temp.push_back(maxs.val[0]); temp.push_back(mins.val[1]); temp.push_back(maxs.val[2]); Point c(temp); temp.clear();
	temp.push_back(maxs.val[0]); temp.push_back(mins.val[1]); temp.push_back(mins.val[2]); Point d(temp); temp.clear();
	temp.push_back(mins.val[0]); temp.push_back(maxs.val[1]); temp.push_back(maxs.val[2]); Point e(temp); temp.clear();
	temp.push_back(mins.val[0]); temp.push_back(maxs.val[1]); temp.push_back(mins.val[2]); Point f(temp); temp.clear();
	temp.push_back(mins.val[0]); temp.push_back(mins.val[1]); temp.push_back(maxs.val[2]); Point g(temp); temp.clear();
	temp.push_back(mins.val[0]); temp.push_back(mins.val[1]); temp.push_back(mins.val[2]); Point h(temp); temp.clear();
	lines.push_back(Line(a, b));
	lines.push_back(Line(a, c));
	lines.push_back(Line(a, e));
	lines.push_back(Line(b, d));
	lines.push_back(Line(b, f));
	lines.push_back(Line(c, g));
	lines.push_back(Line(c, d));
	lines.push_back(Line(d, h));
	lines.push_back(Line(e, f));
	lines.push_back(Line(e, g));
	lines.push_back(Line(f, h));
	lines.push_back(Line(g, h));
	return lines;
}

vector<coordenada> rectangle::getMins() {
	return mins.val;
}

vector<coordenada> rectangle::getMaxs() {
	return maxs.val;
}

rectangle::~rectangle()
{
	//dtor
}
