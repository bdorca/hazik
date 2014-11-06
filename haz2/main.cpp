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
	Color operator/(float a) {
		return Color(r / a, g / a, b / a);
	}
	Color operator*(const Color& c) {
		return Color(r * c.r, g * c.g, b * c.b);
	}
	Color operator/(const Color& c) {
		return Color(r / c.r, g / c.g, b / c.b);
	}
	Color operator+(const Color& c) {
		return Color(r + c.r, g + c.g, b + c.b);
	}
	Color operator-(const Color& c) {
		return Color(r - c.r, g - c.g, b - c.b);
	}

	Color operator-(float a) {
		return Color(r-a,g-a,b-a);
	}

	Color operator+(float a) {
		return Color(r+a,g+a,b+a);
	}

	Color tonemap() {
		float I=0.21*r + 0.72*g + 0.07*b;
		float D=I/(I+1);
		return (*this*(I/D)).saturate();
	}

	Color saturate() {
		return Color(r<1.0f?(r>0.0f?r:0.0f):1.0f,g<1.0f?(g>0.0f?g:0.0f):1.0f,b<1.0f?(b>0.0f?b:0.0f):1.0f);
	}
};
const float FLT_MAX=1048576.1048576;
const float epsilon=0.000001;
const float epsilon2=0.0001;
int dmax=1;
const int screenWidth = 600;	// alkalmazás ablak felbontása
const int screenHeight = 600;

//a struktura kibovitett valtozata ennek-> https://wiki.sch.bme.hu/Sz%C3%A1m%C3%ADt%C3%B3g%C3%A9pes_grafika_h%C3%A1zi_feladat_tutorial#Hogyan_k.C3.B6vess.C3.BCk_a_sugarakat.3F
struct Intersection {
	bool talalt;
	Vector pos, normal;
	int objectID;
	float teto;
	Intersection() {
		objectID=-1;
		talalt=false;
		pos=Vector(0,0);
		normal=Vector(0,0);
		teto=false;
	}

	Intersection(Vector p, Vector n, bool t, bool tet=false, int o=-1) {
		pos=p;
		normal=n;
		talalt=t;
		objectID=o;
		teto=tet;
	}
};


struct Ray {
	Vector start;
	Vector dir;

	Ray() {}

	Ray(Vector s, Vector d) {
		start=s;
		dir=d;
	}
};

struct Light {
	Color color;
	Vector pos;

	Light(Color c=Color(0,0,0), Vector p=Vector(0,0,0)) {
		color=c;
		pos=p;
	}

	Color getColor(Intersection inter) {
		Vector tav=pos-inter.pos;
		float s=pow(1/tav.Length(),2.0);
		float intensity =((inter.normal* tav.normalize())>0?(inter.normal* tav.normalize()): 0.0f);
		Color ret=color*s*intensity;
		return ret.tonemap();
	}

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
	Color La;
	Scene() {
		La=Color(0.5f, 1.0f, 1.0f);
		objectnum=0;
		lightnum=0;
	}
	void render() {
		glDrawPixels(screenWidth, screenHeight, GL_RGB, GL_FLOAT, image);
	}

	Color trace(Ray r, int iter) {
		Color retColor=La;
		if(iter>dmax) return retColor;

		Intersection i = intersectAll(r);
		if (i.objectID != -1) {
			retColor = objects[i.objectID]->surface->getColor(i, r,
			           lights, lightnum, iter);
		}

		return retColor;

	}

	Intersection intersectAll(Ray r) {
		Intersection it;

		float mintav=FLT_MAX;
		for(int i=0; i<objectnum; i++) {
			Intersection tnew = objects[i]->intersect(r);
			float tav = (tnew.pos - r.start).Length();

			if(tnew.talalt &&(tav<mintav || it.objectID==-1))  {
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

	Camera() {}

	Camera(Vector e, Vector l, Vector u) {
		eye=e;
		lookat=(l-eye).normalize();
		float fov=120;
		float meret = tan((fov*M_PI/180)/2); //ezt at kell irni!!!!!!!!!!!!!!!!!!!!
		right=(lookat%u).normalize()*meret;
		up=(right%lookat).normalize()*meret;
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

Camera camera;




struct DiffuseSurface :public Surface {
	Color  kd;
	Color ks;
	float n;
	DiffuseSurface(Color k=Color(1,0.5,1), Color s=Color(1,1,1)) {
		kd=k;// / M_PI;
		n=100;
		ks=s *(n + 2) / M_PI / 2.0;

	}

	Color getColor(Intersection inter, Ray r, Light lights[], int lightnum, int rekurzio) {
		Color Lref;
		Color kdx=kd;
		//Color kdx=kd*(fabs(sin(inter.pos.x*2*M_PI)*sin(inter.pos.z*2*M_PI)));
		Color ka=kd/M_PI;
		Lref=Lref+scene.La*ka;
		for(int i=0; i<lightnum; i++) {
			Vector sh= (lights[i].pos-inter.pos).normalize();
			Ray shadowR(inter.pos+(sh*epsilon2),sh);
			Intersection shadow=scene.intersectAll(shadowR);
			if(shadow.talalt) {
				//std::cout<<"sht "<<shadow.pos.x<<" "<<shadow.pos.y<<" "<<shadow.pos.z<<std::endl;
			}
			if(!shadow.talalt ||(shadowR.start - shadow.pos).Length()> (shadowR.start - lights[i].pos).Length()) {
				float costheta = inter.normal * lights[i].pos;
				if (costheta < 0) return Color(0, 0, 0);
				Lref= Lref+ lights[i].getColor(inter) * kdx * costheta;		// diffuse reflection
				Vector H =(lights[i].pos + r.start).normalize();
				float cosdelta=inter.normal*H;
				if (cosdelta < 0) return Lref;
				Lref = Lref +
				       lights[i].getColor(inter) * ks * pow(cosdelta, n);	// glossy reflection
			}
		}
		return Lref;
//		if(inter.pos.x<0) {
//			return Color(1,0,1);
//		}
//		if(inter.pos.x>0) {
//			return Color(1,1,0);
//            std::cout<<inter.pos.x<<" "<<inter.pos.y<<" "<<inter.pos.z<<std::endl;
//
//		}
//		if(inter.pos.z<-0.99 ) {
//			return Color(0,0,1);
//		}
//		if(inter.pos.z>0.99 ) {
//			return Color(0,0,0);
//		}
//		return Color(0.0,1.0,0.0);
	}

};

struct ReflectionSurface :public Surface {
	Color F0;
	ReflectionSurface() {}
	ReflectionSurface(Color n, Color k) {
		F0=((n - 1) * (n - 1) + k * k) / ((n + 1) * (n + 1) + k * k);
	}


	Color Fresnel(Vector n, Vector v) {
		float costheta=fabs(n*v);
		return F0+(Color(1,1,1)-F0)*pow( 1-costheta, 5 );
	}

	Ray reflect(Intersection inter, Ray r) {
		float cosa=(r.dir*inter.normal)*-1;
		Ray reflectR;
		reflectR.dir=(r.dir+inter.normal*cosa*2).normalize();
		reflectR.start=inter.pos + reflectR.dir * epsilon2;
		return reflectR;
	}

	Color getColor(Intersection inter, Ray r, Light lights[], int lightnum, int rekurzio) {
		if (r.dir * inter.normal > 0.0f) {
			inter.normal = inter.normal*-1 ;
		}
		Ray reflectR=reflect(inter,r);
		Color trace=scene.trace(reflectR,rekurzio+1);
		Color f = Fresnel(inter.normal, r.dir);
		return (f * trace);

	}

};

struct RefractionSurface :public Surface {

	float k;
	float n;

	RefractionSurface() {}

	RefractionSurface(float kx, float nx) {
		k=kx;
		n=nx;
	}

	Color getColor(Intersection inter, Ray r, Light lights[], int lightnum, int rekurzio) {

		if(inter.teto && inter.pos.x>-0.1& inter.pos.x<0.1 &&inter.pos.z>-0.1&& inter.pos.z<0.1 ) {
			//std::cout<<"zx "<<inter.pos.x<<" "<<inter.pos.y<<" "<<inter.pos.z<<std::endl;
			return Color(1,1,1);
		}
		if (inter.teto && inter.pos.y<0.1) {
			return Color(1,0,0);
		}
		if (inter.teto && inter.pos.y>0.8) {
			//std::cout<<inter.pos.x<<" "<<inter.pos.y<<" "<<inter.pos.z<<std::endl;
			return Color(1,1,0);
		}

		if(inter.pos.z<0 ) {
             //   std::cout<<inter.pos.x<<" "<<inter.pos.y<<" "<<inter.pos.z<<std::endl;
			return Color(0,0,1);
		}
		if(inter.pos.z>0) {
               //std::cout<<inter.pos.x<<" "<<inter.pos.y<<" "<<inter.pos.z<<std::endl;
			return Color(0,0,0);
		}
		if(inter.pos.y>1) {
			return Color(0.5,0.5,0.5);
		}



		return Color(0,1,0);
	}

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
	Floor() {}
	Intersection intersect(Ray r) {
		Vector sarok[4];
		sarok[0]=v+tolas;
		sarok[2]=v-tolas;
		sarok[1]=v+Vector(-tolas.x, tolas.y, tolas.z);
		sarok[3]=v+Vector(tolas.x, tolas.y, -tolas.z);
		float t=((v-r.start)*normal)/(r.dir*normal); //(r1- eye) * n/(v * n)
		if(t>epsilon) {
			bool benne=true;
//			for(int i=0; i<4; i++) {
//				Vector v=sarok[i]-sarok[i==3?0:i+1];
//				v.normalize();
//				Vector n=Vector(v.z, v.y, -v.x);
//				Vector metszes(r.start+r.dir*t);
//				float tav =n*(sarok[i] - metszes);
//				if(tav > epsilon) {
//					benne = false;
//					break;
//				}
//			}
			if(benne) {
				Intersection i=Intersection(r.start+r.dir*t,normal,true);
				return i;
			}
		}

		return Intersection();
	}

};

struct Henger :public Object {
	float sugar;
	Vector tengelyv;
	Vector talppont;
	float magassag;

	Henger() {}

	Henger(Surface* surf, float rx, Vector t, Vector tp, float m) {
		surface=surf;
		sugar=rx;
		tengelyv=t.normalize();
		talppont=tp;
		magassag=m;
	}
	Vector getNormal(Vector p) {
		if(p.y<=talppont.y) {
			return tengelyv*-1;
		}
		if(p.y>=talppont.y+magassag) {
			return tengelyv;
		}
		Vector n=Vector(p-(talppont+Vector(0,p.y,0).normalize()));
		return n;
	}
	//https://www.cl.cam.ac.uk/teaching/1999/AGraphHCI/SMAG/node2.html
	//http://mathworld.wolfram.com/Circle-LineIntersection.html
	// https://dl.dropboxusercontent.com/u/15861809/Cylinder_Line_Intersection.pdf
	Intersection intersect(Ray r) {

		float a=pow(r.dir.x,2)+pow(r.dir.z,2);
		float b=2*((r.start.x-talppont.x)*r.dir.x+(r.start.z-talppont.z)*r.dir.z);
		float c=pow(r.start.x-talppont.x,2)+pow(r.start.z-talppont.z,2)-pow(sugar,2);
		float D=pow(b,2)-4*a*c;
		if(D>=0-epsilon) {
			float t1=(-b+sqrt(D))/2*a;
			float t2=(-b-sqrt(D))/2*a;
			float y1=r.start.y+t1*r.dir.y;
			float y2=r.start.y+t2*r.dir.y;
			float x1=r.start.x+t1*r.dir.x;
			float x2=r.start.x+t2*r.dir.x;
			float z1=r.start.z+t1*r.dir.z;
			float z2=r.start.z+t2*r.dir.z;


			float tetoptav=(r.start-(talppont+tengelyv*magassag)).Length();
			float talptav=(r.start-talppont).Length();
            Intersection teto;
            if((y1>talppont.y+tengelyv.y*magassag && y2<talppont.y+tengelyv.y*magassag) || (y2>talppont.y+tengelyv.y*magassag && y1<talppont.y+tengelyv.y*magassag)){
                float t3=(talppont.y+tengelyv.y*magassag-r.start.y)/r.dir.y;
                if(pow(r.start.x+r.dir.x*t3+talppont.x,2)+pow(r.start.z+r.dir.z*t3+talppont.z,2)<=pow(sugar,2))
                    teto=Intersection(r.start+r.dir*t3,getNormal(r.start+r.dir*t3),true,true);
            }

            if((y1>talppont.y && y2<talppont.y) || (y2>talppont.y&& y1<talppont.y)){
                float t3=(talppont.y+tengelyv.y*magassag-r.start.y)/r.dir.y;
                if(pow(r.start.x+r.dir.x*t3+talppont.x,2)+pow(r.start.z+r.dir.z*t3+talppont.z,2)<=pow(sugar,2))
                    teto=Intersection(r.start+r.dir*t3,getNormal(r.start+r.dir*t3),true,true);
            }

			if((y1<(talppont.y+tengelyv.y*magassag) && y1>talppont.y)) {
                float t=((t1>0&&t1<t2)?t1:t2);
				if(y2>talppont.y &&y2<talppont.y+magassag*tengelyv.y) {

                    Intersection i=Intersection(r.start+r.dir*t,Vector(0,0,0),true);
                    if(teto.talalt && (r.start-teto.pos).Length()<(r.start-i.pos).Length())
                        return teto;
                    return i;

				}
                Intersection i=Intersection(r.start+r.dir*t,getNormal(r.start+r.dir*t1),true);
                    if(teto.talalt && (r.start-teto.pos).Length()<(r.start-i.pos).Length())
                        return teto;
                    return i;

			}

			if(y2>talppont.y &&y2<talppont.y+magassag*tengelyv.y) {
                float t=((t1>0&&t1<t2)?t1:t2);
                Intersection i= Intersection(r.start+r.dir*t,getNormal(r.start+r.dir*t2),true);
                    if(teto.talalt && (r.start-teto.pos).Length()<(r.start-i.pos).Length())
                        return teto;
                    return i;

			}
            return teto;

		}

		return Intersection();
	}
};

struct Ellipszoid :public Object {
	Vector center;
	Vector abc;
	//http://mathworld.wolfram.com/Ellipsoid.html
	Ellipszoid(Surface* surf,Vector c, Vector a) {
		surface=surf;
		center=c;
		abc=a;
	}

	Intersection intersect(Ray r) {

	}

};

struct Paraboloid :public Object {

};

//--------------------------------------------
DiffuseSurface ds;
RefractionSurface refs;
ReflectionSurface arany;
ReflectionSurface ezust;
Floor flor;
Henger henger;
Light light1;
Light light2;
Light light3;
// Inicializacio, a program futasanak kezdeten, az OpenGL kontextus letrehozasa utan hivodik meg (ld. main() fv.)
void onInitialization( )
{
	glViewport(0, 0, screenWidth, screenHeight);

	camera=Camera(Vector(0, 2,-7), Vector(0, 0,0), Vector(0, 1, 0));

	ds=DiffuseSurface();
	refs=RefractionSurface();
	arany=ReflectionSurface(Color(0.17,0.35,1.5),Color(3.1,2.7,1.9));
	ezust=ReflectionSurface(Color(0.14,0.16,0.13),Color(4.1,2.3,3.1));

	flor=Floor(&ds,Vector(0,0,0),Vector(0,1,0));

	henger=Henger(&ezust,1,Vector(0,1,0),Vector(0,0,0),1);


	scene.addObject(&henger);
    scene.addObject(&flor);

	light1=Light(Color(1,0,0),Vector(1,1.5,1));
	light2=Light(Color(0,1,0),Vector(-1,1.5,0));
	light3=Light(Color(0,0,1),Vector(-1,1.5,-1));

	scene.addLight(light1);
	scene.addLight(light2);
	scene.addLight(light3);
	camera.picture();

}

// Rajzolas, ha az alkalmazas ablak ervenytelenne valik, akkor ez a fuggveny hivodik meg
void onDisplay( )
{
	glClearColor(0,0,0,0);		// torlesi szin beallitasa
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // kepernyo torles

	scene.render();

	glutSwapBuffers();     				// Buffercsere: rajzolas vege

}

// Billentyuzet esemenyeket lekezelo fuggveny (lenyomas)
void onKeyboard(unsigned char key, int x, int y)
{
	static int z=-4;
	static int yy=1;
	if (key == 'd') {
		camera=Camera(Vector(0, yy, --z), Vector(0, 0,0), Vector(0, 1, 0));
		std::cout<<yy<<" "<<z<<std::endl;
		camera.picture();
		glutPostRedisplay( ); 		// d beture rajzold ujra a kepet
	}
	if (key == 'a') {
		camera=Camera(Vector(0, yy, ++z), Vector(0, 0,0), Vector(0, 1, 0));
		std::cout<<yy<<" "<<z<<std::endl;
		camera.picture();
		glutPostRedisplay( ); 		// d beture rajzold ujra a kepet
	}
	if (key == 'w') {
		camera=Camera(Vector(0, ++yy, z), Vector(0, 0,0), Vector(0, 1, 0));
		std::cout<<yy<<" "<<z<<std::endl;
		camera.picture();
		glutPostRedisplay( ); 		// d beture rajzold ujra a kepet
	}
	if (key == 's') {
		camera=Camera(Vector(0, --yy, z), Vector(0, 0,0), Vector(0, 1, 0));
		std::cout<<yy<<" "<<z<<std::endl;
		camera.picture();
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
		camera.picture();
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

