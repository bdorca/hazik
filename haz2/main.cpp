//=============================================================================================
// Szamitogepes grafika hazi feladat keret. Ervenyes 2014-tol.
// A //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// sorokon beluli reszben celszeru garazdalkodni, mert a tobbit ugyis toroljuk.
// A beadott program csak ebben a fajlban lehet, a fajl 1 byte-os ASCII karaktereket tartalmazhat.
// Tilos:
// - mast "beincludolni", illetve mas konyvtarat hasznalni
// - faljmuveleteket vegezni (printf is fajlmuvelet!)
// - new operatort hivni az onInitialization fuggvenyt kiveve, a lefoglalt adat korrekt felszabaditasa nelkul
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

//--------------------------------------------------------
// 3D Vektor
//--------------------------------------------------------
#include <iostream>
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
	Vector normalize() {
		return (*this)*(1.0f/Length());
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
const float epsilon=0.000001;
const int screenWidth = 600;	// alkalmazás ablak felbontása
const int screenHeight = 600;

struct Intersection {
	bool talalt;
	Vector pos, normal;
	int objectID;
	Intersection() {
		objectID=-1;
		talalt=false;
		pos=Vector(0,0);
		normal=Vector(0,0);
	}

	Intersection(Vector p, Vector n, bool t, int o=-1) {
		pos=p;
		normal=n;
		talalt=t;
		objectID=o;
	}
};


struct Ray {
	Vector start;
	Vector dir;
	Ray(Vector s, Vector d) {
		start=s;
		dir=d;
	}
};

struct Light {
	Color color;
	Vector pos;
};

struct Surface {
	virtual Color getColor(Intersection inter, Ray r, Light* lights,
	                       int lightnum, int rekurzio) = 0;
};



struct Object {
	Surface *surface;
	virtual Intersection intersect(Ray r)=0;
};




struct Scene {
	Object* objects[10];
	Light lights[10];
	int objectnum;
	int lightnum;
	Color image[screenWidth*screenHeight];	// egy alkalmazás ablaknyi kép

	Scene() {
		objectnum=0;
		lightnum=0;
	}
	void render() {
		glDrawPixels(screenWidth, screenHeight, GL_RGB, GL_FLOAT, image);
	}

	Color trace(Ray r, int iter) {
		Color retColor=Color(0.5f, 1.0f, 1.0f);
		Intersection i = intersectAll(r);
		if (i.objectID != -1) {
			retColor = objects[i.objectID]->surface->getColor(i, r,
			           lights, lightnum, iter);
		}

		return retColor;

	}

	Intersection intersectAll(Ray r) {
		Intersection it;
		float FLT_MAX=4096;
		float mintav=0;
		for(int i=0; i<objectnum; i++) {
			Intersection tnew = objects[i]->intersect(r);
			float tav = (tnew.pos - r.start).Length();

			if(tnew.talalt &&(tav<mintav || it.objectID==-1) && tav<FLT_MAX)  {
				it = tnew;
				it.objectID=i;
				mintav=tav;
			}
		}
		return it;
	}

	void addObject(Object* o) {
		if(objectnum<10) {
			objects[objectnum++]=o;
		}
	}

	void addLight(Light l) {
		if(lightnum<10) {
			lights[lightnum++]=l;
		}
	}

};
Scene scene;

struct Camera {
	Vector eye;
	Vector lookat;
	Vector up;
	Vector right;

	Camera(Vector e, Vector l, Vector u) {
		eye=e;
		lookat=(l-eye).normalize();
		float fov=120;
		float meret = tan((fov*M_PI/180)/2); //ezt at kell irni!!!!!!!!!!!!!!!!!!!!
		right=(((lookat-eye).normalize())%u).normalize()*meret;
		up=(right%((lookat-eye).normalize())).normalize()*meret;
	}

	void picture() {
		for(int i=0; i<screenWidth; i++) {
			for(int j=0; j<screenHeight; j++) {
				scene.image[i+j*screenHeight]=pixel(i,j);
			}
		}
	}

	Color pixel(int x, int y) {
		Vector p=lookat + right * ((2.0f * (x + 0.5f)) /screenWidth - 1.0f)
                        + up * ((2.0f * (y + 0.5f)) /screenHeight - 1.0f);
		Ray r=Ray(eye,(p-eye).normalize());
		//std::cout<<x<<" "<<y<<" ; "<<r.start.x<<" "<<r.start.y<<" "<<r.start.z<<" ; "<<r.dir.x<<" "<<r.dir.y<<" "<<r.dir.z<<std::endl;
		Color retColor=scene.trace(r, 0);
		return retColor;
	}

};

Camera camera=Camera(Vector(0, 2, -2), Vector(0, 0,0), Vector(0, 1, 0));




struct DiffuseSurface :public Surface {

	Color getColor(Intersection inter, Ray r, Light* lights,
	               int lightnum, int rekurzio) {
		if(inter.pos.x<-0.99) {
			return Color(1,0,0);
		}
		if(inter.pos.x>0.99) {
			return Color(0.5,0.5,0.5);
		}
		if(inter.pos.z<-0.99 ) {
			return Color(0,0,1);
		}
		if(inter.pos.z>0.99 ) {
			return Color(0,0,0);
		}
		return Color(0.0,1.0,0.0);
	}
};

struct ReflectionSurface :public Surface {

};

struct RefractionSurface :public Surface {

};

struct Floor :public Object {

	Vector normal;
	Vector v;
	Vector tolas;   //sarkok kiszamolasahoz
	Floor(Surface* surf, Vector v1, Vector n,Vector t=Vector(1,0,1)) {
		surface=surf;
		v=v1;
		normal=n;
		tolas=t;
	}

	Intersection intersect(Ray r) {
		Vector sarok[4];
		sarok[0]=v+tolas;
		sarok[2]=v-tolas;
		sarok[1]=v+Vector(-tolas.x, tolas.y, tolas.z);
		sarok[3]=v+Vector(tolas.x, tolas.y, -tolas.z);
		float t=((v-r.start)*normal)/(r.dir*normal); //(r1- eye) * n/(v * n)
		if(t>epsilon) {
			bool benne=true;
			for(int i=0; i<4; i++) {
				Vector v=sarok[i]-sarok[i==3?0:i+1];
				v.normalize();
				Vector n=Vector(v.z, v.y, -v.x);
				Vector metszes(r.start+r.dir*t);
				float tav =n*(sarok[i] - metszes);
				if(tav > epsilon) {
					benne = false;
					break;
				}

			}
			if(benne) {
				Intersection i=Intersection(r.start+r.dir*t,normal,true);
				return i;
			}
		}

		return Intersection();
	}

};

struct Henger :public Object {

};

struct Ellipszoid :public Object {

};

struct Paraboloid :public Object {

};

// Inicializacio, a program futasanak kezdeten, az OpenGL kontextus letrehozasa utan hivodik meg (ld. main() fv.)
void onInitialization( )
{
	glViewport(0, 0, screenWidth, screenHeight);
	DiffuseSurface ds;
	Floor floor=Floor(&ds,Vector(0,0,0),Vector(0,1,0));
	scene.addObject(&floor);
	camera.picture();

}

// Rajzolas, ha az alkalmazas ablak ervenytelenne valik, akkor ez a fuggveny hivodik meg
void onDisplay( )
{
	glClearColor(0.1f, 0.2f, 0.3f, 1.0f);		// torlesi szin beallitasa
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // kepernyo torles

	scene.render();

	glutSwapBuffers();     				// Buffercsere: rajzolas vege

}

// Billentyuzet esemenyeket lekezelo fuggveny (lenyomas)
void onKeyboard(unsigned char key, int x, int y)
{
	if (key == 'd') {
		glutPostRedisplay( ); 		// d beture rajzold ujra a kepet

	}

}

// Billentyuzet esemenyeket lekezelo fuggveny (felengedes)
void onKeyboardUp(unsigned char key, int x, int y)
{

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
	//long time = glutGet(GLUT_ELAPSED_TIME);		// program inditasa ota eltelt ido

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

