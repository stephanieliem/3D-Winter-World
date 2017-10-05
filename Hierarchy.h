#include <iostream>
#include "Quadric.h"
using namespace std;

class Hierarchy {
public:
	Quadric base;
	int id_parent;
	Matrix pos, trans;
	Hierarchy() {
		id_parent = -1;
	}
	void setParent(int parent) {
		id_parent = parent;
	}
	void setQuadric(int i, int radius, int radius2, int radius3) {
		if (i == 1) {
			base.Body(radius);
		}
		else if (i == 2) {
			base.Elips(radius);
		}
		else if (i == 3) {
			base.Oval(radius, radius2, radius3);
		}
		else if (i == 4) {
			base.HalfElips(radius, radius2, radius3);
		}
		else if (i == 5) {
			base.Wind(radius, radius2);
		}
	}
	void setPos(Matrix _pos) {
		pos = pos * _pos;
	}
	void addTransform(Matrix _trans) {
		trans = trans * _trans;
	}
	Matrix getPos() {
		return pos;
	}

	Matrix getTransform(Hierarchy *other) {
		Matrix temp;
		if(id_parent != -1){
			temp = other[id_parent].getTransform(other);}
		temp = temp * pos * trans;
		return temp;
	}

	void draw(double r, double g, double b, Hierarchy *other, int x) {
		if (x == 18 || x == 21 || x == 22) {
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glColor4f(0,0,0,0);
		} 
		else {
			glColor3f(r,g,b);
		}
		Vector dummy[100][100];

		Matrix temp = getTransform(other);

		for (int i=0; i<100; i++) {
			for (int j=0; j<100; j++) {
				dummy[i][j] = base.titik[i][j];
				dummy[i][j] = temp * dummy[i][j];
			}
		}

		int i = 0, j = 0;
		for (i = 0; i < 99; i++)
		{
			for (j = 0; j < 49; j++)
			{
				glBegin(GL_TRIANGLES);
				glVertex3f(dummy[i][j].getX(), dummy[i][j].getY(), dummy[i][j].getZ());
				glVertex3f(dummy[i + 1][j].getX(), dummy[i + 1][j].getY(), dummy[i + 1][j].getZ());
				glVertex3f(dummy[i + 1][j + 1].getX(), dummy[i + 1][j + 1].getY(), dummy[i + 1][j + 1].getZ());
				glEnd();

				glBegin(GL_TRIANGLES);
				glVertex3f(dummy[i][j].getX(), dummy[i][j].getY(), dummy[i][j].getZ());
				glVertex3f(dummy[i + 1][j + 1].getX(), dummy[i + 1][j + 1].getY(), dummy[i + 1][j + 1].getZ());
				glVertex3f(dummy[i][j + 1].getX(), dummy[i][j + 1].getY(), dummy[i][j + 1].getZ());
				glEnd();
			}
			glBegin(GL_TRIANGLES);
			glVertex3f(dummy[i][49].getX(), dummy[i][49].getY(), dummy[i][49].getZ());
			glVertex3f(dummy[i + 1][49].getX(), dummy[i + 1][49].getY(), dummy[i + 1][49].getZ());
			glVertex3f(dummy[i + 1][0].getX(), dummy[i + 1][0].getY(), dummy[i + 1][0].getZ());
			glEnd();

			glBegin(GL_TRIANGLES);
			glVertex3f(dummy[i][49].getX(), dummy[i][49].getY(), dummy[i][49].getZ());
			glVertex3f(dummy[i + 1][0].getX(), dummy[i + 1][0].getY(), dummy[i + 1][0].getZ());
			glVertex3f(dummy[i][0].getX(), dummy[i][0].getY(), dummy[i][0].getZ());
			glEnd();
		}
		//draw sumbu X(atas ke bawah)
		for (j = 0; j < 49; j++)
		{
			for (i = 0; i < 99; i++)
			{
				glBegin(GL_TRIANGLES);
				glVertex3f(dummy[i][j].getX(), dummy[i][j].getY(), dummy[i][j].getZ());
				glVertex3f(dummy[i + 1][j].getX(), dummy[i + 1][j].getY(), dummy[i + 1][j].getZ());
				glVertex3f(dummy[i + 1][j + 1].getX(), dummy[i + 1][j + 1].getY(), dummy[i + 1][j + 1].getZ());
				glEnd();

				glBegin(GL_TRIANGLES);
				glVertex3f(dummy[i][j].getX(), dummy[i][j].getY(), dummy[i][j].getZ());
				glVertex3f(dummy[i + 1][j + 1].getX(), dummy[i + 1][j + 1].getY(), dummy[i + 1][j + 1].getZ());
				glVertex3f(dummy[i][j + 1].getX(), dummy[i][j + 1].getY(), dummy[i][j + 1].getZ());
				glEnd();
			}
			glBegin(GL_TRIANGLES);
			glVertex3f(dummy[99][j].getX(), dummy[99][j].getY(), dummy[99][j].getZ());
			glVertex3f(dummy[0][j].getX(), dummy[0][j].getY(), dummy[0][j].getZ());
			glVertex3f(dummy[0][j + 1].getX(), dummy[0][j + 1].getY(), dummy[0][j + 1].getZ());
			glEnd();

			glBegin(GL_TRIANGLES);
			glVertex3f(dummy[99][j].getX(), dummy[99][j].getY(), dummy[99][j].getZ());
			glVertex3f(dummy[0][j + 1].getX(), dummy[0][j + 1].getY(), dummy[0][j + 1].getZ());
			glVertex3f(dummy[99][j + 1].getX(), dummy[99][j + 1].getY(), dummy[99][j + 1].getZ());
			glEnd();
		}
		//draw sumbu Y(kanan kiri)
		if (x == 18 || x == 21 || x == 22) {
			glDisable(GL_BLEND);
		} 
	}
};