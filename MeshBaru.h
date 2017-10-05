#include <iostream>
#include <fstream>
#include "GL\glut.h"
#include "Camera.h"
#include <windows.h>
#include <string>
#include <Vector>
using namespace std;

GLuint *texture = new GLuint[100];

class Material {
public:
    int texture_index;
	Material() {
		texture_index = 0;
	}
	int textureID;
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

	int *v,*vt,*vn,*textures;
    int length;
	Face() {}
	Face(int a, int b, int c) {
		length = 3;
        v = new int[length];
        v[0] = a;
        v[1] = b;
        v[2] = c;
        vt = new int[length];
        vn = new int[length];
        textures = new int[3];
	}
	void setFace(int a, int b, int c) {
		v[0] = a - 1;
		v[1] = b - 1;
		v[2] = c - 1;
	}
};

class Mesh {
public:
	vector<Vector> vertices,vt_vertices,vn_vertices;
	vector<Face> faces;
	Vector dummy2;
	Face dummy;
	int count;
	Color color;
	bool isV,isVT,isVN;
	Material *material;

	Mesh (char *filename,bool vs,bool vts, bool vns) {
		int v[3],vt[3],vn[3];
		isV = vs;
        isVT = vts;
        isVN = vns;
		char temp[100];
		int countv = 0, countf = 0;
		count = 0;
		FILE * file = fopen(filename, "r");
        if (file) {
            char *readline;
            /*if ('t' == a) {*/
                while (!feof(file)) {
                    char read_line[100];
                    fscanf(file, "%s", read_line);
                    if (strcmp(read_line, "v") == 0) {
                        Vector v(0,0,0);
                        fscanf(file, "%lf %lf %lf",&v.x,&v.y,&v.z);
                        vertices.push_back(v);
                    }
                    else if (strcmp(read_line, "vt") == 0) {
                        Vector vt(0,0,0);
                        fscanf(file, "%lf %lf %lf",&vt.x,&vt.y,&vt.z);
                        vt_vertices.push_back(vt);
                    }
                    else if (strcmp(read_line, "vn") == 0) {
                        Vector vn(0,0,0);
                        fscanf(file, "%lf %lf %lf",&vn.x,&vn.y,&vn.z);
                        vn_vertices.push_back(vn);
                    }
                    else if (strcmp(read_line, "f") == 0) {
                        
                        if (isV && isVT && isVN) {
                            fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n",&v[0],&vt[0],&vn[0],&v[1],&vt[1],&vn[1],&v[2],&vt[2],&vn[2]);
                            Face face(v[0] - 1,v[1] - 1,v[2] - 1);
                            face.vt[0] = vt[0] - 1;
                            face.vt[1] = vt[1] - 1;
                            face.vt[2] = vt[2] - 1;
                            face.vn[0] = vn[0] - 1;
                            face.vn[1] = vn[1] - 1;
                            face.vn[2] = vn[2] - 1;
                            faces.push_back(face);
                        } else if (isV && isVN) {
                            fscanf(file, "%d//%d %d//%d %d//%d\n",&v[0],&vn[0],&v[1],&vn[1],&v[2],&vn[2]);
                            Face face(v[0] - 1,v[1] - 1,v[2] - 1);
                            face.vn[0] = vn[0] - 1;
                            face.vn[1] = vn[1] - 1;
                            face.vn[2] = vn[2] - 1;
                            faces.push_back(face);
                        } else if (isV && isVT) {
                            fscanf(file, "%d/%d %d/%d %d/%d\n",&v[0],&vt[0],&v[1],&vt[1],&v[2],&vt[2]);
                            Face face(v[0] - 1,v[1] - 1,v[2] - 1);
                            face.vt[0] = vt[0] - 1;
                            face.vt[1] = vt[1] - 1;
                            face.vt[2] = vt[2] - 1;
                            faces.push_back(face);
                        }
                        else if (isV) {
                            fscanf(file, "%d %d %d\n",&v[0],&v[1],&v[2]);
                            Face face(v[0] - 1,v[1] - 1,v[2] -1);
                            faces.push_back(face);
                        }
                    }
                }
            //} 
			/*else if (type == ObjLoader::Quads) {
                while (!feof(file)) {
                    char read_line[100];
                    fscanf(file, "%s", read_line);
                    if (strcmp(read_line, "v") == 0) {
                        Vector v(0,0,0);
                        fscanf(file, "%lf %lf %lf",&v.x,&v.y,&v.z);
                        vertices.push_back(v);
                    }
                    else if (strcmp(read_line, "vt") == 0) {
                        Vector vt(0,0,0);
                        fscanf(file, "%lf %lf %lf",&vt.x,&vt.y,&vt.z);
                        vt_vertices.push_back(vt);
                    }
                    else if (strcmp(read_line, "vn") == 0) {
                        Vector vn(0,0,0);
                        fscanf(file, "%lf %lf %lf",&vn.x,&vn.y,&vn.z);
                        vn_vertices.push_back(vn);
                    }
                    else if (strcmp(read_line, "f") == 0) {
                        int v[4],vt[4],vn[4];
                        if (isV && isVT && isVN) {
                            fscanf(file, "%d/%d/%d/%d %d/%d/%d/%d %d/%d/%d/%d\n",&v[0],&vt[0],&vn[0],&v[1],&vt[1],&vn[1],&v[2],&vt[2],&vn[2],&v[3],&vt[3],&vn[3]);
                            Face face(v[0] - 1,v[1] - 1,v[2] - 1,v[3] - 1);
                            face.vt[0] = vt[0] - 1;
                            face.vt[1] = vt[1] - 1;
                            face.vt[2] = vt[2] - 1;
                            face.vt[3] = vt[3] - 1;

                            face.vn[0] = vn[0] - 1;
                            face.vn[1] = vn[1] - 1;
                            face.vn[2] = vn[2] - 1;
                            face.vn[3] = vn[3] - 1;
                            faces.push_back(face);
                        } else if (isV && isVN) {
                            fscanf(file, "%d//%d %d//%d %d//%d %d//%d\n",&v[0],&vn[0],&v[1],&vn[1],&v[2],&vn[2],&v[3],&vn[3]);
                            Face face(v[0] - 1,v[1] - 1,v[2] - 1,v[3] - 1);
                            face.vn[0] = vn[0] - 1;
                            face.vn[1] = vn[1] - 1;
                            face.vn[2] = vn[2] - 1;
                            face.vn[3] = vn[3] - 1;
                            faces.push_back(face);
                        } else if (isV && isVT) {
                            
                            fscanf(file, "%d/%d/ %d/%d/ %d/%d/ %d/%d/\n",&v[0],&vt[0],&v[1],&vt[1],&v[2],&vt[2],&v[3],&vt[3]);
                            Face face(v[0] - 1,v[1] - 1,v[2] - 1, v[3] - 1);
                            face.vt[0] = vt[0] - 1;
                            face.vt[1] = vt[1] - 1;
                            face.vt[2] = vt[2] - 1;
                            face.vt[3] = vt[3] - 1;
                            faces.push_back(face);
                        }
                        else if (isV) {
                            fscanf(file, "%d %d %d %d\n",&v[0],&v[1],&v[2],&v[3]);
                            Face face(v[0] - 1,v[1] - 1,v[2] -1,v[3] - 1);
                            faces.push_back(face);
                        }
                    }
                }
            }*/
            fclose(file);
        } else {
            cout << "Could not open file obj" <<endl;
        }
	}
	void SetMaterial(Material *mat) {
        material = mat;
    }

	void LoadFromFile(const char *filename, char a) {
        
        
    }
	void SetColor(float r, float g, float b){
		color.SetRGB(r, g, b);
	}
	Color GetAmbient(Lamp myLamp){
		return myLamp.GetColor() * myLamp.GetIA();
	}
	Color GetDiffuse(Lamp myLamp, Face f){
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
	void Draw (Lamp myLamp, Camera myCamera) {

		if(material)
		{
			/*glEnable(GL_TEXTURE_2D);
            glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
            glBindTexture(GL_TEXTURE_2D, texture[material->textureID*/	
				glColor3f(1,0,0);
				glPointSize(10);
				glBegin(GL_POINTS);
				glVertex3d(0,0,0);
				glEnd();
				for (int i=0; i<faces.size(); i++) {
				/*Color myColor;
				myLamp.SetColor(1, 1, 1);
				myColor = color;
				myColor = myColor + GetAmbient(myLamp);
				myColor = myColor + GetDiffuse(myLamp, faces[i]);
				myColor = myColor + GetSpecular(myLamp, faces[i], myCamera.VPN);
				glColor3f(myColor.GetRed(), myColor.GetGreen(), myColor.GetBlue());*/
					
					glColor3f(1,1,1);

				glBegin(GL_TRIANGLES);

				Vector temp = vertices[faces[i].v[0]];
				
				//temp = myCamera.TransformtoCameraView(temp);
				temp = myCamera.GetCameraMatrix() * temp;
				/*temp.display();
				cout<<endl<<endl;*/
				glVertex3d(temp.x, temp.y, temp.z);
				temp = vertices[faces[i].v[1]];
				temp = myCamera.GetCameraMatrix() * temp;
				//temp = myCamera.TransformtoCameraView(temp);
				glVertex3d(temp.x, temp.y, temp.z);
				//cout<<endl<<endl;
				temp = vertices[faces[i].v[2]];
				temp = myCamera.GetCameraMatrix() * temp;
				//temp = myCamera.TransformtoCameraView(temp);
				glVertex3d(temp.x, temp.y, temp.z);
				//temp.display();
				//cout<<endl<<endl;
				glEnd();

				//glBegin(GL_POLYGON);
				//for (int j=0;j<3;j++) {
				//	//Vector a = myCamera.TransformtoCameraView(vt_vertices[faces[i].vt[j]]);
    //               // glTexCoord2d(vertices[faces[i].v[j]].x, vertices[faces[i].v[j]].y);
				//	Vector temp = vertices[faces[i].v[0]];
				////temp = myCamera.TransformtoCameraView(temp);
				///*vertices[faces[i].v[0]].display();
				//cout<<endl<<endl;*/
				//glVertex3d(temp.x, temp.y, temp.z);
				//temp = vertices[faces[i].v[1]];
				////temp = myCamera.TransformtoCameraView(temp);
				//glVertex3d(temp.x, temp.y, temp.z);
				///*vertices[faces[i].v[1]].display();
				//cout<<endl<<endl;*/
				//temp = vertices[faces[i].v[2]];
				////temp = myCamera.TransformtoCameraView(temp);
				//glVertex3d(temp.x, temp.y, temp.z);
    //            }
				//glEnd();
			}
			//glDisable(GL_TEXTURE_2D);
		}
	}
};

