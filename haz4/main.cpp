//=============================================================================================
// Szamitogepes grafika hazi feladat keret. Ervenyes 2014-tol.
// A //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// sorokon beluli reszben celszeru garazdalkodni, mert a tobbit ugyis toroljuk.
// A beadott program csak ebben a fajlban lehet, a fajl 1 byte-os ASCII karaktereket tartalmazhat.
// Tilos:
// - mast "beincludolni", illetve mas konyvtarat hasznalni
// - faljmuveleteket vegezni (printf is fajlmuvelet!)
// - new operatort hivni az onInitialization függvényt kivéve, a lefoglalt adat korrekt felszabadítása nélkül
// - felesleges programsorokat a beadott programban hagyni
// - tovabbi kommenteket a beadott programba irni a forrasmegjelolest kommentjeit kiveve
// ---------------------------------------------------------------------------------------------
// A feladatot ANSI C++ nyelvu forditoprogrammal ellenorizzuk, a Visual Studio-hoz kepesti elteresekrol
// es a leggyakoribb hibakrol (pl. ideiglenes objektumot nem lehet referencia tipusnak ertekul adni)
// a hazibeado portal ad egy osszefoglalot.
// ---------------------------------------------------------------------------------------------
// A feladatmegoldasokban csak olyan gl/glu/glut fuggvenyek hasznalhatok, amelyek
// 1. Az oran a feladatkiadasig elhangzottak ES (logikai AND muvelet)
// 2. Az alabbi listaban szerepelnek:
// Rendering pass: glBegin, glVertex[2|3]f, glColor3f, glNormal3f, glTexCoord2f, glEnd, glDrawPixels
// Transzformaciok: glViewport, glMatrixMode, glLoadIdentity, glMultMatrixf, gluOrtho2D,
// glTranslatef, glRotatef, glScalef, gluLookAt, gluPerspective, glPushMatrix, glPopMatrix,
// Illuminacio: glMaterialfv, glMaterialfv, glMaterialf, glLightfv
// Texturazas: glGenTextures, glBindTexture, glTexParameteri, glTexImage2D, glTexEnvi,
// Pipeline vezerles: glShadeModel, glEnable/Disable a kovetkezokre:
// GL_LIGHTING, GL_NORMALIZE, GL_DEPTH_TEST, GL_CULL_FACE, GL_TEXTURE_2D, GL_BLEND, GL_LIGHT[0..7]
//
// NYILATKOZAT
// ---------------------------------------------------------------------------------------------
// Nev    : BODOLAI DOROTTYA EVA
// Neptun : DDFKC4
// ---------------------------------------------------------------------------------------------
// ezennel kijelentem, hogy a feladatot magam keszitettem, es ha barmilyen segitseget igenybe vettem vagy
// mas szellemi termeket felhasznaltam, akkor a forrast es az atvett reszt kommentekben egyertelmuen jeloltem.
// A forrasmegjeloles kotelme vonatkozik az eloadas foliakat es a targy oktatoi, illetve a
// grafhazi doktor tanacsait kiveve barmilyen csatornan (szoban, irasban, Interneten, stb.) erkezo minden egyeb
// informaciora (keplet, program, algoritmus, stb.). Kijelentem, hogy a forrasmegjelolessel atvett reszeket is ertem,
// azok helyessegere matematikai bizonyitast tudok adni. Tisztaban vagyok azzal, hogy az atvett reszek nem szamitanak
// a sajat kontribucioba, igy a feladat elfogadasarol a tobbi resz mennyisege es minosege alapjan szuletik dontes.
// Tudomasul veszem, hogy a forrasmegjeloles kotelmenek megsertese eseten a hazifeladatra adhato pontokat
// negativ elojellel szamoljak el es ezzel parhuzamosan eljaras is indul velem szemben.
//=============================================================================================

#define _USE_MATH_DEFINES
#include <math.h>
#include <stdlib.h>

#if defined(__APPLE__)
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Innentol modosithatod...
#include <iostream>

struct Matrix {
	float m[9];
	Matrix() {}
	Matrix(float *f, int s=9) {
		for(int i=0; i<s; i++) {
			m[i]=f[i];
		}
	}

	Matrix identitiy() {
		float f[9]= {1,0,0,0,1,0,0,0,1};
		Matrix a(f,9);
		return a;
	}

	Matrix operator+(const Matrix &mx) {
		float a[9];
		for(int i=0; i<9; i++) {
			a[i]=m[i]+mx.m[i];
		}
		return Matrix(a,9);
	}

	Matrix operator*(float f) {
		float a[9];
		for(int i=0; i<9; i++) {
			a[i]=m[i]*f;
		}
		return Matrix(a,9);
	}

};


//--------------------------------------------------------
// 3D Vektor
//--------------------------------------------------------

struct Vector {
	float x, y, z;

	Vector( ) {
		x = y = z = 0;
	}
	Vector(float x0, float y0, float z0 = 0) {
		x = x0;
		y = y0;
		z = z0;
	}
	Vector operator*(float a) {
		return Vector(x * a, y * a, z * a);
	}
	Vector operator/(float a) {
		return Vector(x / a, y / a, z / a);
	}
	Vector operator+(const Vector &v) {
		return Vector(x + v.x, y + v.y, z + v.z);
	}
	Vector operator-(const Vector &v) {
		return Vector(x - v.x, y - v.y, z - v.z);
	}
	float operator*(const Vector &v) { 	// dot product
		return (x * v.x + y * v.y + z * v.z);
	}

	float operator/(const Vector &v) {
		return (x / v.x + y / v.y + z / v.z);
	}

	Vector operator%(const Vector &v) { 	// cross product
		return Vector(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
	}
	float Length() {
		return sqrt(x * x + y * y + z * z);
	}
	Vector normalize() {
		return (*this) * (1.0f / Length());
	}
	Vector operator*(Matrix m) {
		return Vector(x*m.m[0]+y*m.m[3]+z*m.m[6],x*m.m[1]+y*m.m[4]+z*m.m[7],x*m.m[2]+y*m.m[5]+z*m.m[8]);
	}

	void operator+=(const Vector &v) {
		*this=*this+v;
	}

};

//--------------------------------------------------------
// Spektrum illetve szin
//--------------------------------------------------------
struct Color {
	float r, g, b;

	Color( ) {
		r = g = b = 0;
	}
	Color(float r0, float g0, float b0) {
		r = r0;
		g = g0;
		b = b0;
	}
	Color operator*(float a) {
		return Color(r * a, g * a, b * a);
	}
	Color operator*(const Color &c) {
		return Color(r * c.r, g * c.g, b * c.b);
	}
	Color operator+(const Color &c) {
		return Color(r + c.r, g + c.g, b + c.b);
	}

	Color operator/(float a) {
		return Color(r/a, g/a, b/a);
	}
};

const int screenWidth = 600;
const int screenHeight = 600;
const int NTESS = 100;
bool line = false;
const int TSIZE = 512;


void glVertex3fx(Vector v)
{
	glVertex3f(v.x, v.y, v.z);
}

struct Object {
	Vector center;
	virtual void draw() = 0;
};

struct Light {
	GLenum id;
	Vector pos;
	Color Ld, La, Ls;

	Light() {}

	Light(GLenum i, Vector p, Color d, Color a, Color s) {
		id = i;
		pos = p;
		Ld = d;
		La = a;
		Ls = s;
	}

	void setOGL() {
		float ppos[4] = {pos.x, pos.y, pos.z, 1};
		float ld[4] = {Ld.r, Ld.g, Ld.b, 0};
		float la[4] = {La.r, La.g, La.b, 0};
		float ls[4] = {Ls.r, Ls.g, Ls.b, 0};
		glLightfv(id, GL_POSITION, ppos);
		glLightfv(id, GL_DIFFUSE, ld);
		glLightfv(id, GL_AMBIENT, la);
		glLightfv(id, GL_SPECULAR, ls);
		glLightf(id, GL_CONSTANT_ATTENUATION, 0.0f);
		glLightf(id, GL_QUADRATIC_ATTENUATION, 0.1f);
		glEnable(id);
	}
};

struct Camera {

	Vector eye, lookat, vup;
	float fov, asp, fp, bp;
	Vector sebesseg;
	float hossz;
	float maxhossz;
	float k;

	float suly;
	float sulyhajo;

	Camera() {}

	Camera(Vector e, Vector l, Vector u) {
		eye = e;
		lookat = l;
		vup = u;
		fov = 90;
		asp = 1;
		fp = 0.1;
		bp = 1000;

		sebesseg=Vector(0,0.001,0.001);
		maxhossz=5;
		hossz=0;
		k=0.001;
		suly=0.01;
		sulyhajo=1;
	}

	void setOGL() {

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity( );
		gluPerspective(fov, asp, fp, bp);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity( );
		gluLookAt(eye.x, eye.y, eye.z, lookat.x, lookat.y, lookat.z, vup.x, vup.y, vup.z);
	}

	void moveIT(float koz);
};


struct Material {
	Color kd;
	Color ks;
	Color ka;
	int shininess;
	bool blended;
	Material() {}

	Material(Color d, Color s, Color a, int n, bool bl = false) {
		kd = d;
		ks = s;
		ka = a;
		shininess = n;
		blended = bl;
	}

	void setOGL() {
		float d[] = {kd.r, kd.g, kd.b, 1.0};
		float s[] = {ks.r, ks.g, ks.b, 1.0};
		float a[] = {ka.r, ka.g, ka.b, 1.0};

		if(blended) {
			d[3] = s[3] = a[3] = 0.2;
		}

		glMaterialfv( GL_FRONT, GL_DIFFUSE, d);
		glMaterialfv( GL_FRONT, GL_SPECULAR, s);
		glMaterialfv( GL_FRONT, GL_AMBIENT, a);
		glMaterialf( GL_FRONT, GL_SHININESS, shininess);

	}

	void changeAll(Color c) {
		ka=kd=ks=c;
	}
};


struct Texture {

	unsigned int text_id;
	unsigned char tex[TSIZE][TSIZE][3];

	void genPlanetText() {
		for(int i = 0; i < TSIZE; i++) {
			for(int j = 0; j < TSIZE; j++) {
				if(j % 4 == 0 && i % 4 == 0) {
					tex[i][j][0] = 0;
					tex[i][j][1] = 128;
					tex[i][j][2] = 0;
				} else {
					tex[i][j][0] = 255;
					tex[i][j][1] = 128;
					tex[i][j][2] = 0;
				}
			}
		}
	}

	void genSkyText() {
		for(int i = 0; i < TSIZE; i++) {
			for(int j = 0; j < TSIZE; j++) {
				int star = rand() % 100;
				if(!star) {
					tex[i][j][0] = 0;
					tex[i][j][1] = 255*0.7;
					tex[i][j][2] = 255*0.7;
				} else {
					tex[i][j][0] = 0;
					tex[i][j][1] = 0;
					tex[i][j][2] = 0;
				}
			}
		}
	}

	void setOGL() {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, TSIZE, TSIZE, 0, GL_RGB, GL_UNSIGNED_BYTE, tex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	}

};



struct ParamObject : public Object {
	Material *mat;
	Texture *tex;
	bool hasMater;
	bool hasText;

	virtual Vector getNormal(float x, float y, float z) = 0;

	void draw() {
		if(hasMater) {
			glEnable(GL_LIGHTING);
			mat->setOGL();
		} else {
			glDisable(GL_LIGHTING);
		}

		if(hasText) {
			glEnable(GL_TEXTURE_2D);
			tex->setOGL();
		} else {
			glDisable(GL_TEXTURE_2D);
		}
		if(line) {
			glBegin(GL_LINE_STRIP);
		} else {
			glBegin(GL_QUADS);
		}
		for(int i = 0; i < NTESS; i++) {
			for(int j = 0; j < NTESS; j++) {
				VertexOGL( (float)i / NTESS, (float)j / NTESS );
				VertexOGL( (float)(i + 1) / NTESS, (float)j / NTESS );
				VertexOGL( (float)(i + 1) / NTESS, (float)(j + 1) / NTESS );
				VertexOGL( (float)i / NTESS, (float)(j + 1) / NTESS );
			}
		}
		glEnd();
	}
	virtual void VertexOGL(float u, float v) = 0;
	virtual ~ParamObject() {}
};


//-------------------------------------
//1. hazifeladatban hasznalt osztaly alapjan

struct CatmullRom {
	float time[10];
	Vector controlPoints[10];
	int pointnum;

	Vector As[4];


	CatmullRom() {
		pointnum = 10;
		for(int i = 0; i < pointnum; i++) {
			time[i] = (float)i / (float)pointnum;
		}


		controlPoints[0] = Vector(0, 0, 0);
		controlPoints[1] = Vector(0.5, 0, 0.5);
		controlPoints[2] = Vector(1, 0, 1);
		controlPoints[3] = Vector(1.3, 0, 1.5);
		controlPoints[4] = Vector(1.2, 0, 2);
		controlPoints[5] = Vector(0.9, 0, 2.5);
		controlPoints[6] = Vector(0.8, 0, 3);
		controlPoints[7] = Vector(0.2, 0, 3.5);
		controlPoints[8] = Vector(0.5, 0, 4);
		controlPoints[9] = Vector(0, 0, 4.5);



	}

	Vector A0(Vector x) {
		return x;
	}

	Vector A1(Vector v) {
		return v;
	}

	Vector A2(Vector xi, Vector xii, Vector vi, Vector vii, float ti, float tii) {
		return (xii - xi) * 3 * (1.0 / pow(tii - ti, 2.0)) - (vii + vi * 2) * (1.0 / (tii - ti));
	}

	Vector A3(Vector xi, Vector xii, Vector vi, Vector vii, float ti, float tii) {
		return (xi - xii) * 2 * (1.0 / pow(tii - ti, 3.0)) + (vii + vi) * (1.0 / pow(tii - ti, 2.0));
	}

	Vector V(int i) {
		return  ((controlPoints[i+1] - controlPoints[i]) * (1.0 / (time[i+1] - time[i])) + (controlPoints[i] - controlPoints[i-1]) * (1.0 / (time[i] - time[i-1]))) * 0.5;
	}

	Vector r(int i, float t) {
		Vector r = As[0] + As[1] * (t - time[i-1]) + As[2] * pow(t - time[i-1], 2.0) + As[3] * pow(t - time[i-1], 3.0);
		return r;
	}

	void computeAs(int i) {
		Vector v;
		Vector vi;
		if(i == 1) {
			v = Vector(0, 0);
		} else {
			v = V(i - 1);
		}
		if(i == pointnum - 1) {
			vi = Vector(0, 0);
		} else {
			vi = V(i);
		}


		As[0] = A0(controlPoints[i-1]);
		As[1] = A1(v);
		As[2] = A2(controlPoints[i-1], controlPoints[i], v, vi, time[i-1], time[i]);
		As[3] = A3(controlPoints[i-1], controlPoints[i], v, vi, time[i-1], time[i]);
	}

	Vector deriveR(int i, float t) {
		Vector dr = As[1] + As[2] * (t - time[i-1]) * 2 + As[3] * pow(t - time[i-1], 2.0) * 3;
		return dr;

	}


};


//-------------------------------------

struct CRTest : public ParamObject {

	CatmullRom cr;


	CRTest(Material *m, Texture *t = NULL, Vector c = Vector(0, 0, 0)) {
		center = c;
		if(m) {
			mat = m;
			hasMater = true;
		} else {
			hasMater = false;
		}
		if(t) {
			hasText = true;
			tex = t;
		} else {
			hasText = false;
		}
	}

	Vector getNormal(float x, float y, float z) {
		return Vector();
	}

	Vector getNormal(float x, float y, float z, int i, float t) {
		Vector dr = cr.deriveR(i, t);
		Vector meroleges = Vector(x, y, 0);
		Vector v2 = meroleges % dr;
		Vector n = dr % v2;
		return n;
	}


	void VertexOGL(float u, float v) {
		int ind = -1;
		for(int i = 1; i < cr.pointnum; i++) {
			if(u >= cr.time[i-1] && u <= cr.time[i]) {
				ind = i;
			}
		}
		if(ind == -1) {
			ind = cr.pointnum - 1;
			u = cr.time[ind];
			return;
		}

		cr.computeAs(ind);


		float t = u;
		Vector p = cr.r(ind, t);
		float R = p.x;

		float z = p.z;
		float x = R * cos(2 * M_PI * v);
		float y = R * sin(2 * M_PI * v);

		Vector n = getNormal(x, y, z, ind, t);

		if(y < 0 && ((x - 0)*(x - 0) + (z - 1.5)*(z - 1.5)) < 0.5 * 0.5) {
			return;
		}

		glNormal3f(n.x, n.y, n.z);


		glVertex3fx(Vector(x, y, z));

	}

	void draw() {
		if(hasMater) {
			glEnable(GL_LIGHTING);
			mat->setOGL();
		} else {
			glDisable(GL_LIGHTING);
		}

		if(hasText) {
			glEnable(GL_TEXTURE_2D);
			tex->setOGL();
		} else {
			glDisable(GL_TEXTURE_2D);
		}
		for(int i = 0; i < NTESS; i++) {
			for(int j = 0; j < NTESS; j++) {
				if(line) {
					glBegin(GL_LINE_STRIP);
				} else {
					glBegin(GL_QUADS);
				}
				VertexOGL( (float)i / NTESS, (float)j / NTESS );
				VertexOGL( (float)(i + 1) / NTESS, (float)j / NTESS );
				VertexOGL( (float)(i + 1) / NTESS, (float)(j + 1) / NTESS );
				VertexOGL( (float)i / NTESS, (float)(j + 1) / NTESS );
				glEnd();
			}
		}
	}

};


struct Gomb : public ParamObject {
	float r;
	float fordulas;

	Gomb(Vector c, Material *m, float rx, Texture *t = NULL) {
		fordulas=0;
		if(m) {
			mat = m;
			hasMater = true;
		} else {
			hasMater = false;
		}
		if(t) {
			hasText = true;
			tex = t;
		} else {
			hasText = false;
		}
		r = rx;
		center = c;
	}

	Vector getNormal(float x, float y, float z) {
		Vector n(x, y, z);
		return n;
	}

	void VertexOGL(float u, float v) {
		glTexCoord2f(v, u);
		float x = r * sin(u * M_PI) * cos(2 * M_PI * v);
		float y = r * sin(u * M_PI) * sin(2 * M_PI * v);
		float z = r * cos(u * M_PI);

		Vector n = getNormal(x, y, z);

		glNormal3f(n.x, n.y, n.z);
		glVertex3f(x, y, z);

	}

};

struct Henger : public ParamObject {

	float h;
	Vector irany;
	float R;

	Henger(Material *m, Vector c, float hx, float r, Vector i) {
		center = c;
		h = hx;
		irany = i.normalize();
		mat = m;
		R = r;
		if(m) {
			mat = m;
			hasMater = true;
		} else {
			hasMater = false;
		}
		hasText = false;
	}

	Vector getNormal(float x, float y, float z) {
		return Vector(x, y, 0);
	}

	void VertexOGL(float u, float v) {
		float x = R * cos(2 * M_PI * u);
		float y = R * sin(2 * M_PI * u);
		float z = h * v;

		Vector n = getNormal(x, y, z);
		glNormal3f(n.x, n.y, n.z);
		glVertex3f(x, y, z);
	}

};

struct Teglatest : public ParamObject {

	float A;
	float B;
	float C;
	Vector sarok[8];

	Teglatest(Material *m, Vector c, float ax, float bx, float cx, Texture *t = NULL ) {
		A = ax;
		B = bx;
		C = cx;
		center = c;

		if(m) {
			mat = m;
			hasMater = true;
		} else {
			hasMater = false;
		}
		if(t) {
			hasText = true;
			tex = t;
		} else {
			hasText = false;
		}

		sarok[0] = Vector(center.x - A / 2, center.y - B / 2, center.z - C / 2);
		sarok[1] = Vector(center.x - A / 2, center.y + B / 2, center.z - C / 2);
		sarok[2] = Vector(center.x - A / 2, center.y + B / 2, center.z + C / 2);
		sarok[3] = Vector(center.x - A / 2, center.y - B / 2, center.z + C / 2);

		sarok[4] = Vector(center.x + A / 2, center.y - B / 2, center.z - C / 2);
		sarok[5] = Vector(center.x + A / 2, center.y + B / 2, center.z - C / 2);
		sarok[6] = Vector(center.x + A / 2, center.y + B / 2, center.z + C / 2);
		sarok[7] = Vector(center.x + A / 2, center.y - B / 2, center.z + C / 2);

	}

	void draw() {
		if(hasMater) {
			glEnable(GL_LIGHTING);
			mat->setOGL();
		} else {
			glDisable(GL_LIGHTING);
		}

		if(hasText) {
			glEnable(GL_TEXTURE_2D);
			tex->setOGL();
		} else {
			glDisable(GL_TEXTURE_2D);
		}

		if(line) {
			glBegin(GL_LINE_STRIP);
		} else {
			glBegin(GL_QUADS);
		}

		for(int i = 0; i < 4; i++) {
			glNormal3f(-1,0,0);
			glVertex3fx(sarok[i]);
		}
		for(int i = 4; i < 8; i++) {
			glNormal3f(1,0,0);
			glVertex3fx(sarok[i]);
		}

		glNormal3f(0,0,-1);
		glVertex3fx(sarok[0]);
		glVertex3fx(sarok[1]);
		glVertex3fx(sarok[5]);
		glVertex3fx(sarok[4]);

		glNormal3f(0,0,1);
		glVertex3fx(sarok[3]);
		glVertex3fx(sarok[2]);
		glVertex3fx(sarok[6]);
		glVertex3fx(sarok[7]);

		glNormal3f(0,-1,0);
		glVertex3fx(sarok[0]);
		glVertex3fx(sarok[4]);
		glVertex3fx(sarok[7]);
		glVertex3fx(sarok[3]);

		glNormal3f(0,1,0);
		glVertex3fx(sarok[1]);
		glVertex3fx(sarok[5]);
		glVertex3fx(sarok[6]);
		glVertex3fx(sarok[2]);



		glEnd();
	}

	Vector getNormal(float x, float y, float z) {
		return Vector();
	}

	void VertexOGL(float u, float v) {}
};


struct MIR {
	CRTest *test;
	Teglatest *napelemek[2];
	Vector center;
	float nh;
	Material crMaterial;
	Material neMaterial;
	float ford;
	Vector fordTengely;
	float kering;
	float w;
	float T;
	Vector v;
	Vector acp;

	MIR(Vector c, float napelemhossz = 1.5) {
		center = c;
		nh = napelemhossz;
		ford=0;
		fordTengely=Vector(1,1,0).normalize();
		kering=0;
		w=360.0/300000.0;
		T=2*M_PI/w;
		v=acp=Vector(0,0,0);
	}

	void build() {


		crMaterial = Material(Color(1, 1, 0), Color(1, 1, 1), Color(0.2, 0, 0), 100);
		test = new CRTest(&crMaterial, NULL, center);
		Vector napelemCenter1 = test->cr.controlPoints[test->cr.pointnum-1] + Vector(nh / 2, 0, -0.5);
		Vector napelemCenter2 = test->cr.controlPoints[test->cr.pointnum-1] - Vector(nh / 2, 0, 0.5);
		neMaterial=Material(Color(0.7,0.7,0.7), Color(0.5,0.5,0.5),Color(0.2,0.2,0.2),10);
		napelemek[0] = new Teglatest(&neMaterial, napelemCenter1, nh, 0.5, 0.1);
		napelemek[1] = new Teglatest(&neMaterial, napelemCenter2, nh, 0.5, 0.1);
	}

	void draw() {

		glPushMatrix();
		glMatrixMode(GL_MODELVIEW);
		glTranslatef(test->center.x, test->center.y, test->center.z);
		glRotatef(ford,fordTengely.x,fordTengely.y,fordTengely.z);
		glTranslatef(0, 0, -2);
		test->draw();

		napelemek[0]->draw();
		napelemek[1]->draw();

		glPopMatrix();
	}



	void moveIT(Vector planet,float time) {
		float tav=(center-planet).Length();
		v=(Vector(0,0,-1)*w)%(Vector(center-planet)*tav);
		acp=(planet-center)*(v*v/tav);
		kering+=time*w;
		center.x=planet.x-tav*sin(kering/(180)*M_PI);
		center.y=planet.y-tav*cos(kering/(180)*M_PI);
		test->center=center;
	}

	~MIR() {
		delete test;
		delete[] napelemek;
	}
};


struct Muhold {
	Gomb *test;
	Henger *fuvokak[6];
	Vector center;
	float r;
	Material testm;
	Material fuvokam;
	Gomb *tuz[6];
	bool validTuz[6];
	float kering;
	float ford;
	Vector fordTengely;
	Vector sebesseg;
	Vector sebeltolas;
	float w;
	float T;
	Vector v;
	Vector acp;


	Muhold(Vector c, float R) {
		center = c;
		r = R;

		for(int i=0; i<6; i++) {
			tuz[i]=new Gomb(Vector(0,0,0),NULL,0);
			validTuz[i]=false;
		}
		kering=0;
		ford=0;
		fordTengely=Vector(1,0,0);
		sebesseg=Vector(0,0,0);
		sebeltolas=Vector(0,0,0);
		w=360.0/300000.0;
		T=2*M_PI/w;
		v=acp=Vector(0,0,0);
	}

	void build() {
		testm = Material(Color(0.5, 0.5, 0.5), Color(0.2, 0.2, 0.2), Color(0, 0, 0.1), 100);
		fuvokam = Material(Color(1, 0, 0), Color(1, 0, 0), Color(1, 0, 0), 10);
		test = new Gomb(center, &testm , r, NULL);

		fuvokak[0] = new Henger(&fuvokam, Vector(center.x + r, center.y, center.z), r / 2, r / 10, Vector(r, 0, 0));
		fuvokak[1] = new Henger(&fuvokam, Vector(center.x - r, center.y, center.z), r / 2, r / 10, Vector(-r, 0, 0));

		fuvokak[2] = new Henger(&fuvokam, Vector(center.x, center.y + r, center.z), r / 2, r / 10, Vector(0, r, 0));
		fuvokak[3] = new Henger(&fuvokam, Vector(center.x, center.y - r, center.z), r / 2, r / 10, Vector(0, -r, 0));

		fuvokak[4] = new Henger(&fuvokam, Vector(center.x, center.y, center.z + r), r / 2, r / 10, Vector(0, 0, r));
		fuvokak[5] = new Henger(&fuvokam, Vector(center.x, center.y, center.z - r), r / 2, r / 10, Vector(0, 0, -r));


	}

	void draw() {
		glPushMatrix();
		glMatrixMode(GL_MODELVIEW);
		glTranslatef(center.x,center.y,center.z);
		glRotatef(ford,fordTengely.x,fordTengely.y,fordTengely.z);
		test->draw();


		for(int i = 0; i < 6; i++) {
			Vector c = fuvokak[i]->center-center;
			glPushMatrix();

			glMatrixMode(GL_MODELVIEW);
			glTranslatef(c.x, c.y, c.z);

			switch(i) {
				case 0:
					glRotatef(90, 0, 1, 0);
					fuvokam = Material(Color(1, 0, 0), Color(0.5, 0, 0), Color(0.1, 0, 0), 10);
					break;
				case 1:
					glRotatef(-90, 0, 1, 0);
					break;
				case 2:
					glRotatef(-90, 1, 0, 0);
					fuvokam= Material(Color(0, 1, 0), Color(0, 0.5, 0), Color(0, 0.1, 0), 10);
					break;
				case 3:
					glRotatef(90, 1, 0, 0);
					break;
				case 4:
					glRotatef(0, 1, 0, 0);
					fuvokam= Material(Color(0, 0, 1), Color(0, 0, 0.5), Color(0, 0, 0.1), 10);
					break;
				case 5:
					glRotatef(180, 0, 1, 0);
					break;
			}

			fuvokak[i]->draw();
			glPopMatrix();
		}

		drawTuz();
		glPopMatrix();
	}

	void drawTuz() {
		for(int i=0; i<6; i++) {
			if(validTuz[i]) {
				glColor3f(1.0,0.5,0.0);
				Vector c=fuvokak[i]->center+fuvokak[i]->irany*fuvokak[i]->h;
				tuz[i]->center=c;
				glPushMatrix();
				glTranslatef(tuz[i]->center.x-center.x,tuz[i]->center.y-center.y,tuz[i]->center.z-center.z);
				tuz[i]->draw();
				glPopMatrix();
			}
		}
	}

	void addTuz(int pos) {
		Vector c=fuvokak[pos]->center+fuvokak[pos]->irany*fuvokak[pos]->h;
		tuz[pos]->center=c;
		tuz[pos]->r=fuvokak[pos]->R;
		validTuz[pos]=true;
	}

	void removeTuz(int pos) {
		validTuz[pos]=false;
	}

	void moveIT(Vector planet,float time) {
		float tav=(center-planet).Length();
		v=(Vector(0,0,-1)*w)%(Vector(center-planet)*tav);
		acp=(planet-center)*(v*v/tav);

		kering+=time*w;
		center.x=planet.x-tav*sin(kering/(180)*M_PI);
		center.y=planet.y-tav*cos(kering/(180)*M_PI);

		std::cout<<"sebesseg: "<<sebesseg.x<<" "<<sebesseg.y<<" "<<sebesseg.z<<std::endl;
		center+=sebesseg*time;
		test->center=center;

		fuvokak[0]->center=Vector(center.x + r, center.y, center.z);
		fuvokak[1]->center=Vector(center.x - r, center.y, center.z);

		fuvokak[2]->center=Vector(center.x, center.y + r, center.z);
		fuvokak[3]->center=Vector(center.x, center.y - r, center.z);

		fuvokak[4]->center=Vector(center.x, center.y, center.z + r);
		fuvokak[5]->center=Vector(center.x, center.y, center.z - r);
	}

	void setspeed(int pos, float seb) {
		Vector v=fuvokak[pos]->irany;

		float tomb[9];

		tomb[0]=1;
		tomb[1]=tomb[2]=tomb[3]=tomb[6]=0;
		tomb[4]=tomb[8]=cos(-ford/180.0*M_PI);
		tomb[5]=-1*sin(-ford/180.0*M_PI);
		tomb[7]=tomb[5]*-1;
		Matrix m=Matrix(tomb,9);
		v=v*m;
		Vector s=v.normalize()*seb;
		sebesseg+=s;
		std::cout<<"sebci "<<s.x<<" "<<s.y<<" "<<s.z<<"; ";
		std::cout<<"uj vektor: "<<v.x<<" "<<v.y<<" "<<v.z<<"; "<<sebesseg.x<<" "<<sebesseg.y<<" "<<sebesseg.z<<std::endl;
	}

	~Muhold() {
		delete test;
		delete[] fuvokak;
		delete[] tuz;
	}

};


struct Scene {
	Camera *camera;

	Gomb *Sky;
	Light *Sun;
	Gomb *sun;
	Gomb *planet;
	Gomb *Legkor;
	Muhold *muhold;
	MIR *mir;

	Material legkor;
	Material planetMaterial;
	Texture planetTexture;
	Texture skyTexture;

	long now;
	long lasttime;

	Scene() {
	}


	void build() {
		mir = new MIR(Vector(0, -30, 0), 2);

		camera = new Camera(Vector(0,-33,0), mir->center, Vector(0, 0, 1));

		Sun = new Light(GL_LIGHT0, Vector(40, -80, 15), Color(400, 400, 400), Color(400, 400, 400), Color(400, 400, 400));
		sun = new Gomb(Sun->pos, NULL, 1, NULL);

		Sky = new Gomb(Vector(0, 0, 0), NULL, 100, &skyTexture);

		glGenTextures(1, &(skyTexture.text_id));
		glBindTexture(GL_TEXTURE_2D, skyTexture.text_id);
		skyTexture.genSkyText();


		mir->build();
		muhold = new Muhold(Vector(0, -35, 0), 0.3);
		muhold->build();

		planetMaterial = Material(Color(1, 0.5, 0), Color(0, 0, 0), Color(0, 0, 0), 1);
		planet = new Gomb(Vector(0, 0, 0), &planetMaterial, 10, &planetTexture);


		glGenTextures(2, &(planetTexture.text_id));
		glBindTexture(GL_TEXTURE_2D, planetTexture.text_id);
		planetTexture.genPlanetText();


		legkor = Material(Color(0, 0, 1), Color(0, 0, 1), Color(0, 0, 1), 10, true);
		Legkor = new Gomb(planet->center, &legkor, planet->r + 0.5, NULL);
	}

	void moveALL() {
		float diff= now-lasttime;
		muhold->moveIT(planet->center,diff/10.0);


		float koz=0.01;
		diff=koz;
		for(float i=lasttime/10.0; i<now/10.0; i+=koz) {

			float fordP=diff*360.0/100000.0;
			planet->fordulas=fmod(planet->fordulas+fordP, 360);

			float fordMir=diff*360/10000.0;
			mir->ford=fmod(mir->ford+fordMir,360);


			float fordMuh=diff*360/10000.0;
			muhold->ford=fmod(muhold->ford+fordMuh,360);


			mir->moveIT(planet->center,diff);


			camera->moveIT(koz);
		}
		lasttime=now;
	}

	void render() {
		moveALL();
		camera->lookat=mir->center;

		camera->setOGL();
		Sun->setOGL();
		glColor3f(1, 1, 1);
		Sky->draw();

		glPushMatrix();
		glMatrixMode(GL_MODELVIEW);
		glTranslatef(planet->center.x,planet->center.y,planet->center.z);
		glRotatef(planet->fordulas,0,0,1);
		planet->draw();
		glPopMatrix();

		glPushMatrix();
		glMatrixMode(GL_MODELVIEW);
		glTranslatef(sun->center.x,sun->center.y,sun->center.z);
		glColor3f(1, 1, 0);
		sun->draw();
		glPopMatrix();

		glPushMatrix();
		glMatrixMode(GL_MODELVIEW);

		muhold->draw();
		glPopMatrix();

		glPushMatrix();
		glMatrixMode(GL_MODELVIEW);
		mir->draw();
		glPopMatrix();

//        glDisable(GL_LIGHTING);
//        glColor3f(1,1,1);
//        glBegin(GL_LINE_STRIP);
//        glVertex3fx(camera->eye-Vector(0,0,1));
//        glVertex3fx(camera->lookat);
//        glEnd();


		glPushMatrix();
		glMatrixMode(GL_MODELVIEW);
		glTranslatef(Legkor->center.x,Legkor->center.y,Legkor->center.z);

		glEnable(GL_CULL_FACE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		Legkor->draw();
		glDisable(GL_BLEND);
		glDisable(GL_CULL_FACE);

		glPopMatrix();
	}

	~Scene() {
		delete Sun;
		delete sun;
		delete planet;
		delete Legkor;
		delete Sky;
		delete camera;
		delete muhold;
		delete mir;
	}
};

Scene scene;
// Inicializacio, a program futasanak kezdeten, az OpenGL kontextus letrehozasa utan hivodik meg (ld. main() fv.)
void onInitialization( )
{
	glViewport(0, 0, screenWidth, screenHeight);
	glDisable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glShadeModel(GL_SMOOTH);


	scene = Scene();
	scene.lasttime=glutGet(GLUT_ELAPSED_TIME);
	scene.now=glutGet(GLUT_ELAPSED_TIME);

	scene.build();


}

// Rajzolas, ha az alkalmazas ablak ervenytelenne valik, akkor ez a fuggveny hivodik meg
void onDisplay( )
{
	glClearColor(1.0, 1.0, 1.0, 1.0f);		// torlesi szin beallitasa
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // kepernyo torles

	scene.render();
	glutSwapBuffers();     				// Buffercsere: rajzolas vege

}

// Billentyuzet esemenyeket lekezelo fuggveny (lenyomas)
void onKeyboard(unsigned char key, int x, int y)
{
	static float szorzo= -0.001;
//    if (key == 'd') glutPostRedisplay( ); 		// d beture rajzold ujra a kepet
	if (key == 'l') {
		line = !line;
		glutPostRedisplay( );
	}
	if (key == 'w') {
//		scene.camera->eye.z++;
		scene.muhold->addTuz(5);

		scene.muhold->setspeed(5,szorzo);
		glutPostRedisplay( );
	}
	if (key == 's') {
//		scene.camera->eye.z--;
		scene.muhold->addTuz(4);
		scene.muhold->setspeed(4, szorzo);

		glutPostRedisplay( );
	}
	if (key == 'a') {
//		scene.camera->eye.x--;
		scene.muhold->addTuz(0);
		scene.muhold->setspeed(0, szorzo);

		glutPostRedisplay( );
	}
	if (key == 'd') {
//		scene.camera->eye.x++;
		scene.muhold->addTuz(1);
		scene.muhold->setspeed(1, szorzo);

		glutPostRedisplay( );
	}

	if (key == 'q') {
//		scene.camera->eye.y++;
		scene.muhold->addTuz(2);
		scene.muhold->setspeed(2, szorzo);


		glutPostRedisplay( );
	}
	if (key == 'e') {
//		scene.camera->eye.y--;
		scene.muhold->addTuz(3);
		scene.muhold->setspeed(3, szorzo);

		glutPostRedisplay( );
	}

}

// Billentyuzet esemenyeket lekezelo fuggveny (felengedes)
void onKeyboardUp(unsigned char key, int x, int y)
{
	if (key == 'w') {
		scene.muhold->removeTuz(5);

		glutPostRedisplay( );
	}
	if (key == 's') {
		scene.muhold->removeTuz(4);

		glutPostRedisplay( );
	}
	if (key == 'a') {
		scene.muhold->removeTuz(0);

		glutPostRedisplay( );
	}
	if (key == 'd') {
		scene.muhold->removeTuz(1);
		glutPostRedisplay( );
	}

	if (key == 'q') {
		scene.muhold->removeTuz(2);
		glutPostRedisplay( );
	}
	if (key == 'e') {
		scene.muhold->removeTuz(3);
		glutPostRedisplay( );
	}
}

// Eger esemenyeket lekezelo fuggveny
void onMouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)   // A GLUT_LEFT_BUTTON / GLUT_RIGHT_BUTTON illetve GLUT_DOWN / GLUT_UP
		glutPostRedisplay( ); 						 // Ilyenkor rajzold ujra a kepet
}

// Eger mozgast lekezelo fuggveny
void onMouseMotion(int x, int y)
{

}

// `Idle' esemenykezelo, jelzi, hogy az ido telik, az Idle esemenyek frekvenciajara csak a 0 a garantalt minimalis ertek
void onIdle( )
{
	scene.now = glutGet(GLUT_ELAPSED_TIME);		// program inditasa ota eltelt ido


	glutPostRedisplay();
}
//------------------------------------------------------

void Camera::moveIT(float koz)
{

	hossz=(eye-scene.mir->center).Length();
	Vector force(0,0,0);
	if(hossz>=maxhossz) {
		Vector tav=scene.mir->center-eye;
		force+=(tav.normalize())*(hossz)*-k;

	}
	sebesseg=sebesseg-force/1.0*koz;
	eye+=sebesseg*koz;

}


// ...Idaig modosithatod
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// A C++ program belepesi pontja, a main fuggvenyt mar nem szabad bantani
int main(int argc, char **argv)
{
	glutInit(&argc, argv); 				// GLUT inicializalasa
	glutInitWindowSize(600, 600);			// Alkalmazas ablak kezdeti merete 600x600 pixel
	glutInitWindowPosition(100, 100);			// Az elozo alkalmazas ablakhoz kepest hol tunik fel
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);	// 8 bites R,G,B,A + dupla buffer + melyseg buffer

	glutCreateWindow("Grafika hazi feladat");		// Alkalmazas ablak megszuletik es megjelenik a kepernyon

	glMatrixMode(GL_MODELVIEW);				// A MODELVIEW transzformaciot egysegmatrixra inicializaljuk
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);			// A PROJECTION transzformaciot egysegmatrixra inicializaljuk
	glLoadIdentity();

	onInitialization();					// Az altalad irt inicializalast lefuttatjuk

	glutDisplayFunc(onDisplay);				// Esemenykezelok regisztralasa
	glutMouseFunc(onMouse);
	glutIdleFunc(onIdle);
	glutKeyboardFunc(onKeyboard);
	glutKeyboardUpFunc(onKeyboardUp);
	glutMotionFunc(onMouseMotion);

	glutMainLoop();					// Esemenykezelo hurok

	return 0;
}

