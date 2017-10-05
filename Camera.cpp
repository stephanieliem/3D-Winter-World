#include "Mesh.h"
#include <iostream>
using namespace std;

//Init
bool checkmove = false;
double totmove = 0, diff = 5, totrot1 = 0, diff1 = 5, maxleft = -50, maxright = 50, xdiff = 4, zdiff = -4, maxleft1 = -20, maxright1 = 20, maxfront = 40, maxback = -40, totx = 0, totz = 0, diff2 = 5, totdegree = 0, collision = true;
Vector objpos(0, 0, 0), pos(0, 7, 350), orient(0, 1, 0);
Camera myCam(objpos, pos, orient);
Lamp myLamp(0.1, 0.9, Vector(-20, 130, 20));
Vector Eye(0,0,100);

Matrix transteddy, transrumah, transbox, trans, transpiz, transis, transfi, rotationisland, transis2, translationisland, transis3, transrumah2, transr, transmissile, transsnow;
Material *mat = new Material("skynew.bmp");
Material *mats = new Material("Cottage_Texture.bmp");
Material *matpiz = new Material("piz.bmp");
Material *matis = new Material("Base.bmp");
Material *matfire = new Material("firee.bmp");
Material *mathouse = new Material("building03_c.bmp");
Material *matrmh = new Material("buiding.bmp");
Material *matmissile = new Material("rudal_texture.bmp");
Material *matsnowman = new Material("snowman_texture.bmp");

Mesh *teddy = new Mesh("Teddy Bear.txt", false);
Mesh *pizza = new Mesh("pizza.obj", true, matpiz);
Mesh *rumah = new Mesh("omah.obj", true, mats);
Mesh *skybox = new Mesh("sky.obj", true, mat);
Mesh *island = new Mesh("mini2.obj", true, matis);
Mesh *island2 = new Mesh("mini2.obj", true, matis);
Mesh *island3 = new Mesh("mini2.obj", true, matis);
Mesh *fire = new Mesh("fireee.obj", true, matfire);
Mesh *house = new Mesh("newhouse.obj", true, mathouse);
Mesh *housea = new Mesh("rumahagain.obj", true, matrmh);
Mesh *missile = new Mesh("rudal.obj", true, matmissile);
Mesh *snowman = new Mesh("mann.obj", true, matsnowman);

void Display(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	teddy->SetColor(0.3, 0, 0);
	
	//Draw all mesh
	teddy->Draw(myLamp, myCam, transteddy);
	rumah->Draw(myLamp, myCam, transrumah);
	skybox->Draw(myLamp, myCam, transbox);
	pizza->Draw(myLamp, myCam, transpiz);
	island->Draw(myLamp, myCam, transis);
	island2->Draw(myLamp, myCam, transis2);
	island3->Draw(myLamp, myCam, transis3);
	fire->Draw(myLamp, myCam, transfi);
	house->Draw(myLamp, myCam, transrumah2);
	housea->Draw(myLamp, myCam, transr);
	missile->Draw(myLamp, myCam, transmissile);
	snowman->Draw(myLamp, myCam, transsnow);
	
	//Animation
	if (checkmove) {
		rotationisland.setIdentity();
		rotationisland.rotatey(diff1);
		teddy->addTransform(rotationisland);
		rumah->addTransform(rotationisland);
		pizza->addTransform(rotationisland);
		fire->addTransform(rotationisland);
		snowman->addTransform(rotationisland);
		island->addTransform(rotationisland);
	//	island->da(rotationisland);
		rotationisland.setIdentity();
		rotationisland.rotatey(diff2);
		missile->addTransform(rotationisland);

		translationisland.setIdentity();
		translationisland.translation(0, diff, 0);
		housea->addTransform(translationisland);
		island2->addTransform(translationisland);

		translationisland.setIdentity();
		translationisland.translation(xdiff, 0, zdiff);
		house->addTransform(translationisland);
		island3->addTransform(translationisland);

		totdegree += diff2;

		//Island 1
		totmove += diff;
		if (totmove == 40 || totmove == -40) {
			diff = diff * -1;
		}
		
		//Island 2
		totrot1 += diff1;
		if (totrot1 <= maxleft && diff1 == -5) {
			diff1 = diff1 * -1;
			maxleft = -maxleft;
		} 
		if (totrot1 >= maxright && diff1 == 5) {
			diff1 = diff1 * -1;
			maxright = -maxright;
		}

		//Island 3
		totx += xdiff;
		totz += zdiff;
		if (totx <= maxleft1 && xdiff == -4) {
			xdiff = xdiff * -1;
			maxleft1 = -maxleft1;
		} 
		if (totx >= maxright1 && xdiff == 4) {
			xdiff = xdiff * -1;
			maxright1 = -maxright1;
		}
		if (totz <= maxback && zdiff == -4) {
			zdiff = zdiff * -1;
			maxback = -maxback;
		} 
		if (totz >= maxfront && zdiff == 4) {
			zdiff = zdiff * -1;
			maxfront = -maxfront;
		}
	}
	glutSwapBuffers();
	skybox->SetColor(0.5, 0.5, 0.5);
}

void whenidle(){
	if (collision) {
		Vector temp1 = myCam.GetPos() - island->getPosition();
		if (temp1.Magnitude() < 290) {
			myCam.SetPos(myCam.GetPos() - (temp1 - temp1.Normalize() * 290));
		}
		Vector temp2 = myCam.GetPos() - island2->getPosition();
		if (temp2.Magnitude() < 290) {
			myCam.SetPos(myCam.GetPos() - (temp2 - temp2.Normalize() * 290));
		}
		Vector temp3 = myCam.GetPos() - island3->getPosition();
		if (temp3.Magnitude() < 290) {
			myCam.SetPos(myCam.GetPos() - (temp3 - temp3.Normalize() * 290));
		}
	}
	glutPostRedisplay();
}

void DoInit(){
	glOrtho(-100, 100, -100, 100, -100, 100);

	glMatrixMode(GL_PROJECTION);
	glViewport(0, 0, 700, 500);
	gluPerspective(90, 700/500, 0.1, 50);
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_DEPTH_TEST);

	//Set Position
	Matrix translation, dilatation, rotation;
	translation.translation(10, -30, 250);
	rotation.rotatey(-35);
	dilatation.dilatation(0.5);
	transteddy = dilatation * translation * rotation;
	teddy->setMatrixPos(transteddy);

	translation.setIdentity();
	translation.translation(210, -405, 2350);
	dilatation.dilatation(0.06);
	rotation.setIdentity();
	rotation.rotatey(-100);
	transpiz = dilatation * translation * rotation;
	pizza->setMatrixPos(transpiz);

	translation.setIdentity();
	rotation.setIdentity();
	translation.translation(10, -33, 120);
	transrumah = translation * rotation;
	rotation.setIdentity();
	rotation.rotatex(-3);
	transrumah = rotation * transrumah;
	rumah->setMatrixPos(transrumah);

	translation.setIdentity();
	dilatation.dilatation(80);
	translation.translation(0, -0.47, 1);
	transis = dilatation * translation;
	island->setMatrixPos(transis);

	translation.setIdentity();
	translation.translation(-40, -23, 200);
	transfi = translation;
	fire->setMatrixPos(transfi);

	dilatation.setIdentity();
	translation.setIdentity();
	dilatation.dilatation(800);
	transbox = dilatation; 
	skybox->setMatrixPos(transbox);

	dilatation.setIdentity();
	translation.setIdentity();
	rotation.setIdentity();
	dilatation.dilatation(50);
	rotation.rotatey(10);
	translation.translation(5, 3, -5);
	transis2 = dilatation * translation * rotation;
	island2->setMatrixPos(transis2);
	Matrix temp1 = dilatation * translation;

	dilatation.setIdentity();
	translation.setIdentity();
	rotation.setIdentity();
	dilatation.dilatation(40);
	rotation.rotatey(50);
	translation.translation(-5, 1, -3);
	transis3 = dilatation * translation * rotation;
	island3->setMatrixPos(transis3);

	dilatation.setIdentity();
	translation.setIdentity();
	rotation.setIdentity();
	dilatation.dilatation(3);
	translation.translation(-124, 45, -33);
	rotation.rotatey(5);
	transrumah2 = translation * rotation * dilatation;
	house->setMatrixPos(transrumah2);

	translation.setIdentity();
	rotation.setIdentity();
	translation.translation(360, 165, -80);
	rotation.rotatex(-3);
	transr = translation * rotation * dilatation;
	housea->setMatrixPos(transr);

	translation.setIdentity();
	rotation.setIdentity();
	translation.translation(-10, 100, 140);
	transmissile = translation;
	missile->setMatrixPos(transmissile);

	dilatation.setIdentity();
	translation.setIdentity();
	rotation.setIdentity();
	translation.translation(-10, -3, 14);
	rotation.rotatey(270);
	dilatation.dilatation(8);
	transsnow = dilatation * translation * rotation;
	snowman->setMatrixPos(transsnow);

	//Load Texture
	mat->LoadTextureFromFile("skynew.bmp");
	matpiz->LoadTextureFromFile("piz.bmp");
	mats->LoadTextureFromFile("Cottage_Texture.bmp");
	matis->LoadTextureFromFile("Base.bmp");
	matfire->LoadTextureFromFile("firee.bmp");
	mathouse->LoadTextureFromFile("building03_c.bmp");
	matrmh->LoadTextureFromFile("buiding.bmp");
	matmissile->LoadTextureFromFile("rudal_texture.bmp");
	matsnowman->LoadTextureFromFile("snowman_texture.bmp");
	rumah->SetColor(1,1,1);
	
	myCam.Calculate();

	cout << "Key :" << endl;
	cout << "   q -> Look At Left" << endl;
	cout << "   e -> Look At Right" << endl;
	cout << "   r -> Look At Top" << endl;
	cout << "   f -> Look At Bottom" << endl;
	cout << "   w -> Forward Camera" << endl;
	cout << "   a -> Left Camera" << endl;
	cout << "   s -> Right Camera" << endl;
	cout << "   d -> Back Camera" << endl;
	cout << "   t -> Up Camera" << endl;
	cout << "   g -> Down Camera" << endl;
	cout << "   z -> Look Around Y-Axis (-)" << endl;
	cout << "   x -> Look Around Y-Axis (+)" << endl;
	cout << "   c -> Look Around X-Axis (-)" << endl;
	cout << "   v -> Look Around X-Axis (+)" << endl;
	cout << "   i -> Forward Lamp" << endl;
	cout << "   j -> Left Lamp" << endl;
	cout << "   k -> Back Lamp" << endl;
	cout << "   l -> Right Lamp" << endl;
	cout << "   n -> Up Lamp" << endl;
	cout << "   m -> Down Lamp" << endl;
	cout << "   Space -> Toogle Animation (ON/OFF)" << endl;
	cout << "   y -> Toogle Collision (ON/OFF)" << endl;
}

void KeyboardEvent(unsigned char key, int x, int y){
	double deg = 0.3;
	Matrix rot, trans;
	if (key == 'q'){
		myCam.RotateY(-deg);
	}
	else if (key == 'e'){
		myCam.RotateY(deg);
	}
	else if (key == 'r'){
		myCam.RotateX(-deg);
	}
	else if (key == 'f'){
		myCam.RotateX(deg);
	}
	else if (key == 'w'){
		myCam.Strafe(2, 4.0f);
	}
	else if (key == 'a'){
		myCam.Strafe(1, 4.0f);
	}
	else if (key == 's'){
		myCam.Strafe(4, 4.0f);
	}
	else if (key == 'd'){
		myCam.Strafe(3, 4.0f);
	}
	else if (key == 't'){
		myCam.Strafe(5, 4.0f);
	}
	else if (key == 'g'){
		myCam.Strafe(6, 4.0f);
	}
	else if (key == 'z'){
		myCam.LookAroundY(-10);
	}
	else if (key == 'x'){
		myCam.LookAroundY(10);
	}
	else if (key == 'c'){
		myCam.LookAroundX(-10);
	}
	else if (key == 'v'){
		myCam.LookAroundX(10);
	}
	else if (key == ' '){
		if (checkmove) checkmove = false;
		else checkmove = true;
	}
	if (key == 'i') {
		myLamp.position.z += -10;
	}
	else if (key == 'j') {
		myLamp.position.x += -10;
	}
	else if (key == 'k') {
		myLamp.position.z += 10;
	}
	else if (key == 'l') {
		myLamp.position.x += 10;
	}
	else if (key == 'n') {
		myLamp.position.y += 10;
	}
	else if (key == 'm') {
		myLamp.position.y += -10;
	}
	else if (key == 'y') {
		if (collision) collision = false;
		else collision = true;
	}
}

void main(int argc, char**argv){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(700, 500);
	glutInitWindowPosition(10, 10);
	glutCreateWindow("Winter Wonderland");
	glutKeyboardFunc(KeyboardEvent);
	glutDisplayFunc(Display);
	glutIdleFunc(whenidle);
	DoInit();
	glutMainLoop();
}