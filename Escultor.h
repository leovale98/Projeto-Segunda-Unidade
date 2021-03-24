#ifndef _ESCULTOR_H_
#define _ESCULTOR_H_
#include <fstream>
#include <iostream>
#include <vector>
#include <list>
#include <cmath>
using namespace std;

typedef unsigned short int byte;

struct Voxel{
    float r,g,b;
    float a;
    bool is_on;
    Voxel (
    float R=0.0,
    float G=0.0,
    float B=0.0,
    float A=1.0,
    bool on=false
) : r(R), g(G), b(B), a(A), is_on(on) {}
};

class Canvas{
protected:
    int dimX, dimY, dimZ, minX, minY, minZ;
    vector<Voxel> x;
    void startCanvas(int sx, int sy, int sz, int mx=0, int my=0, int mz=0);
public:
    inline Canvas(int sx=0, int sy=0, int sz=0, int mx=0, int my=0, int mz=0){startCanvas(sx, sy, sz, mx, my, mz);}
    inline Voxel &local(int i, int j, int k){ return x.at(i*dimY*dimZ+j*dimZ+k);}
    inline const Voxel &local(int i, int j, int k) const { return x.at(i*dimY*dimZ+j*dimZ+k);}
    inline unsigned getVertices() const { return (dimX + 1) * (dimY + 1) * (dimZ + 1);}
    inline unsigned int getVoxelVertex(unsigned i, unsigned j, unsigned k) {return i * (dimZ + 1) * (dimY + 1) + j * (dimZ + 1) + k;}
    int getFaces() const;
    inline const int getPosX (int i) {return i-minX;} 
    inline const int getPosY (int j) {return j-minY;} 
    inline const int getPosZ (int k) {return k-minZ;} 
    void clean();
    
};

class Solid
{     
protected:
	float x, y, z;  
    Voxel v;
public:
    inline Solid(float i=0.0, float j=0.0, float k=0.0,float R=0.0,float G=0.0,float B=0.0,float A=1.0,bool on=true):x(i),y(j),z(k), v(R,G,B,A,on){};
    ~Solid ();
    virtual void sculpt(Canvas &c)const =0;
    inline void setPos(float i, float j, float k) {x = i; y = j; z = k;}
    inline void setColor(float red, float green, float blue, float alpha){v.r = red; v.g = green; v.b = blue; v.a = alpha;}
	virtual float getMaxX() const = 0;
  	virtual float getMaxY() const = 0;
  	virtual float getMaxZ() const = 0;
	virtual float getMinX() const = 0;
 	virtual float getMinY() const = 0;
	virtual float getMinZ() const = 0;
};

typedef Solid *pSolid;

class Box : public Solid
{
private:
    int sx, sy, sz;
public:       
    inline Box(float x0=0.0, float y0=0.0,float z0=0.0, int l=0, int c=0, int p=0,float R=0.0,float G=0.0,float B=0.0,float A=1.0,bool on=true):Solid::Solid(x0,y0,z0, R, G, B, A, on),sx(l), sy(c), sz(p){};
    void sculpt(Canvas &c) const;
	inline float getMaxX() const {  return (x + sx / 2.0);}
	inline float getMaxY() const {  return (y + sy / 2.0);}
	inline float getMaxZ() const {  return (z + sz / 2.0);}
	inline float getMinX() const {  return (x - sx / 2.0);}
	inline float getMinY() const {  return (y - sy / 2.0);}
	inline float getMinZ() const {  return (z - sz / 2.0);}
};


class Prism : public Solid
{
private:
    float x1,z1,x2, z2; 
	float h;
public:       
    inline Prism(float x0=0.0,float y0=0.0,float z0=0.0, float i =0, float k=0,float i1=0, float k1=0,int H=0,float R=0.0,float G=0.0,float B=0.0,float A=1.0,bool on=true):Solid::Solid(x0,y0,z0, R, G, B, A, on),x1(i), z1(k),x2(i1),z2(z1),h(H){};
    void sculpt(Canvas &c) const;
	inline float getMaxX() const {  return (max(max(x1,x2),x));}
	inline float getMaxY() const {  return (y+h);}
	inline float getMaxZ() const {  return (max(max(z1,z2),z));}
	inline float getMinX() const {  return (min(min(x1,x2),x));}
	inline float getMinY() const {  return (y);}
	inline float getMinZ() const {  return (min(min(z1,z2),z));}
};

class Sphere : public Solid
{
private:
	int radius;
public:
	inline Sphere(float x0=0.0, float y0=0.0,float z0=0.0, int rad=0, float R=0.0,float G=0.0,float B=0.0,float A=1.0,bool on=true):Solid::Solid(x0,y0,z0, R, G, B, A, on),radius(rad){};
	void sculpt(Canvas &c) const;
	inline float getMaxX() const {  return (x + radius);}
	inline float getMaxY() const {  return (y + radius);}
	inline float getMaxZ() const {  return (z + radius);}
	inline float getMinX() const {  return (x - radius);}
	inline float getMinY() const {  return (y - radius);}
	inline float getMinZ() const {  return (z - radius);}
};
class Cylinder : public Solid
{
private:
	int radius, h;
public:
	inline Cylinder(float x0=0.0, float y0=0.0,float z0=0.0, int rad=0, int H=0, float R=0.0,float G=0.0,float B=0.0,float A=1.0,bool on=true):Solid::Solid(x0,y0,z0, R, G, B, A, on),radius(rad),h(H){};
	void sculpt(Canvas &c) const;
	inline float getMaxX() const {  return (x + radius);}
	inline float getMaxY() const {  return (y + radius);}
	inline float getMaxZ() const {  return (z + h);}
	inline float getMinX() const {  return (x - radius);}
	inline float getMinY() const {  return (y - radius);}
	inline float getMinZ() const {  return (z);}
};
class CylinderX : public Solid
{
private:
	int radius, h;
public:
	inline CylinderX(float x0=0.0, float y0=0.0,float z0=0.0, int rad=0, int H=0, float R=0.0,float G=0.0,float B=0.0,float A=1.0,bool on=true):Solid::Solid(x0,y0,z0, R, G, B, A, on),radius(rad),h(H){};
	void sculpt(Canvas &c) const;
	inline float getMaxX() const {  return (x + h);}
	inline float getMaxY() const {  return (y + radius);}
	inline float getMaxZ() const {  return (z + radius);}
	inline float getMinX() const {  return (x);}
	inline float getMinY() const {  return (y - radius);}
	inline float getMinZ() const {  return (z - radius);}
};
class Ellipsoid : public Solid
{
private:
	float rx, ry, rz;
public:
	inline Ellipsoid(float x0=0.0, float y0=0.0,float z0=0.0, float RX=0, float RY=0, float RZ=0,float R=0.0,float G=0.0,float B=0.0,float A=1.0,bool on=true):Solid::Solid(x0,y0,z0, R, G, B, A, on),rx(RX), ry(RY), rz(RZ){};
	void sculpt(Canvas &c) const;
	inline float getMaxX() const {  return (x + rx);}
	inline float getMaxY() const {  return (y + ry);}
	inline float getMaxZ() const {  return (z + rz);}
	inline float getMinX() const {  return (x - rx);}
	inline float getMinY() const {  return (y - ry);}
	inline float getMinZ() const {  return (z - rz);}

};


typedef list <pSolid>::iterator Iter;
class Sculptor : public Canvas
{
private:
    //int Xmin, Xmax, Ymin, Ymax, Zmin, Zmax;
    list<pSolid> s;
    float r, g, b, a;
public:
	inline Sculptor(float R=0.0, float G=0.0, float B=0.0, float alpha=1.0) : Canvas::Canvas(), r(R), g(G), b(B), a(alpha) {}
    inline void setColor8(float R, float G, float B, float A){r=(R+1)/257; g=(G+1)/257; b=(B+1)/257; A=a;}//set color já com os valores normatizados
    inline void setColor(float R, float G, float B, float A){r=R; g=G; b=B; A=a;}//set color cru
    inline void putVoxel(int X, int Y, int Z){putBox(X, X+1, Y, Y+1, Z, Z+1);}
    inline void cutVoxel(int X, int Y, int Z){cutBox(X, X+1, Y, Y+1, Z, Z+1);}
    void putBox(int X0, int X1,
                int Y0, int Y1,
                int Z0, int Z1);
    void cutBox(int X0, int X1,
                int Y0, int Y1,
                int Z0, int Z1);
    void putSphere(int Xc, int Yc, int Zc, int R);
    void cutSphere(int Xc, int Yc, int Zc, int R);
    void putCylinder(int Xc, int Yc, int Zc, int R,int H);
    void cutCylinder(int Xc, int Yc, int Zc, int R, int H);
    void putCylinderX(int Xc, int Yc, int Zc, int R,int H);
    void cutCylinderX(int Xc, int Yc, int Zc, int R, int H);
    void putEllipsoid(int Xc, int Yc, int Zc,
                      int Rx, int Ry, int Rz);
    void cutEllipsoid(int Xc, int Yc, int Zc,
                      int Rx, int Ry, int Rz);
    void putPrism(int Xc, int Yc, int Zc,int X2,int Z2,int X3, int H);//Coloca um prisma 
    void cutPrism(int Xc, int Yc, int Zc,int X2,int Z2,int X3, int H);
    void cleanVoxels(void);
    void write(const char *Arq);
    float getMaxX();
	float getMaxY();
	float getMaxZ();
	float getMinX();
	float getMinY();
	float getMinZ();
};

#endif
