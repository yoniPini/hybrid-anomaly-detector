
#ifndef MINCIRCLE_H_
#define MINCIRCLE_H_

#include <iostream>
#include <vector>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include "anomaly_detection_util.h"
#include <math.h>

using namespace std;


// ------------ DO NOT CHANGE -----------
//class Point{
//public:
//	float x,y;
//	Point(float x,float y):x(x),y(y){}
//};

class Circle{
public:
	Point center;
	float radius;
	Circle(Point c,float r):center(c),radius(r){}
	
	Circle() {
		radius = 0.0;
		center.x = center.y = 0.0;
	}

	Circle(const Circle& c) {
		*this = c;
	}

	Circle& operator=(const Circle& c) {
		if (this != &c) {
			this->center.x = c.center.x;
			this->center.y = c.center.y;
			this->radius = c.radius;
		}
		return *this;
	}

	Circle(const Circle&& c) {
		*this = c;
	}

	Circle& operator=(const Circle&& c) {
		if (this != &c) {
			this->center.x = c.center.x;
			this->center.y = c.center.y;
			this->radius = c.radius;
		}
		return *this;
	}
};
// --------------------------------------



float dist(Point a, Point b);

Circle from2points(Point a,Point b);

Point circumcenter(Point b, Point c);

Circle from3Points(Point a, Point b, Point c);

Circle trivial(vector<Point>& P);

Circle welzl(Point** P,vector<Point> R, size_t n);

Circle findMinCircle(Point** points,size_t size);

Circle findMinCircle(const float *x, const float *y, size_t size);

bool isAnomalousCircle(float x, float y, Circle c);

#endif /* MINCIRCLE_H_ */
