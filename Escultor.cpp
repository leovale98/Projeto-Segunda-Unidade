#include "Escultor.h"
using namespace std;


// FUNÇÕES DE CANVAS

void Canvas::startCanvas(int sx, int sy, int sz, int mx, int my, int mz){
	minX = mx;
	minY = my;
	minZ = mz;
	dimX = sx;
	dimY = sy;
	dimZ = sz;
	if (dimX*dimY*dimZ > 0)
		x = vector <Voxel> (dimX * dimY * dimZ, Voxel());
}
int Canvas::getFaces() const{
	int quantity=0;
	//cout << "RAIO ";
	for (int i = 0; i < dimX; ++i) {

    	for (int j = 0; j < dimY; ++j) {

      		for (int k = 0; k < dimZ; ++k) {

        		if ( local(i, j, k).is_on ) {
         			quantity += 3;
          			if (j == 0 or !local(i, j - 1, k).is_on ) ++quantity;
          			if (i == 0 or !local(i - 1, j, k).is_on ) ++quantity;
         			if (k == 0 or !local(i, j, k - 1).is_on ) ++quantity;

        		}


     		}
    	}
	}
	return quantity;
}
void Canvas::clean(){
	//vector <pInt> posicoes;
  //posicoes.reserve(dimX * dimY * dimZ);

  for (int i = 0; i < dimX; ++i) {
    for (int j = 0; j < dimY; ++j) {
      for (int k = 0; k < dimZ; ++k) {
        if (
          // Se ele não pertencer a nenhuma das extremidades e se esle está ativo
          i != 0 && j != 0 && k != 0 && local(i, j, k).is_on &&
          i < (dimX - 1) && j < (dimY - 1) && k < (dimZ - 1) &&
          // Se ele estiver rodeado de mais 6 voxeis
          local(i - 1, j, k).is_on &&
          local(i + 1, j, k).is_on &&
          local(i, j - 1, k).is_on &&
          local(i, j + 1, k).is_on &&
          local(i, j, k - 1).is_on &&
          local(i, j, k + 1).is_on
        ) {
          local(i, j, k).a = 0.0;
         }
      }
    }
  }


    for (int i = 0; i < dimX; ++i) {
      for (int j = 0; j < dimY; ++j) {
        for (int k = 0; k < dimZ; ++k) {
          if (local(i, j, k).a == 0.0) local(i, j, k).is_on = false;
        }
      }
}

}

//FUNÇÕES DE BOX
void Box::sculpt(Canvas &c) const{
    for (int i = getMinX(); i < getMaxX(); i++) {

    	for (int j = getMinY(); j < getMaxY(); j++) {

      		for (int k = getMinZ(); k < getMaxZ(); k++) {
      			//cout << c.getPosX(i) << ' ' << c.getPosY(j) << ' ' << c.getPosZ(k) << ' ';

      			c.local(c.getPosX(i), c.getPosY(j), c.getPosZ(k)) = Voxel(v.r,v.g,v.b,v.a, v.is_on);
      		}
    	}
   }
}

//FUNÇÕES DE ESFERA
void Sphere::sculpt(Canvas &c) const{
	for (int i = getMinX(); i < getMaxX(); i++) {
    	for (int j = getMinY(); j < getMaxY(); j++) {
      		for (int k = getMinZ(); k < getMaxZ(); k++) {
				if(pow(i-x,2) + pow(j-y,2) + pow(k-z,2)<pow(radius,2))
      				c.local(c.getPosX(i), c.getPosY(j), c.getPosZ(k)) = Voxel(v.r,v.g,v.b,v.a, v.is_on);
      		}
    	}
    }
}

//FUNÇÕES DE ELLIPSOID
void Ellipsoid::sculpt(Canvas &c) const{
	for (int i = getMinX(); i < getMaxX(); i++) {
    	for (int j = getMinY(); j < getMaxY(); j++) {
      		for (int k = getMinZ(); k < getMaxZ(); k++) {
				if((pow((i-x)/rx,2)+ pow((j-y)/ry,2) + pow((k-z)/rz,2)<1)){
      				c.local(c.getPosX(i), c.getPosY(j), c.getPosZ(k)) = Voxel(v.r,v.g,v.b,v.a, v.is_on);
				}
      		}
    	}
    }
}
//FUNÇÕES DE CYLINDER
void Cylinder::sculpt(Canvas &c) const{//Desenha um cilindro que sobe no eixo Z
	for (int i = getMinX(); i < getMaxX(); i++) {
    	for (int j = getMinY(); j < getMaxY(); j++) {
      		for (int k = getMinZ(); k < getMaxZ(); k++) {
				if(pow(i-x,2) + pow(j-y,2)<pow(radius,2))
      				c.local(c.getPosX(i), c.getPosY(j), c.getPosZ(k)) = Voxel(v.r,v.g,v.b,v.a, v.is_on);
      		}
    	}
    }
}
//FUNÇÕES DE CYLINDERX
void CylinderX::sculpt(Canvas &c) const{//Desenha um cilindro que sobe no eixo X
	for (int i = getMinX(); i < getMaxX(); i++) {
    	for (int j = getMinY(); j < getMaxY(); j++) {
      		for (int k = getMinZ(); k < getMaxZ(); k++) {
				if(pow(j-y,2) + pow(k-z,2)<pow(radius,2))
      				c.local(c.getPosX(i), c.getPosY(j), c.getPosZ(k)) = Voxel(v.r,v.g,v.b,v.a, v.is_on);
      		}
    	}
    }
}
//FUNÇÕES DE PRISM
void Prism::sculpt(Canvas &c) const{//Desenha o prisma, é como um box, mas corta tudo acima de uma linha e abaixo de outra
	//cout<<getMaxZ()<<endl;
	//Calcula os coefcicientes angulares das retas que vai retirar
	float m1,m2;
	m1 =(z1-z)/(x1-x);
	m2 =(z2-z)/(x2-x);
	//cout<< " m1: "<<m1<<" m2: "<< m2 <<endl;
	for (int i = getMinX(); i < getMaxX(); i++) {
    	for (int j = getMinY(); j < getMaxY(); j++) {
      		for (int k = getMinZ(); k < getMaxZ(); k++) {
				if((k-z)>=(m1*(i-x)) && (k-z)>=(m2*(i-x))){//Coloca como visivel só os voxels abaixo da primeira reta e acima da segunda
					c.local(c.getPosX(i), c.getPosY(j), c.getPosZ(k)) = Voxel(v.r,v.g,v.b,v.a, v.is_on);
				}
      		}
    	}
    }
}

//FUNÇÕES DE SCULPTOR

float Sculptor::getMaxX(){
	float maxX, present;
	for (Iter i = s.begin(); i != s.end(); ++i){
	    present = (*i)->getMaxX();
	    //cout << present << ' ';
	    if (i == s.begin()) maxX = present;
	    if (maxX < present) maxX = present;
   }
	return maxX;
}

float Sculptor::getMaxY(){
	float maxY, present;
	for (Iter i = s.begin(); i != s.end(); ++i){
	    present = (*i)->getMaxY();
	    if (i == s.begin()) maxY = present;
	    if (maxY < present) maxY = present;
   }
	return maxY;
}
float Sculptor::getMaxZ(){
	float maxZ, present;
	for (Iter i = s.begin(); i != s.end(); ++i){
	    present = (*i)->getMaxZ();
	    if (i == s.begin()) maxZ = present;
	    if (maxZ < present) maxZ = present;
   }
	return maxZ;
}
float Sculptor::getMinX(){
	float minX, present;
	for (Iter i = s.begin(); i != s.end(); ++i){
	    present = (*i)->getMinX();
	    if (i == s.begin()) minX = present;
	    if (minX > present) minX = present;
   }
	return minX;
}
float Sculptor::getMinY(){
	float minY, present;
	for (Iter i = s.begin(); i != s.end(); ++i){
	    present = (*i)->getMinY();
	    if (i == s.begin()) minY = present;
	    if (minY > present) minY = present;
   }
	return minY;
}

float Sculptor::getMinZ(){
	float minZ, present;
	for (Iter i = s.begin(); i != s.end(); ++i){
	    present = (*i)->getMinZ();
	    if (i == s.begin()) minZ = present;
	    if (minZ > present) minZ = present;
   }
	return minZ;
}

void Sculptor::putSphere(int Xc, int Yc, int Zc, int R){
	s.push_back( new Sphere(Xc, Yc, Zc, abs(R), r, g, b, a));
}
void Sculptor::cutSphere(int Xc, int Yc, int Zc, int R){
	s.push_back( new Sphere(Xc, Yc, Zc, abs(R), r, g, b, a, false));
}

void Sculptor::putBox(int X0, int X1, int Y0, int Y1, int Z0, int Z1) {
  	s.push_back( new Box((X1 + X0)/2.0,(Y1 + Y0) / 2.0,(Z1 + Z0) / 2.0, abs(X1 - X0), abs(Y1 - Y0), abs(Z1 - Z0), r, g, b, a));
}

void Sculptor::cutBox(int X0, int X1,int Y0, int Y1,int Z0, int Z1){
	s.push_back( new Box((X1 + X0)/2.0,(Y1 + Y0) / 2.0,(Z1 + Z0) / 2.0, abs(X1 - X0), abs(Y1 - Y0), abs(Z1 - Z0), r, g, b, a, false));
}

void Sculptor::putEllipsoid(int Xc, int Yc, int Zc, int Rx, int Ry, int Rz){
	s.push_back (new Ellipsoid(Xc, Yc, Zc, abs(Rx), abs(Ry), abs(Rz), r, g, b, a));
}
void Sculptor::cutEllipsoid(int Xc, int Yc, int Zc, int Rx, int Ry, int Rz){
	s.push_back (new Ellipsoid(Xc, Yc, Zc, abs(Rx), abs(Ry), abs(Rz), r, g, b, a,false));
}
void Sculptor::putCylinder(int Xc, int Yc, int Zc, int R, int H){
	s.push_back( new Cylinder(Xc, Yc, Zc, abs(R),H, r, g, b, a));
}
void Sculptor::cutCylinder(int Xc, int Yc, int Zc, int R, int H){
	s.push_back( new Cylinder(Xc, Yc, Zc, abs(R),H, r, g, b, a, false));
}
void Sculptor::putCylinderX(int Xc, int Yc, int Zc, int R, int H){
	s.push_back( new CylinderX(Xc, Yc, Zc, abs(R),H, r, g, b, a));
}
void Sculptor::cutCylinderX(int Xc, int Yc, int Zc, int R, int H){
	s.push_back( new CylinderX(Xc, Yc, Zc, abs(R),H, r, g, b, a, false));
}
void Sculptor::putPrism(int Xc, int Yc, int Zc,int X2,int Z2,int X3, int H){
	s.push_back( new Prism(Xc, Yc, Zc, X2,Z2, X3, 0,H,r,g, b, a));
}
void Sculptor::cutPrism(int Xc, int Yc, int Zc,int X2,int Z2,int X3, int H){
	s.push_back( new Prism(Xc, Yc, Zc, X2,Z2, X3, 0,H,r,g, b, a, false));
}

void Sculptor::write(const char *Arq){
	ofstream outOff(Arq);
	unsigned P0, P1, P2, P3, P4, P5, P6, P7;
	startCanvas(getMaxX() - getMinX(), getMaxY() - getMinY(), getMaxZ() - getMinZ(), getMinX(), getMinY(), getMinZ());
	//cout << getMaxX() << ' ' << getMinX();
	for(Iter i = s.begin(); i != s.end(); ++i){ (*i)->sculpt(*this); /*cout << "RAIO "*/; }
	clean();
	outOff << "OFF" << endl;
  	outOff << getVertices() << " " << getFaces() << " " << 0 << endl;

	for (int i = 0; i < dimX + 1; i++) {
	    for (int j = 0; j < dimY + 1; j++) {
	    	for (int k = 0; k < dimZ + 1; k++) {
	       	float a, b, c;
			a = -0.5 + i;
			b = -0.5 + j;
			c = -0.5 + k;
	        outOff << a << " " << b << " " << c << endl;
	    	}
	    }
	}

	for (int i = 0; i < dimX; ++i) {
    	for (int j = 0; j < dimY; ++j) {
      		for (int k = 0; k < dimZ; ++k) {
				P0 = getVoxelVertex(i, j, k);
                P1 = getVoxelVertex(i + 1, j, k);
                P2 = getVoxelVertex(i + 1, j, k + 1);
                P3 = getVoxelVertex(i, j, k + 1);
                P4 = getVoxelVertex(i, j + 1, k + 1);
                P5 = getVoxelVertex(i, j + 1, k);
                P6 = getVoxelVertex(i + 1, j + 1, k);
                P7 = getVoxelVertex(i + 1, j + 1, k + 1);
        		Voxel v = local(i, j, k);
        		if (v.is_on){
			        if (j == 0 or !local(i, j - 1, k).is_on )
			        	outOff << 4 << " " << P0 << " " << P1 << " " << P2 << " " << P3 << " " << v.r << " " << v.g << " " << v.b << " " << v.a << endl;
			        if (i == 0 or !local(i - 1, j, k).is_on )
			        	outOff << 4 << " " << P0 << " " << P3 << " " << P4 << " " << P5 << " " << v.r << " " << v.g << " " << v.b << " " << v.a << endl;
			        if (k == 0 or !local(i, j, k - 1).is_on )
			        	outOff << 4 << " " << P0 << " " << P5 << " " << P6 << " " << P1 << " " << v.r << " " << v.g << " " << v.b << " " << v.a << endl;
			        outOff << 4 << " " << P5 << " " << P4 << " " << P7 << " " << P6 << " " << v.r << " " << v.g << " " << v.b << " " << v.a << endl;
			        outOff << 4 << " " << P1 << " " << P6 << " " << P7 << " " << P2 << " " << v.r << " " << v.g << " " << v.b << " " << v.a << endl;
			        outOff << 4 << " " << P3 << " " << P2 << " " << P7 << " " << P4 << " " << v.r << " " << v.g << " " << v.b << " " << v.a << endl;
        		}
      		}
    	}
  	}
}



