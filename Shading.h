#include"Hierarchy.h"

class Color{
public:
	float red, green, blue;
	Color(int r = 0, int g = 0, int b = 0){
		red = r;
		green = g;
		blue = b;
	}
	void SetRGB(float r, float g, float b){
		red = r;
		green = g;
		blue = b;
	}
	float GetRed(){
		return red;
	}
	float GetGreen(){
		return green;
	}
	float GetBlue(){
		return blue;
	}
	Color operator+(Color x){
		Color temp;
		temp.red = red + x.GetRed();
		temp.blue = blue + x.GetBlue();
		temp.green = green + x.GetGreen();
		return temp;
	}
	Color operator*(float koef){
		Color temp;
		temp.red = red * koef;
		temp.green = green * koef;
		temp.blue = blue * koef;
		return temp;
	}
};

class Lamp{
public:
	float IA, IS;
	Vector position;
	Color myColor;
	Lamp(float ia, float is, Vector pos){
		IA = ia;
		IS = is;
		position = pos;
		myColor.SetRGB(1,0,0);
	}
	void SetPosition(Vector pos){
		position.setAll(pos.getX(), pos.getY(), pos.getZ(),1);
	}
	void SetIA(float ia){
		IA = ia;
	}
	void SetIS(float is){
		IS = is;
	}
	void SetColor(float r, float g, float b){
		myColor.SetRGB(r,g,b);
	}
	Vector GetPos(){
		return position;
	}
	float GetIA(){
		return IA;
	}
	float GetIS(){
		return IS;
	}
	Color GetColor(){
		return myColor;
	}
	void Draw(Matrix camera){
		Hierarchy lamp[1];
		lamp[0].setQuadric(2, 10, 10, 10);
		Matrix trans;
		trans.translation(position.x, position.y, position.z);
		lamp[0].setPos(trans);
		lamp[0].pos = camera * lamp[0].pos;
		lamp[0].draw(1, 1, 0.3, lamp, 0); 
	}
};

