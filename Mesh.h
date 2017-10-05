#include <iostream>
#include <fstream>
#include "GL\glut.h"
#include "Camera.h"
#include <windows.h>
#include <string>
#include <Vector>
using namespace std;

GLuint *texture = new GLuint[100];
int texture_index = 0;

class Material {
public:
	int textureID;
	Material(const char * filename) {
		LoadTextureFromFile(filename);
	}
	void setzero()
	{
		texture_index=0;
	}
    void LoadTextureFromFile(const char * filename) {
        textureID = texture_index;
        texture_index++;
        char header[54];
        int vPos;
        int width,height;
        int imageSize;
        char *v;
        
        FILE *file = fopen(filename, "rb");
        
        if (!file) {
            printf("Image could not be opened\n");
            return;
        }
        if (fread(header,1,54,file) != 54) {
            printf("File must be BMP");
        }
        if (header[0] != 'B' || header[1] != 'M') {
            printf("File must be BMP");
            return;
        }
        vPos = *(int*)&(header[0x0A]);
        imageSize = *(int*)&(header[0x22]);
        width = *(int*)&(header[0x12]);
        height = *(int*)&(header[0x16]);
        
        if (imageSize == 0)    imageSize = width*height * 3; // 3 : one byte for each Red, Green and Blue component
        if (vPos == 0)      vPos = 54; // seek to 1 bit after header
        
        // Create a buffer
        v = new char[imageSize];
        
        // Read the actual v from the file into the buffer
        fread(v, 1, imageSize, file);
        
        fclose(file);
        
        // Create one OpenGL texture
        glGenTextures(1, &texture[textureID]);
        
        // "Bind" the newly created texture : all future texture functions will modify this texture
        glBindTexture(GL_TEXTURE_2D, texture[textureID]);
        
        // Give the image to OpenGL
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, v);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glBindTexture(GL_TEXTURE_2D, texture[0]);
    }
};

class Face {
public:
	int v[3],vt[3];
	void setFace(int a, int b, int c) {
		v[0] = a - 1;
		v[1] = b - 1;
		v[2] = c - 1;
	}
	void setFaceTexture(int a,int b,int c) {
		vt[0] = a - 1;
		vt[1] = b - 1;
		vt[2] = c - 1;
	}
};

float Max(float a, float b)
{
	return a>b?a:b;
}

class Mesh {
public:
	Material *material;
	vector<Vector> vertices, v_textures, vcopy;
	vector<Face> face;
	Vector dummy2;
	Face dummy;
	int count;
	float edge, colliderSize;
	Color color;
	Matrix trans, mposition;
	Vector position;

	Mesh (char *filename, bool facess, Material *mat=NULL, Vector pos= Vector(1,1,1)) {
		material=mat;
		position = pos;
		trans.setIdentity();
		char temp[100];
		int countv = 0, countf = 0;
		count = 0;
		fstream fio;
		fio.open(filename, ios::in);
		while (!fio.eof() && fio.good()) {
			fio >> temp;
			if (strcmp(temp, "v") == 0) {
				double x, y, z;
				fio >> x >> y >> z;
				dummy2.setAll(x, y, z, 1);
				vertices.push_back(dummy2);
				vcopy.push_back(dummy2);
			} else if (strcmp(temp,"vt") == 0) {
				double x, y, z;
				fio >> x >> y;
				dummy2.setAll(x, y, 0, 1);
				v_textures.push_back(dummy2);
			} else if (strcmp(temp, "f") == 0) {
				if (facess) {
					int f1, f2, f3;
					int vt[3];
					char slash[6];
					fio >>f1 >>slash[0]>>vt[0]>>f2>>slash[1]>>vt[1]>>f3>>slash[2]>>vt[2];
					dummy.setFace(f1, f2, f3);
					dummy.setFaceTexture(vt[0],vt[1],vt[2]);
					count++;
					face.push_back(dummy);
				}
				else {
					int f1, f2, f3;
					fio >> f1 >> f2 >> f3;
					dummy.setFace(f1, f2, f3);
					count++;
					face.push_back(dummy);
				}
			}
		}
		fio.close();
	}

	
	void setMat(Material *m) {
		material = m;
	}
	void SetColor(float r, float g, float b) {
		color.SetRGB(r, g, b);
	}
	Color GetAmbient(Lamp myLamp) {
		return myLamp.GetColor() * myLamp.GetIA();
	}

	Color GetDiffuse(Lamp myLamp, Face f) {
		Vector normal;
		Vector p1 = vertices[f.v[0]];
		Vector p2 = vertices[f.v[1]];
		Vector p3 = vertices[f.v[2]];

		normal = (p2 - p1) * (p3 - p1);
		normal = normal.Normalize();

		Vector point = myLamp.GetPos() - p1;
		point = point.Normalize();

		float pengali = normal ^ point;

		return myLamp.GetColor() * myLamp.GetIS() * max(pengali, 0);
	}

	void se(Vector a) {
		position = a;
		for(int i=0; i<vcopy.size(); i++) {
			vcopy[i] = a + vcopy[i];
		}
	}

	void addTransform(Matrix other) {
		trans = other * trans;
	}

	Color GetSpecular(Lamp myLamp, Face f, Vector myEye){
		Vector p1 = vertices[f.v[0]];
		Vector p2 = vertices[f.v[1]];
		Vector p3 = vertices[f.v[2]];

		Vector normal;
		normal = (p2 - p1) * (p3 - p1);
		normal = normal.Normalize();

		Vector L = myLamp.GetPos() - p1;
		L = L.Normalize();

		Vector R = L*(-1);

		R = R + normal * 3 * max(L^normal, 0);
		Vector V = myEye - p1;
		V = V.Normalize();

		return myLamp.GetColor() * myLamp.GetIS() * max(V^R, 0);
	}

	double Absolute(double x){
		if (x < 0)
			return x * -1.0F;
		return x;
	}

	void setMatrixPos(Matrix Pos) {
		mposition = Pos;
	}

	Vector getPosition() {
		return trans * Vector(1,1,1);
	}

	void countP(Matrix a) {
		position = a *position;
	}

	void k(Camera myCamera) {
		Matrix t = myCamera.GetViewMatrix();
		/*for(int i=0; i<vcopy.size(); i++)
		{
			vcopy[i] = t * vcopy[i];
		}*/
		Vector temp = t * position;

		position.z = temp.z;
	}
	void Draw (Lamp myLamp, Camera myCamera, Matrix transform = Matrix()) {
		if (material != NULL) {
			glEnable(GL_TEXTURE_2D);
            glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			//position = myCamera.GetViewMatrix() * position;
            glBindTexture(GL_TEXTURE_2D, texture[material->textureID]);
			for (int i=0; i<face.size(); i++) {
				Color myColor;
				myLamp.SetColor(1, 1, 1);
				myColor = color + GetAmbient(myLamp) + GetDiffuse(myLamp, face[i]) + GetSpecular(myLamp, face[i], myCamera.VPN);
				glColor3f(myColor.GetRed(), myColor.GetGreen(), myColor.GetBlue());
				//glTexCoord2d(v_textures[face[i].vt[0]].x,v_textures[face[i].vt[0]].y);
				//position = trans*position;
				
				glBegin(GL_TRIANGLES);
				for(int j=0; j<3; j++)
				{
					glTexCoord2f(v_textures[face[i].vt[j]].x,v_textures[face[i].vt[j]].y);
					Vector temp = vertices[face[i].v[j]];
					temp = trans * transform * temp;
					temp = myCamera.TransformtoCameraView(temp);
					glVertex3d(temp.x, temp.y, temp.z);	
				}
				glEnd();
			}
			glDisable(GL_TEXTURE_2D);
		} else {
			for (int i=0; i<count; i++) {
				Color myColor;
				myLamp.SetColor(1, 1, 1);
				myColor = color + GetAmbient(myLamp) + GetDiffuse(myLamp, face[i]) + GetSpecular(myLamp, face[i], myCamera.VPN);
				glColor3f(myColor.GetRed(), myColor.GetGreen(), myColor.GetBlue());
			//	position = trans*position;
				glBegin(GL_TRIANGLES);
				for(int j=0; j<3; j++)
				{
					Vector temp = vertices[face[i].v[j]];
					temp = trans * transform * temp;
					temp = myCamera.TransformtoCameraView(temp);
					glVertex3d(temp.x, temp.y, temp.z);	
				}
				glEnd();
			}
		}
	}
};