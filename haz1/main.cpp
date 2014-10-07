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
	Vector operator+(const Vector& v) {
		return Vector(x + v.x, y + v.y, z + v.z);
	}
	Vector operator-(const Vector& v) {
		return Vector(x - v.x, y - v.y, z - v.z);
	}
	float operator*(const Vector& v) { 	// dot product
		return (x * v.x + y * v.y + z * v.z);
	}
	Vector operator%(const Vector& v) { 	// cross product
		return Vector(y*v.z-z*v.y, z*v.x - x*v.z, x*v.y - y*v.x);
	}
	float Length() {
		return sqrt(x * x + y * y + z * z);
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
	Color operator*(const Color& c) {
		return Color(r * c.r, g * c.g, b * c.b);
	}
	Color operator+(const Color& c) {
		return Color(r + c.r, g + c.g, b + c.b);
	}
};



const int screenWidth = 600;	// alkalmazás ablak felbontása
const int screenHeight = 600;
const int camHeight = 58;
const int camWidth = 68;

bool lastclick=false;
float lastx=0;
float lasty=0;

Color image[screenWidth*screenHeight];	// egy alkalmazás ablaknyi kép
int pointnum=0;
Vector controlPoints[10];
float time[10];


Vector convCoords(int x,int y)
{
	Vector ret;
	float divx=(float)screenWidth/(float)camWidth;
	float divy=(float)screenHeight/(float)camHeight;
	ret.x=x/divx;
	ret.y=(screenHeight-y)/divy;
	return ret;
}

void drawCircles()
{
	float r=2.0f;
	for (int i=0 ; i<pointnum ; i++ ) {


		glBegin(GL_TRIANGLE_FAN);
		glColor3f(0.0,0.0,0.0);
		glVertex2f(controlPoints[i].x,controlPoints[i].y);
		for(int j = 0; j <= 32; j++) {
			float ford = float(j) / 32 * r * M_PI;
			float px=controlPoints[i].x + r*cos(ford);
			float py=controlPoints[i].y + r*sin(ford);
			glVertex2f(px, py);
		}
		glEnd();
	}
}

void putConvex()
{
	if(pointnum>=2) {
		int minyPoint=0;
		for(int i=1; i<pointnum; i++) {
			if(controlPoints[i].y<controlPoints[minyPoint].y) {
				minyPoint=i;
			}
		}
		Vector* burok[11];
		int burokdb=1;
		burok[0]=&controlPoints[minyPoint];
		Vector* current=burok[0];
		bool alma=true;
		while(alma) {
			for(int i=0; i<pointnum; i++) {
				if(current!=&controlPoints[i]) {
					Vector v=controlPoints[i]-*current;
					float vsize=v.Length();
					v.x/=vsize;
					v.y/=vsize;
					Vector n=Vector(v.y,-v.x);
					bool hatar=true;
					for(int j=0; j<pointnum; j++) {
						if(j!=i) {
							float tav=n*(controlPoints[j]-controlPoints[i]);
							if(tav>0.001) {
								hatar=false;
							}
						}
					}
					if(hatar) {
						burok[burokdb++]=&controlPoints[i];
						current=&controlPoints[i];
						if(burok[0]==burok[burokdb-1]){
                            alma=false;
						}
						break;
					}
				}
			}
		}
		glColor3f(0.5f,1.0f,1.0f);
		glBegin(GL_POLYGON);
		for(int i=0; i<burokdb; i++) {
			glVertex2f(burok[i]->x,burok[i]->y);
		}
		glEnd();
	}
}

class CatmullRom
{
		Vector As[4];

		Vector A0(Vector x) {
			return x;
		}

		Vector A1(Vector v) {
			return v;
		}

		Vector A2(Vector xi, Vector xii, Vector vi, Vector vii, float ti, float tii) {
			return (xii-xi)*3*(1/pow(tii-ti,2.0))-(vii+vi*2)*(1/(tii-ti));
		}

		Vector A3(Vector xi, Vector xii, Vector vi, Vector vii, float ti, float tii) {
			return (xi-xii)*2*(1/pow(tii-ti,3.0))+(vii+vi)*(1/pow(tii-ti,2.0));
		}

		Vector V(int i) {
			return  ((controlPoints[i+1]-controlPoints[i])*(1/(time[i+1]-time[i]))+(controlPoints[i]-controlPoints[i-1])*(1/(time[i]-time[i-1])))*0.5;
		}

	public:

		Vector r(int i, float t) {
			Vector r=As[0]+As[1]*(t-time[i-1])+As[2]*pow(t-time[i-1],2.0)+As[3]*pow(t-time[i-1],3.0);
			return r;
		}

		void computeAs(int i) {
			Vector v;
			Vector vi;
			if(i==1) {
				v=Vector(0,0);
			} else {
				v=V(i-1);
			}
			if(i==pointnum-1) {
				vi=Vector(0,0);
			} else {
				vi=V(i);
			}

			As[0]=A0(controlPoints[i-1]);
			As[1]=A1(v);
			As[2]=A2(controlPoints[i-1],controlPoints[i],v,vi,time[i-1],time[i]);
			As[3]=A3(controlPoints[i-1],controlPoints[i],v,vi,time[i-1],time[i]);
		}
};

void drawCatmullRom()
{
	if(pointnum>=2) {
		CatmullRom cr=CatmullRom();
		int osztas=100;

		glColor3f(0.0,0.5,0.0);
		glBegin(GL_LINE_STRIP);
		glVertex2f(controlPoints[0].x,controlPoints[0].y);
		for(int i=1; i<pointnum; i++) {
			float t=time[i];
			float tdiff=(time[i]-time[i-1])/osztas;
			cr.computeAs(i);
			for(int j=0; j<osztas; j++) {
				t=time[i-1]+j*tdiff;
				Vector p=cr.r(i,t);
				//std::cout<<"i: "<<i<<" t: "<<t<<" x: "<<p.x<<" y: "<<p.y<<std::endl;
				glVertex2f(p.x,p.y);
			}
		}
		glEnd();
	}
}

class CatmullClark{

    Vector calcgreen(Vector v1, Vector v2){
        return v1+(v2-v1)*0.5;
    }

    Vector calcreds(){

    }

};

void drawCatmullClark(){

}

class BezierCurve
{
		float B(int i, float t) {
			int n = pointnum-1;
			float choose = 1;
			for(int j = 1; j <= i; j++) {
				choose *= (float)(n-j+1)/j;
			}
			return choose * pow(t, i) * pow(1-t, n-i);
		}
	public:
		Vector r(float t) {
			Vector rr(0, 0);
			for(int i = 0; i < pointnum; i++) {
				rr = rr + controlPoints[i] * B(i,t);
			}
			return rr;
		}
};

void drawBezier()
{
	if(pointnum>=2) {
		glColor3f(1.0,0.0,0.0);
		int koz=1000;
		Vector endPoint;
		BezierCurve bc=BezierCurve();
		glBegin(GL_LINE_STRIP);
		for(int i=0; i<koz; i++) {
			float t=(float)i/(float)koz;
			endPoint=bc.r(t);
			glVertex2f(endPoint.x,endPoint.y);
		}
		glEnd();
	}
}

// Inicializacio, a program futasanak kezdeten, az OpenGL kontextus letrehozasa utan hivodik meg (ld. main() fv.)
void onInitialization( )
{
	glViewport(0, 0, screenWidth, screenHeight);



}

// Rajzolas, ha az alkalmazas ablak ervenytelenne valik, akkor ez a fuggveny hivodik meg
void onDisplay( )
{
	glClearColor(0.8f, 0.8f, 0.8f, 0.0f);		// torlesi szin beallitasa
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // kepernyo torles


	if(lastclick) {
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glTranslatef(-1.0,-1.0,0);
		glRotatef(0,0,0,0);
		glScalef(2*1/(float)camWidth,2*1/(float)camHeight,1);
		putConvex();
		drawBezier();
		drawCatmullRom();
		drawCircles();

	}

	glutSwapBuffers();     				// Buffercsere: rajzolas vege

}

// Billentyuzet esemenyeket lekezelo fuggveny (lenyomas)
void onKeyboard(unsigned char key, int x, int y)
{
	if (key == 'd') glutPostRedisplay( ); 		// d beture rajzold ujra a kepet

}

// Billentyuzet esemenyeket lekezelo fuggveny (felengedes)
void onKeyboardUp(unsigned char key, int x, int y)
{

}

// Eger esemenyeket lekezelo fuggveny
void onMouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {  // A GLUT_LEFT_BUTTON / GLUT_RIGHT_BUTTON illetve GLUT_DOWN / GLUT_UP
		if(pointnum<10) {
			controlPoints[pointnum]=convCoords(x,y);
			time[pointnum]=glutGet(GLUT_ELAPSED_TIME)/100;
			pointnum++;
		}
		lastclick=true;
		glutPostRedisplay( ); 						 // Ilyenkor rajzold ujra a kepet
	}
}



// Eger mozgast lekezelo fuggveny
void onMouseMotion(int x, int y)
{

}

// `Idle' esemenykezelo, jelzi, hogy az ido telik, az Idle esemenyek frekvenciajara csak a 0 a garantalt minimalis ertek
void onIdle( )
{
	long time = glutGet(GLUT_ELAPSED_TIME);		// program inditasa ota eltelt ido

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

