#include "Point.h"

ostream& operator<<(ostream& os, Point p) {
	os << '(';
	for (size_t i = 0; i < p.val.size(); ++i) {
		os << p.val[i];
		if (i < p.val.size() - 1)
			os << ',';
	}
	os << ')';
	return os;
}

bool operator==(Point a, Point b) {
	if (a.val.size() != b.val.size())
		return false;
	bool result = 1;
	for (size_t i = 0; i < a.val.size() && result; ++i) {
		result &= a.val[i] == b.val[i];
	}
	return result;
}

bool operator!=(Point a, Point b) {
	if (a.val.size() != b.val.size())
		return false;
	bool result = 0;
	for (size_t i = 0; i < a.val.size() && !result; ++i) {
		result |= a.val[i] != b.val[i];
	}
	return result;
}

Point::Point(vector<coordenada> u) :val(u) {
	//ctor
}

Point::Point() {
	vector<coordenada> p(3, 0.0);
	val = p;
}

Point::~Point() {
	//dtor
}

float euclidean(Point a, Point b) {
	float result = 0;
	if (a.val.size() == b.val.size()) {
		for (size_t i = 0; i < a.val.size(); ++i) {
			result += pow(a.val[i] - b.val[i], 2);
		}
	}
	return sqrt(result);
}

Line::Line(Point a, Point b) :u(a), v(b) {}
