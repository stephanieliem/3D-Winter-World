#include "Vector.h"

double phi = 3.14;

class Matrix{
public:
	double data[4][4];
	Matrix(){
		for (int i=0; i<4; i++){
			for (int j=0; j<4; j++){
				if (i==j){
					data[i][j]=1;
				}else{
					data[i][j]=0;
				}
			}
		}
	}

	void setIdentity()
	{
		for (int i=0; i<4; i++){
			for (int j=0; j<4; j++){
				if(i==j)
				{
					data[i][j]=1;
				}else
				{
					data[i][j]=0;
				}
			}
		}

	}
	void setmatrix(int i, int j, double n){
		data[i][j]=n;
	}
	double getdata(int i, int j){
		return data[i][j];
	}
	Matrix transpose(){
		Matrix b;
		for (int i=0; i<4; i++){
			for (int j=0; j<4; j++){
				b.data[j][i]=data[i][j];
			}
		}
		return b;
	}
	Vector operator* (Vector a){
		Vector b;
		double datavector[4], hasilvector[4]={0,0,0,0};
		datavector[3]=a.getW();
		datavector[0]=a.getX();
		datavector[1]=a.getY();
		datavector[2]=a.getZ();
		for (int i=0; i<4; i++){
			for (int j=0; j<4; j++){
				hasilvector[i]+=data[i][j]*datavector[j];
			}
		}
		b.setX(hasilvector[0]);
		b.setY(hasilvector[1]);
		b.setZ(hasilvector[2]);
		b.setW(hasilvector[3]);
		return b;
	}
	Matrix operator+ (Matrix a){
		Matrix b;
		for (int i=0; i<4; i++){
			for (int j=0; j<4; j++){
				b.data[i][j]=data[i][j]+a.data[i][j];
			}
		}
		return b;
	}
	Matrix operator- (Matrix a){
		Matrix b;
		for (int i=0; i<4; i++){
			for (int j=0; j<4; j++){
				b.data[i][j]=data[i][j]-a.data[i][j];
			}
		}
		return b;
	}
	Matrix operator* (double n){
		Matrix b;
		for (int i=0; i<4; i++){
			for (int j=0; j<4; j++){
				b.data[i][j]=data[i][j]*n;
			}
		}
		return b;
	}
	Matrix operator* (Matrix a){
		Matrix b;
		for (int i=0; i<4; i++){
			for (int j=0; j<4; j++){
				b.data[i][j]=0;
				for (int k=0; k<4; k++){
					b.data[i][j]+=data[i][k]*a.data[k][j];
				}
			}
		}
		return b;
	}

	void rotatex(double a){
		for (int i=0; i<4; i++){
			for (int j=0; j<4; j++){
				if(i==j)
				{
					setmatrix(i,j,1);
				}else
				{
					setmatrix(i,j,0);
				}
			}
		}
		float deg = a * phi/180;
		setmatrix(0,0,1);
		setmatrix(1,1,cos(deg));
		setmatrix(1,2,-(sin(deg)));
		setmatrix(2,1,sin(deg));
		setmatrix(2,2,cos(deg));
	}
	void rotatey(double a){
		for (int i=0; i<3; i++){
			for (int j=0; j<3; j++){
				setmatrix(i,j,0);
			}
		}
		setmatrix(0,0,cos(a*phi/180));
		setmatrix(0,2,sin(-a*phi/180));
		setmatrix(1,1,1);
		setmatrix(2,0,sin(a*phi/180));
		setmatrix(2,2,cos(a*phi/180));
	}
	void rotatez(double a){
		for (int i=0; i<3; i++){
			for (int j=0; j<3; j++){
				setmatrix(i,j,0);
			}
		}
		setmatrix(0,0,cos(a*phi/180));
		setmatrix(0,1,sin(-a*phi/180));
		setmatrix(1,0,sin(a*phi/180));
		setmatrix(1,1,cos(a*phi/180));
		setmatrix(2,2,1);
	}
	void translation(double a, double b, double c){
		for (int i=0; i<4; i++){
			for (int j=0; j<4; j++){
				if (i==j){
					setmatrix(i,j,1);
				}else{
					setmatrix(i,j,0);
				}
			}
		}
		setmatrix(0,3,a);
		setmatrix(1,3,b);
		setmatrix(2,3,c);
	}

	void dilatation(double x){
		for (int i=0; i<4; i++){
			for (int j=0; j<4; j++){
				if (i == j && i<3) {
					setmatrix(i, j, x);
				}
				else if (i == 3 && j == 3) {
					setmatrix(i, j, 1);
				}
				else {
					setmatrix(i, j, 0);
				}
			}
		}
	}

	void display(){
		for (int i=0; i<4; i++){
			for (int j=0; j<4; j++){
				cout << data[i][j] << " ";
			}
			cout<<endl;
		}
	}
};