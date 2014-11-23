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
	Vector operator+(const Vector& v) {
		return Vector(x + v.x, y + v.y, z + v.z);
	}
	Vector operator-(const Vector& v) {
		return Vector(x - v.x, y - v.y, z - v.z);
	}
	float operator*(const Vector& v) { 	// dot product
		return (x * v.x + y * v.y + z * v.z);
	}

	float operator/(const Vector& v) {
		return (x / v.x + y / v.y + z / v.z);
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

	Vector operator*(Matrix m) {
		return Vector(x*m.m[0]+y*m.m[3]+z*m.m[6],x*m.m[1]+y*m.m[4]+z*m.m[7],x*m.m[2]+y*m.m[5]+z*m.m[8]);
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
const int NTESS=100;

struct Object {
	Vector center;
	virtual void draw()=0;
};

struct Light {
	GLenum id;
	Vector pos;
	Color Ld, La, Ls;

	Light() {}

	Light(GLenum i, Vector p, Color d, Color a, Color s) {
		id=i;
		pos=p;
		Ld=d;
		La=a;
		Ls=s;
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

	Camera() {}

	Camera(Vector e, Vector l, Vector u) {
		eye=e;
		lookat=l;
		vup=u;
		fov=90;
		asp=1;
		fp=0.1;
		bp=100;
	}

	void setOGL() {

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity( );
		gluPerspective(fov, asp, fp, bp);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity( );
		gluLookAt(eye.x, eye.y, eye.z, lookat.x, lookat.y, lookat.z, vup.x, vup.y, vup.z);


	}
};



struct Material {
	Color kd;
	Color ks;
	Color ka;
	int shininess;
    bool blended;
	Material() {}

	Material(Color d, Color s, Color a, int n, bool bl=false) {
		kd=d;
		ks=s;
		ka=a;
		shininess=n;
		blended=bl;
	}

	void setOGL() {
		float d[] = {kd.r, kd.g, kd.b, 1.0};
		float s[] = {ks.r, ks.g, ks.b, 1.0};
		float a[] = {ka.r, ka.g, ka.b, 1.0};

        if(blended){
            d[3]=s[3]=a[3]=0.2;
        }

		glMaterialfv( GL_FRONT, GL_DIFFUSE, d);
		glMaterialfv( GL_FRONT, GL_SPECULAR, s);
		glMaterialfv( GL_FRONT, GL_AMBIENT, a);
		glMaterialf( GL_FRONT, GL_SHININESS, shininess);

	}

	void changeAll(Color c){
        ka=kd=ks=c;
	}
};


struct Texture {
	unsigned int text_id;
	unsigned char tex[256][256][3];

	void gen() {
		for(int i=0; i<256; i++) {
			for(int j=0; j<256; j++) {
					if(j%2==0 && i%4==0){
                        srand(j);
                        tex[i][j][0]=0;
                        tex[i][j][1]=128;
                        tex[i][j][2]=0;
					}else{
                        tex[i][j][0]=255;
                        tex[i][j][1]=128;
                        tex[i][j][2]=0;
					}


			}
		}

	}

	void setOGL() {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 256, 256, 0, GL_RGB,GL_UNSIGNED_BYTE, tex);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	}

};

struct ParamObject : public Object {
	Material* mat;
	Texture* tex;
	bool hasMater;
	bool hasText;

	virtual Vector getNormal(float x, float y, float z)=0;

	void draw() {
		if(hasMater) {
			mat->setOGL();
		}

		if(hasText) {
			tex->setOGL();
		}

		glBegin(GL_QUADS);
		for(int i=0; i<NTESS; i++) {
			for(int j=0; j<NTESS; j++) {
				VertexOGL( (float)i/NTESS, (float)j/NTESS );
				VertexOGL( (float)(i+1)/NTESS, (float)j/NTESS );
				VertexOGL( (float)(i+1)/NTESS, (float)(j+1)/NTESS );
				VertexOGL( (float)i/NTESS, (float)(j+1)/NTESS );
			}
		}
		glEnd();
	}
	virtual void VertexOGL(float u, float v)=0;
		virtual ~ParamObject(){}
};


struct CRTest :public ParamObject {
	CRTest(Material *m) {
		mat=m;
	}
	void VertexOGL(float u, float v) {}
};


struct Gomb :public ParamObject {
	float r;



	Vector getNormal(float x, float y, float z) {

	}

	Gomb(Vector c,Material *m, float rx, Texture* t=NULL) {
		if(m) {
			mat=m;
			hasMater=true;
		} else {
			hasMater=false;
		}
		if(t) {
			hasText=true;
			tex=t;
		} else {
			hasText=false;
		}
		r=rx;
		center=c;
	}

	void VertexOGL(float u, float v) {
		glTexCoord2f(u,v);
		//x(u, v) = r · sin vπ · cos 2πu, y(u, v) = r · sin vπ · sin 2πu, z(u, v) = r · cos vπ.
		float x=r*sin(v*M_PI)*cos(2*M_PI*u)+center.x;
		float y=r*sin(v*M_PI)*sin(2*M_PI*u)+center.y;
		float z=r*cos(v*M_PI)+center.z;

		Vector n(x,y,z);
		n=n-center;
		glNormal3f(n.x,n.y,n.z);
		glVertex3f(x, y, z);

	}

};

struct Henger :public ParamObject {

    float h;
    Vector irany;
    float R;

	Henger(Material *m, Vector c, float hx,float r, Vector i) {
	    center=c;
	    h=hx;
	    irany=i.normalize();
		mat=m;
		R=r;
        if(m) {
			mat=m;
			hasMater=true;
		} else {
			hasMater=false;
		}
        hasText=false;
	}

    Vector getNormal(float x, float y, float z){
        if(z<center.z+0.001){
            return irany;
        }
        if(z>center.z+h-0.001){
            return irany*-1;
        }
        return Vector(x,y,center.z)-center;

    }
    //center==talppont
    //x=R·cos2piu y=R·sin2piu z=h·v
	void VertexOGL(float u, float v) {
        float x=R*cos(2*M_PI*u)+center.x;
        float y=R*sin(2*M_PI*u)+center.y;
        float z=h*v+center.z;

        Vector n=getNormal(x,y,z);
		glNormal3f(n.x,n.y,n.z);
		glVertex3f(x, y, z);
	}

};

struct Napelem :public ParamObject {

	Napelem(Material *m) {
		mat=m;
	}

	Vector getNormal(){}

	void VertexOGL(float u, float v) {}
};

struct MIR {
    CRTest* test;
    Napelem* napelemek[2];
    Vector center;

    void build(){

    }

    void draw(){

    }
};

struct Muhold {
    Gomb* test;
    Henger* fuvokak[6];
    Vector center;
    float r;
    Material testm;
    Material fuvokam;

    Muhold(Vector c){
        center=c;
        r=1;
    }

    void build(){
        testm=Material(Color(0.3,0.3,0),Color(0.1,0,0),Color(0,0,0.1),100);
        fuvokam=Material(Color(1,0,0),Color(1,0,0),Color(1,0,0),10);
        test=new Gomb(center, &testm ,r, NULL);
        //Henger(Material *m, Vector c, float hx,float r, Vector i)

        fuvokak[0]=new Henger(&fuvokam, Vector(center.x+r,center.y,center.z), r/2, r/10, Vector(r,0,0));
        fuvokak[1]=new Henger(&fuvokam, Vector(center.x-r,center.y,center.z), r/2, r/10, Vector(-r,0,0));

        fuvokak[2]=new Henger(&fuvokam, Vector(center.x,center.y+r,center.z), r/2, r/10, Vector(0,r,0));
        fuvokak[3]=new Henger(&fuvokam, Vector(center.x,center.y-r,center.z), r/2, r/10, Vector(0,-r,0));

        fuvokak[4]=new Henger(&fuvokam, Vector(center.x,center.y,center.z+r), r/2, r/10, Vector(0,0,r));
        fuvokak[5]=new Henger(&fuvokam, Vector(center.x,center.y,center.z-r), r/2, r/10, Vector(0,0,-r));



    }

    void draw(){

        test->draw();
        for(int i=0;i<6;i++){
            Vector c=fuvokak[i]->center;
            glPushMatrix();

            glMatrixMode(GL_MODELVIEW);

            switch(i){
            case 0:
//                glTranslatef(c.x+center.x,c.y+center.y,c.z+center.z+r);
                glTranslatef(center.x,center.y,center.z+r);
                glRotatef(90,0,1,0);
                break;
            case 1:
                //glTranslatef(c.x+center.x,c.y+center.y,c.z+center.z+r);
                glTranslatef(center.x,center.y,center.z+r);
                glRotatef(-90,0,1,0);
                break;
            case 2:
                //glTranslatef(c.x+center.x,c.y+center.y,c.z+center.z+r);
                glTranslatef(center.x,center.y,center.z+r);
                glRotatef(-90,1,0,0);
                fuvokam.changeAll(Color(0,1,0));
                break;
            case 3:
                //glTranslatef(c.x+center.x,c.y+center.y,c.z+center.z+r);
                glTranslatef(center.x,center.y,center.z+r);
                glRotatef(90,1,0,0);
                break;
            case 4:
                glRotatef(0,1,0,0);
                fuvokam.changeAll(Color(0,0,1));
                break;
            case 5:
                //glTranslatef(c.x+center.x,c.y+center.y,c.z+center.z-r);
                glTranslatef(center.x,center.y,center.z-r);
                glRotatef(180,0,1,0);
                break;
            }

            fuvokak[i]->draw();
            glPopMatrix();
        }
    }

    ~Muhold(){
        delete test;
        delete[] fuvokak;
    }

};


struct Scene {
	Camera* camera;

//	Light* lights[10];
//	int lightnum;
//
	Object* objects[10];
	int objectnum;


    Light* Sun;
    Gomb* planet;
    Gomb* Legkor;
    Muhold* muhold;
    MIR* mir;

    Material legkor;
    Material planetMaterial;
    Texture planetTexture;

	Scene() {
//		lightnum=0;
		objectnum=0;
	}

//	void addLight(Light *l) {
//		if(lightnum<10) {
//			lights[lightnum++]=l;
//		}
//	}
//
//	void addObject(Object *o) {
//		if(objectnum<10) {
//			objects[objectnum++]=o;
//		}
//	}

	void build() {
		camera= new Camera(Vector(0,3,10),Vector(0,0,0),Vector(0,0,1));
		Sun=new Light(GL_LIGHT0,Vector(0,4,5),Color(10,10,10),Color(10,10,10),Color(10,10,10));


        //Henger(Material *m, Vector c, float hx,float r, Vector i)
//        Material fem=Material(Color(0,0,0),Color(0,0,0),Color(0,0,0),1);
//       objects[objectnum++]=new Henger(&fem, Vector(0,0,0), 1, 0.5, Vector(0,0,1));

        muhold=new Muhold(Vector(0,0,0));
        muhold->build();

		planetMaterial=Material(Color(1,0.5,0),Color(0,0,0),Color(0,0,0),100);
		planet=new Gomb(Vector(0,0,-8),&planetMaterial,2,&planetTexture);


		glGenTextures(1, &(planetTexture.text_id));
		glBindTexture(GL_TEXTURE_2D,planetTexture.text_id);
		planetTexture.gen();


        glClearColor(0.0,0.0,0.0,0.0);
        legkor=Material(Color(0,0,1),Color(0,0,1),Color(0,0,1),10, true);
        Legkor=new Gomb(planet->center,&legkor,planet->r+0.1,NULL);
	}

	void render() {
		camera->setOGL();
        Sun->setOGL();
//		for(int i=0; i<lightnum; i++) {
//			lights[i]->setOGL();
//		}
        glPushMatrix();
        glMatrixMode(GL_MODELVIEW);
            glEnable(GL_TEXTURE_2D);
            planet->draw();
            glDisable(GL_TEXTURE_2D);

            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
                Legkor->draw();
            glDisable(GL_BLEND);
        glPopMatrix();

		for(int i=0; i<objectnum; i++) {

			objects[i]->draw();
		}
        muhold->draw();


	}

	~Scene() {
//		delete[] lights;
		delete[] objects;
        delete Sun;
        delete planet;
        delete Legkor;

		delete camera;
//		lightnum=0;
        objectnum=0;
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
	glEnable(GL_LIGHTING);
	glShadeModel(GL_SMOOTH);

    //Material(Color d, Color s, Color a, int n, bool bl=false)

	scene=Scene();
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
	if (key == 'd') glutPostRedisplay( ); 		// d beture rajzold ujra a kepet

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

