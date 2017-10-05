#include "Shading.h"

class Camera{
public:
	Vector position;
	Vector VPN;
	Vector orientation; 
	Vector yAxis, xAxis;
	Vector objectPos;
	Matrix fin;

	Camera(Vector objpos, Vector pos, Vector orient){
		position = pos;
		objectPos = objpos;
		VPN = (objectPos - position).Normalize();
		orientation=(orient);
		fin.setIdentity();
	}
	void SetPos(Vector pos){
		position = pos;
	}
	void SetObjectPos(Vector objpos){
		objectPos = objpos;
	}
	void SetVPN(Vector objectPos){
		VPN = (objectPos - position).Normalize();
	}
	void SetYAxis(){ //v
		yAxis = orientation - VPN * (orientation ^ VPN);
		yAxis = yAxis.Normalize();
	}
	void SetXAxis(){  //u
		xAxis = VPN * yAxis; 
		xAxis = xAxis.Normalize();
	}
	Vector GetPos(){
		return position;
	}
	Vector GetVPN(){
		return VPN;
	}
	void Calculate(){
		SetVPN(objectPos);
		SetYAxis();
		SetXAxis();
	}
	void CalculateU(){
		SetVPN(objectPos);
		xAxis = VPN * yAxis;
		xAxis = xAxis.Normalize();
	}
	void CalculateV(){
		SetVPN(objectPos);
		yAxis = xAxis * VPN; 
		yAxis = yAxis.Normalize();
	}
	Matrix GetViewMatrix()
	{
		fin.setIdentity();
		fin.data[0][0] = xAxis.getX();
		fin.data[0][1] = xAxis.getY();
		fin.data[0][2] = xAxis.getZ();
		fin.data[0][3] = (position*-1) ^ xAxis;

		fin.data[1][0] = yAxis.getX();
		fin.data[1][1] = yAxis.getY();
		fin.data[1][2] = yAxis.getZ();
		fin.data[1][3] = (position*-1) ^ yAxis;

		fin.data[2][0] = VPN.getX();
		fin.data[2][1] = VPN.getY();
		fin.data[2][2] = VPN.getZ();
		fin.data[2][3] = (position*-1) ^ VPN;
		return fin;
	}
	void Throw(Matrix a){
		position = a * position;
		fin.data[0][3] = (position*-1) ^ xAxis;
		fin.data[1][3] = (position*-1) ^ yAxis;
		fin.data[2][3] = (position*-1) ^ VPN;
	}
	void Strafe(int dir, float speed){
		if (dir == 1){
			position = position + (xAxis * -speed);
		}
		if (dir == 2){
			position = position + (VPN * speed);
		}
		if (dir == 3){
			position = position + (xAxis * speed);
		}
		if (dir == 4){
			position = position + (VPN * -speed);
		}
		if (dir == 5){
			position = position + (yAxis * speed);
		}
		if (dir == 6){
			position = position + (yAxis * -speed);
		}

		fin.data[0][3] = (position*-1) ^ xAxis;
		fin.data[1][3] = (position*-1) ^ yAxis;
		fin.data[2][3] = (position*-1) ^ VPN;
	}

	Vector TransformtoCameraView(Vector object){
		Vector result;
		result = GetViewMatrix() * object;
		return result;
	}

	//xAxis
	void RotateX(double deg){

		//setup rotation matrix
		Matrix rot;
		double px = pow(xAxis.x, 2), py = pow(xAxis.y, 2), pz = pow(xAxis.z, 2);
		double x = xAxis.x, y = xAxis.y, z = xAxis.z;

		rot.data[0][0] = px + (py + pz) * cos(deg);
		rot.data[1][0] = x * y * (1 - cos(deg)) - z * (sin(deg));
		rot.data[2][0] = x * z * (1 - cos(deg)) + y * (sin(deg));
		rot.data[3][0] = position.x * (py + pz) - x*(position.y * y + position.z * z) * (1 - cos(deg)) + (position.y * z - position.z * y)*sin(deg);
		
		rot.data[0][1] = x * y * (1 - cos(deg)) + z * (sin(deg));
		rot.data[1][1] = py + (px + pz) * cos(deg);
		rot.data[2][1] = y * z * (1 - cos(deg)) - x * (sin(deg));
		rot.data[3][1] = position.y * (px + pz) - y*(position.x * x + position.z * z) * (1 - cos(deg)) + (position.z * x - position.x * z)*sin(deg);
		
		rot.data[0][2] = x * z * (1 - cos(deg)) - y * (sin(deg));;
		rot.data[1][2] = y * z * (1 - cos(deg)) + x * (sin(deg));
		rot.data[2][2] = pz + (px + py) * cos(deg);
		rot.data[3][2] = position.z * (px + py) - z*(position.x * x + position.y * y) * (1 - cos(deg)) + (position.x * y - position.y * x)*sin(deg);

		// recalculate camera matrix
		VPN = rot * VPN;
		yAxis = rot * yAxis;
		fin.data[1][0] = yAxis.getX();
		fin.data[1][1] = yAxis.getY();
		fin.data[1][2] = yAxis.getZ();
		fin.data[1][3] = (position*-1) ^ yAxis;

		fin.data[2][0] = VPN.getX();
		fin.data[2][1] = VPN.getY();
		fin.data[2][2] = VPN.getZ();
		fin.data[2][3] = (position*-1) ^ VPN;

	}

	//yAxis
	void RotateY(double deg){
		//setup rotation matrix
		Matrix rot;
		double px = pow(yAxis.x, 2), py = pow(yAxis.y, 2), pz = pow(yAxis.z, 2);
		double x = yAxis.x, y = yAxis.y, z = yAxis.z;

		rot.data[0][0] = px + (py + pz) * cos(deg);
		rot.data[1][0] = x * y * (1 - cos(deg)) - z * (sin(deg));
		rot.data[2][0] = x * z * (1 - cos(deg)) + y * (sin(deg));
		rot.data[3][0] = position.x * (py + pz) - x*(position.y * y + position.z * z) * (1 - cos(deg)) + (position.y * z - position.z * y)*sin(deg);

		rot.data[0][1] = x * y * (1 - cos(deg)) + z * (sin(deg));
		rot.data[1][1] = py + (px + pz) * cos(deg);
		rot.data[2][1] = y * z * (1 - cos(deg)) - x * (sin(deg));
		rot.data[3][1] = position.y * (px + pz) - y*(position.x * x + position.z * z) * (1 - cos(deg)) + (position.z * x - position.x * z)*sin(deg);

		rot.data[0][2] = x * z * (1 - cos(deg)) - y * (sin(deg));;
		rot.data[1][2] = y * z * (1 - cos(deg)) + x * (sin(deg));
		rot.data[2][2] = pz + (px + py) * cos(deg);
		rot.data[3][2] = position.z * (px + py) - z*(position.x * x + position.y * y) * (1 - cos(deg)) + (position.x * y - position.y * x)*sin(deg);

		// recalculate camera matrix
		VPN = (rot * VPN).Normalize();
		xAxis = (rot * xAxis).Normalize();
		fin.data[0][0] = xAxis.getX();
		fin.data[0][1] = xAxis.getY();
		fin.data[0][2] = xAxis.getZ();
		fin.data[0][3] = (position*-1) ^ xAxis;

		fin.data[2][0] = VPN.getX();
		fin.data[2][1] = VPN.getY();
		fin.data[2][2] = VPN.getZ();
		fin.data[2][3] = (position*-1) ^ VPN;

		RefreshMatrix();
	}

	//VPN
	void RotateZ(double deg){
		//setup rotation matrix
		Matrix rot;
		double px = pow(VPN.x, 2), py = pow(VPN.y, 2), pz = pow(VPN.z, 2);
		double x = VPN.x, y = VPN.y, z = VPN.z;

		rot.data[0][0] = px + (py + pz) * cos(deg);
		rot.data[1][0] = x * y * (1 - cos(deg)) - z * (sin(deg));
		rot.data[2][0] = x * z * (1 - cos(deg)) + y * (sin(deg));
		rot.data[3][0] = position.x * (py + pz) - x*(position.y * y + position.z * z) * (1 - cos(deg)) + (position.y * z - position.z * y)*sin(deg);

		rot.data[0][1] = x * y * (1 - cos(deg)) + z * (sin(deg));
		rot.data[1][1] = py + (px + pz) * cos(deg);
		rot.data[2][1] = y * z * (1 - cos(deg)) - x * (sin(deg));
		rot.data[3][1] = position.y * (px + pz) - y*(position.x * x + position.z * z) * (1 - cos(deg)) + (position.z * x - position.x * z)*sin(deg);

		rot.data[0][2] = x * z * (1 - cos(deg)) - y * (sin(deg));;
		rot.data[1][2] = y * z * (1 - cos(deg)) + x * (sin(deg));
		rot.data[2][2] = pz + (px + py) * cos(deg);
		rot.data[3][2] = position.z * (px + py) - z*(position.x * x + position.y * y) * (1 - cos(deg)) + (position.x * y - position.y * x)*sin(deg);

		// recalculate camera matrix
		VPN = (rot * VPN).Normalize();
		xAxis = (rot * xAxis).Normalize();
		fin.data[0][0] = xAxis.x;
		fin.data[0][1] = xAxis.y;
		fin.data[0][2] = xAxis.z;
		fin.data[0][3] = (position*-1) ^ xAxis;

		fin.data[1][0] = yAxis.x;
		fin.data[1][1] = yAxis.y;
		fin.data[1][2] = yAxis.z;
		fin.data[1][3] = (position*-1) ^ yAxis;
		RefreshMatrix();
	}
	
	void Display(){
		printf("My Matrix: \n");
		for (int i = 0; i < 4; i++){
			for (int j = 0; j < 4; j++){
				printf("%lf ", fin.data[i][j]);
			}
			printf("\n");
		}
	}

	void RefreshMatrix(){
		for (int i = 0; i < 4; i++){
			for (int j = 0; j < 4; j++){
				fin.data[i][j] = abs(fin.data[i][j]) < pow(10.0, -6) ? 0 : fin.data[i][j];
			}
		}
	}

	void LookAroundY(double deg){
		Matrix rot;
		rot.rotatey(deg);
		position = rot * position;
		CalculateU();
	}

	void LookAroundX(double deg){
		Matrix rot;
		rot.rotatex(deg);
		position = rot * position;
		CalculateV();
	}

};