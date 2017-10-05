#include <iostream>
#include <math.h>
#define PI 3.14159265
using namespace std;
//26415039_KevinNyoto
class Vector {
public:
	double x, y, z, w;
	Vector() {
		x = 0;
		y = 0;
		z = 0;
		w = 1;
	}
	Vector(double _x, double _y, double _z){
		x = _x;
		y = _y;
		z = _z;
	}
	double getX(){
		return x;
	}
	double getY(){
		return y;
	}
	double getZ(){
		return z;
	}
	double getW(){
		return w;
	}
	void setX(double _x){
		x = _x;
	}
	void setY(double _y){
		y = _y;
	}
	void setZ(double _z){
		z = _z;
	}
	void setW(double _w){
		w = _w;
	}
	void setAll(double _x, double _y, double _z, double _w) {
		x = _x;
		y = _y;
		z = _z;
		w = _w;
	}

	Vector Normalize() {
		double magnitude = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
		return Vector(x / magnitude, y / magnitude, z / magnitude);
	}

	double Magnitude() {
		return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
	}

	Vector operator*(double tmp){
		return Vector(x * tmp, y * tmp, z * tmp);
	}

	Vector operator+(Vector tmp){
		return Vector(x + tmp.getX(), y + tmp.getY(), z + tmp.getZ());
	}

	//Dot
	double operator^(Vector tmp){
		return ((x * tmp.getX()) + (y * tmp.getY()) + (z * tmp.getZ()));
	}

	Vector operator-(Vector other) {
		return Vector(x - other.getX(), y - other.getY(), z - other.getZ());
	}

	//Cross
	Vector operator*(Vector tmp){
		return Vector(y * tmp.getZ() - tmp.getY() * z, z * tmp.getX() - tmp.getZ() * x, x * tmp.getY() - tmp.getX() * y);
	}

	void operator=(Vector other){
		x = other.getX();
		y = other.getY();
		z = other.getZ();
	}

	void display() {
		cout << x << " " << y << " " << z << endl;
	}
};