#include "minCircle.h"


float dist(Point a, Point b){
	float x2=(a.x-b.x)*(a.x-b.x);
	float y2=(a.y-b.y)*(a.y-b.y);
	return sqrt(x2+y2);
}

Circle from2points(Point a,Point b){
	float x=(a.x+b.x)/2;
	float y=(a.y+b.y)/2;
	float r=dist(a,b)/2;
	return Circle(Point(x,y),r);
}


Circle from3Points(Point a, Point b, Point c){
	// find the circumcenter of the triangle a,b,c
	// find the circumcenter of the triangle a,b,c
	
	Point mAB((a.x+b.x)/2 , (a.y+b.y)/2); // mid point of line AB
	float slopAB = (b.y - a.y) / (b.x - a.x); // the slop of AB
	float pSlopAB = - 1/slopAB; // the perpendicular slop of AB
	// pSlop equation is:
	// y - mAB.y = pSlopAB * (x - mAB.x) ==> y = pSlopAB * (x - mAB.x) + mAB.y
	
	Point mBC((b.x+c.x)/2 , (b.y+c.y)/2); // mid point of line BC
	float slopBC = (c.y - b.y) / (c.x - b.x); // the slop of BC
	float pSlopBC = - 1/slopBC; // the perpendicular slop of BC
	// pSlop equation is:
	// y - mBC.y = pSlopBC * (x - mBC.x) ==> y = pSlopBC * (x - mBC.x) + mBC.y
	
	/*
	pSlopAB * (x - mAB.x) + mAB.y = pSlopBC * (x - mBC.x) + mBC.y
	pSlopAB*x - pSlopAB*mAB.x + mAB.y = pSlopBC*x - pSlopBC*mBC.x + mBC.y
	
	x*(pSlopAB - pSlopBC) = - pSlopBC*mBC.x + mBC.y + pSlopAB*mAB.x - mAB.y
	x = (- pSlopBC*mBC.x + mBC.y + pSlopAB*mAB.x - mAB.y) / (pSlopAB - pSlopBC);
	
	*/
	
	float x = (- pSlopBC*mBC.x + mBC.y + pSlopAB*mAB.x - mAB.y) / (pSlopAB - pSlopBC);
	float y = pSlopAB * (x - mAB.x) + mAB.y;
	Point center(x,y);
	float R=dist(center,a);
	
	return Circle(center,R);
}

Circle trivial(vector<Point>& P){
	if(P.size()==0)
		return Circle(Point(0,0),0);
	else if(P.size()==1)
		return Circle(P[0],0);
	else if (P.size()==2)
		return from2points(P[0],P[1]);

	// maybe 2 of the points define a small circle that contains the 3ed point
	Circle c=from2points(P[0],P[1]);
	if(dist(P[2],c.center)<=c.radius)
		return c;
	c=from2points(P[0],P[2]);
	if(dist(P[1],c.center)<=c.radius)
		return c;
	c=from2points(P[1],P[2]);
	if(dist(P[0],c.center)<=c.radius)
		return c;
	// else find the unique circle from 3 points
	return from3Points(P[0],P[1],P[2]);
}


Circle welzl(Point** P,vector<Point> R, size_t n){
	if(n==0 || R.size()==3){
		return trivial(R);
	}

	// remove random point p
	// swap is more efficient than remove
	//srand (time(NULL));
	int i=rand()%n;
	Point p(P[i]->x,P[i]->y);
	swap(P[i],P[n-1]);

	Circle c=welzl(P,R,n-1);

	if(dist(p,c.center)<=c.radius)
		return c;

	R.push_back(p);

	return welzl(P,R,n-1);
}

Circle findMinCircle(Point** points,size_t size){
	return welzl(points,{},size);
}

Circle findMinCircle(const float *x, const float *y, size_t size) {
	Point** p = new Point*[size];
	for (size_t i = 0; i < size; i++) {
		Point *p1 = new Point(x[i], y[i]);
		p[i] = p1;
	}
	Circle c = findMinCircle(p, size);
	for (size_t i = 0; i < size; i++) 
		delete p[i];
	delete p;
	return c;
}

bool isAnomalousCircle(float x, float y, Circle c) {
	float t1 =  fabs(sqrt(pow(x - c.center.x, 2.0) + pow(y - c.center.y, 2.0)));
	return (t1 > c.radius);
}

/*
Circle findMinCircle(vector<float> x, vector<float> y, size_t size) {
	Point** p = new Point*[size];
	for (size_t i = 0; i < size; i++) {
		Point *p1 = new Point(x[i], y[i]);
		p[i] = p1;
	}
	Circle c = findMinCircle(p, size);
	for (size_t i = 0; i < size; i++) 
		delete p[i];
	delete p;
	return c;
}

*/
