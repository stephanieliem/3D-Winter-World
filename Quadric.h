#include <iostream>
#include "Matrix.h"
#include "GL/glut.h"
#include <stack>
#include <math.h>
using namespace std;

class Quadric {
public:
	Vector titik[100][100];
	//double PI = 3.14159265358979323846;

	void drawquadric() {
		int i = 0, j = 0;
		for (i = 0; i < 99; i++)
		{
			j = 0;
			for (j = 0; j < 49; j++)
			{
				glBegin(GL_TRIANGLES);
				glVertex3f(titik[i][j].getX(), titik[i][j].getY(), titik[i][j].getZ());
				glVertex3f(titik[i + 1][j].getX(), titik[i + 1][j].getY(), titik[i + 1][j].getZ());
				glVertex3f(titik[i + 1][j + 1].getX(), titik[i + 1][j + 1].getY(), titik[i + 1][j + 1].getZ());
				glEnd();
				glBegin(GL_TRIANGLES);
				glVertex3f(titik[i][j].getX(), titik[i][j].getY(), titik[i][j].getZ());
				glVertex3f(titik[i + 1][j + 1].getX(), titik[i + 1][j + 1].getY(), titik[i + 1][j + 1].getZ());
				glVertex3f(titik[i][j + 1].getX(), titik[i][j + 1].getY(), titik[i][j + 1].getZ());
				glEnd();
			}
			/*glBegin(GL_QUADS);
			glVertex3f(titik[i][49].getX(), titik[i][49].getY(), titik[i][49].getZ());
			glVertex3f(titik[i + 1][49].getX(), titik[i][0].getY(), titik[i][0].getZ());
			glVertex3f(titik[i][49].getX(), titik[i][49].getY(), titik[i][49].getZ());
			glVertex3f(titik[i][0].getX(), titik[i][0].getY(), titik[i][0].getZ());
			glEnd();*/
		}

		//draw sumbu X(atas ke bawah)
		i = 0, j = 0;
		for (j = 0; j < 49; j++)
		{
			i = 0;
			for (i = 0; i < 99; i++)
			{
				glBegin(GL_QUADS);
				glVertex3f(titik[i][j].getX(), titik[i][j].getY(), titik[i][j].getZ());
				glVertex3f(titik[i + 1][j].getX(), titik[i + 1][j].getY(), titik[i + 1][j].getZ());
				glVertex3f(titik[i + 1][j + 1].getX(), titik[i + 1][j + 1].getY(), titik[i + 1][j + 1].getZ());
				glVertex3f(titik[i][j + 1].getX(), titik[i][j + 1].getY(), titik[i][j + 1].getZ());
				glEnd();
			}
			/*glBegin(GL_QUADS);
			glVertex3f(titik[99][j].getX(), titik[99][j].getY(), titik[99][j].getZ());
			glVertex3f(titik[0][j].getX(), titik[0][j].getY(), titik[0][j].getZ());
			glEnd();*/
		}
		//draw sumbu Y(kanan kiri)
	}

	void Elips(int radius) { 
		int ii=0, jj;
		for (double i=-phi; i<=phi; i+=phi/50.0){
			jj=0;
			for (double j=-phi/2; j<=phi/2; j+=phi/50.0){
				titik[ii][jj].setX(radius*cos(i)*cos(j));
				titik[ii][jj].setY(radius*sin(i)*cos(j));
				titik[ii][jj].setZ(radius*sin(j));
				jj++;
			}
			ii++;
		}
	}
	void HalfElips(int radius, int radius2, int radius3) { 
		int ii=0, jj;
		for (double i=-phi; i<=phi; i+=phi/50.0){
			jj=0;
			for (double j=-phi/2; j<=phi/18; j+=phi/50.0){
				titik[ii][jj].setX(radius*cos(i)*cos(j));
				titik[ii][jj].setY(radius2*sin(i)*cos(j));
				titik[ii][jj].setZ(radius3*sin(j));
				jj++;
			}
			ii++;
		}
	}


	void Oval(int radius, int radius2, int radius3) { 
		int ii=0, jj;
		for (double i=-phi; i<=phi; i+=phi/50.0){
			jj=0;
			for (double j=-phi/2; j<=phi/2; j+=phi/50.0){
				titik[ii][jj].setX(radius*cos(i)*cos(j));
				titik[ii][jj].setY(radius2*sin(i)*cos(j));
				titik[ii][jj].setZ(radius3*sin(j));
				jj++;
			}
			ii++;
		}
	}

	//void Tabung(double radius) {
	//	int ii=0, jj;
	//	for (double i=-phi; i<=phi; i+=phi/50.0){
	//		jj=0;
	//		for (double j=-phi/2; j<=phi/2; j+=phi/50.0){
	//			if ()
	//			jj++;
	//		}
	//		ii++;
	//	}
	//}

	void Body(int radius) { 
		int ii=0, jj;
		for (double i=-phi; i<=phi; i+=phi/50.0){
			jj=0;
			for (double j=-phi/2; j<=phi/2; j+=phi/50.0){
				/*if (j <= phi/10) {
				titik[ii][jj].setX(radius*cos(i)*cos(j));
				titik[ii][jj].setY(radius*0.5*sin(i)*cos(j));
				titik[ii][jj].setZ(radius*0.8*sin(j));
				}
				else if (j > phi/10) {
				titik[ii][jj].setX(0);
				titik[ii][jj].setY(0);
				titik[ii][jj].setZ(0);
				}*/
				/*else if (j <= -phi/3) {
				titik[ii][jj].setX(0);
				titik[ii][jj].setY(0);
				titik[ii][jj].setZ(-radius*1.5*sin(phi/4.5));
				}*/
				if (j > -phi/4) {
					titik[ii][jj].setX(radius*cos(i)*cos(j));
					titik[ii][jj].setY(radius*sin(i)*cos(j));
					titik[ii][jj].setZ(radius*0.6*sin(j));
				}
				else {
					titik[ii][jj].setX(0);
					titik[ii][jj].setY(0);
					titik[ii][jj].setZ(-radius*1.2*sin(phi/4.5));
				}
				jj++;
			}
			ii++;
		}
	}

	void Wind(double r, double p) {
		int ii = 0, jj = 0;

		double bagi = r / 49.0, tinggi = p / 50.0, sinus = 60;
		for (double i=-phi; i<=phi; i+=phi/50.0){
			jj=0;
			double x = r, t = 0;
			for (double j=-phi/2; j<=phi/2; j+=phi/50.0){
				titik[ii][jj].setX(x * cos(i) * sin(j * 10));
				titik[ii][jj].setY(x * sin(i) * sin(j * 10));
				titik[ii][jj].setZ(sinus * cos((j + phi/2.0) * phi / 4.0) - sinus);
				titik[ii][jj].setW(t);
				jj++;
				t += tinggi;
				x -= bagi;
			}
			ii++;
		}
	}

	//void Torus(int a, int c) {
	//	int i, lineAmount = 100;
	//	double radius = 30, x = 5, y = 5;

	//	GLfloat twicePi = 2.0f * PI;
	//	glColor3f(1, 1, 1);
	//	gl
	//}

	/*Matrix getTransform() {
	Matrix temp;
	while (tempTransform.empty() == false) {
	temp = temp * tempTransform.top();
	tempTransform.pop();
	}
	return temp;
	}*/

	void transform(Matrix mat) {
		for (int i=0; i<100; i++) {
			for (int j=0; j<50; j++) {
				titik[i][j] = mat * titik[i][j];
			}
		}
	}
};