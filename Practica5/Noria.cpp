/***************************************************
Autora: Mireia Pires State.
Simulación del movimiento de una noria.
Dependencias:
+GLUT
***************************************************/
#define PROYECTO "Noria3D de Mireia"
#define PI 3.1415926

#include <iostream> // Biblioteca de entrada salida
#include <sstream>
#include <Utilidades.h>

using namespace std;

// Nombres de las listas.
static GLuint noria;
static GLuint radios;
static GLuint cabinaAbajo;
static GLuint cabinaArriba;
static GLuint soporteCabina1;
static GLuint soporteCabina2;
static GLuint puertas;

// Variables dependientes del tiempo.
static float anguloTiempo = 0;
static const int tasaFPS = 60;

void crearRadios() {
	// A partir de un modelo de cilindro, creamos los radios.
	for (float angulo = 0; angulo < 360; angulo += 30) {
		glPushMatrix();
		glRotatef(angulo, 0, 0, 1);
		glCallList(radios);
		glPopMatrix();
	}
}

void crearCabinas() {
	// Se crean 12 cabinas.
	for (int angulo = 0; angulo < 360; angulo += 30) {
		// Se crean los soportes grises.
		glPushMatrix();
		glTranslatef(cos(angulo * PI / 180) * 0.87, sin(angulo * PI / 180) * 0.87, 0.03);
		glCallList(soporteCabina1);
		glPopMatrix();

		// Se crean los soportes rojos que 
		// enganchan la cabina al soporte gris.
		glPushMatrix();
		glTranslatef(cos(angulo * PI / 180) * 0.87, sin(angulo * PI / 180) * 0.87, 0.125);
		glCallList(soporteCabina2);
		glPopMatrix();

		// Se crean las puertas de las cabinas.
		glPushMatrix();
		glRotatef(-anguloTiempo, 0, 0, 1);
		glTranslatef(cos((anguloTiempo - angulo) * PI / 180) * 0.87, sin((anguloTiempo - angulo) * PI / 180) * 0.87 - 0.2, 0.169);
		glScalef(0.7, 1.5, 0.01);
		glCallList(puertas);
		glPopMatrix();

		// Se crean las piezas de abajo de las cabinas.
		glPushMatrix();
		glRotatef(-anguloTiempo, 0, 0, 1);
		glTranslatef(cos((anguloTiempo - angulo) * PI / 180) * 0.87, sin((anguloTiempo - angulo) * PI / 180) * 0.87 - 0.255, 0.25);
		glColor3f(0.5, 0, 0);
		glCallList(cabinaAbajo);
		glPopMatrix();

		// Se crean las piezas de arriba de las 
		// cabinas simulando ser de cristal.
		glPushMatrix();
		glRotatef(-anguloTiempo, 0, 0, 1);
		glTranslatef(cos((anguloTiempo - angulo) * PI / 180) * 0.87, sin((anguloTiempo - angulo) * PI / 180) * 0.87 - 0.1025, 0.25);
		glColor3f(0.5, 0, 0);
		glCallList(cabinaArriba);
		glPopMatrix();
	}
}

void crearSoportesNoria() {
	// Soportes delanteros.
	glPushMatrix();
	glColor3f(0.45, 0.45, 0.45);
	glTranslatef(-0.4, -0.7, -0.25);
	glRotatef(-30, 0, 0, 1);
	glRotatef(15, 1, 0, 0);
	glScalef(0.1, 1.5, 0.04);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.35, 0.35, 0.35);
	glTranslatef(-0.4, -0.7, -0.25);
	glRotatef(-30, 0, 0, 1);
	glRotatef(15, 1, 0, 0);
	glScalef(0.1, 1.5, 0.04);
	glutWireCube(1);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.45, 0.45, 0.45);
	glTranslatef(0.4, -0.7, -0.25);
	glRotatef(30, 0, 0, 1);
	glRotatef(15, 1, 0, 0);
	glScalef(0.1, 1.5, 0.04);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.35, 0.35, 0.35);
	glTranslatef(0.4, -0.7, -0.25);
	glRotatef(30, 0, 0, 1);
	glRotatef(15, 1, 0, 0);
	glScalef(0.1, 1.5, 0.04);
	glutWireCube(1);
	glPopMatrix();

	// Soportes traseros.
	glPushMatrix();
	glColor3f(0.45, 0.45, 0.45);
	glTranslatef(-0.4, -0.7, 0.75);
	glRotatef(-30, 0, 0, 1);
	glRotatef(-15, 1, 0, 0);
	glScalef(0.1, 1.5, 0.04);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.35, 0.35, 0.35);
	glTranslatef(-0.4, -0.7, 0.75);
	glRotatef(-30, 0, 0, 1);
	glRotatef(-15, 1, 0, 0);
	glScalef(0.1, 1.5, 0.04);
	glutWireCube(1);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.45, 0.45, 0.45);
	glTranslatef(0.4, -0.7, 0.75);
	glRotatef(30, 0, 0, 1);
	glRotatef(-15, 1, 0, 0);
	glScalef(0.1, 1.5, 0.04);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.35, 0.35, 0.35);
	glTranslatef(0.4, -0.7, 0.75);
	glRotatef(30, 0, 0, 1);
	glRotatef(-15, 1, 0, 0);
	glScalef(0.1, 1.5, 0.04);
	glutWireCube(1);
	glPopMatrix();
}

void FPS() {
	// Cuenta los fotogramas por segundo y los muestra por segundo
	// en el titulo de la ventana.

	// Para conseguir el instante en el que se inicia la aplicacion.
	static int antes = glutGet(GLUT_ELAPSED_TIME);
	static int fotogramas = 0;
	int ahora, tiempoTranscurrido;

	fotogramas++;

	ahora = glutGet(GLUT_ELAPSED_TIME);
	tiempoTranscurrido = (ahora - antes) / 1000;

	// Si transcurre mas de un segundo, 
	// muestro fps y reinicio la cuenta y el reloj.

	if (tiempoTranscurrido >= 1) {
		stringstream titulo;
		titulo << "Noria de Mireia Pires State. Los FPS = " << fotogramas;
		glutSetWindowTitle(titulo.str().c_str());
		// Reinicio.
		antes = ahora;
		fotogramas = 0;
	}
}

void init()
{

#pragma region noria
	// Se crea lista con piezas de la noria.
	noria = glGenLists(1);
	glNewList(noria, GL_COMPILE);

	// Color por defecto de los componentes.
	glColor3f(0.45, 0.45, 0.45);

	// Se crea el eje central de la noria.
	glBegin(GL_POLYGON);
	glColor3f(0.35, 0.35, 0.35);
	glutSolidCylinder(0.1, 1, 32, 32);
	glEnd();

	// Se crea la parte frontal del eje.
	glBegin(GL_POLYGON);
	glutSolidCylinder(0.14, 0.1, 32, 32);
	glEnd();

	// Se crea la rueda. 
	glPushAttrib(GL_CURRENT_BIT);
	glPushMatrix();
	glBegin(GL_POLYGON);
	glColor3f(0.84, 0.825, 0.816);
	glutSolidTorus(0.03, 0.87, 64, 64);
	glEnd();
	glPopMatrix();
	glPopAttrib();

	glEndList();
#pragma endregion

#pragma region radios
	// Se crea una lista que mas adelante servira para crear los
	// radios de la noria. Seran cilindros de 0,65 de alto.
	radios = glGenLists(1);
	glNewList(radios, GL_COMPILE);

	glColor3f(0.84, 0.825, 0.816);
	// Para ponerlo en vertical.
	glRotatef(90, -1, 0, 0);
	// Se crea el cilindro.
	glutWireCylinder(0.025, 0.85, 10, 1);

	glEndList();
#pragma endregion

#pragma region cabina
	// Se crean dos listas que mas adelante serviran para
	// crear los soportes de las cabinas.
	soporteCabina1 = glGenLists(1);
	glNewList(soporteCabina1, GL_COMPILE);

	glBegin(GL_POLYGON);
	glColor3f(0.45, 0.45, 0.45);
	glutSolidCylinder(0.015, 0.835, 32, 32);
	glEnd();

	glEndList();

	soporteCabina2 = glGenLists(1);
	glNewList(soporteCabina2, GL_COMPILE);

	glBegin(GL_POLYGON);
	glColor3f(0.5, 0, 0);
	glutSolidCylinder(0.025, 0.45, 32, 32);
	glEnd();

	glEndList();

	// Se crea una lista que mas adelante servira para
	// crear las partes de arriba de las cabinas.
	cabinaArriba = glGenLists(1);
	glNewList(cabinaArriba, GL_COMPILE);

	glPushMatrix();
	glColor4f(0.1, 0.2, 0.9, 0.15);
	glutSolidCube(0.15);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.1, 0.2, 0.9);
	glutWireCube(0.152);
	glPopMatrix();

	glEndList();

	// Se crea una lista que mas adelante servira para
	// crear las partes de abajo de las cabinas.
	cabinaAbajo = glGenLists(1);
	glNewList(cabinaAbajo, GL_COMPILE);

	glPushMatrix();
	glColor3f(0.5, 0, 0);
	glutSolidCube(0.155);
	glPopMatrix();

	glEndList();

	// Se crea una lista que mas adelante servira
	// para crear las puertas de las cabinas.
	puertas = glGenLists(1);
	glNewList(puertas, GL_COMPILE);

	glPushMatrix();
	glColor3f(0.5, 0, 0);
	glutSolidCube(0.152);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.3, 0, 0);
	glutWireCube(0.152);
	glPopMatrix();

	glEndList();
#pragma endregion

}

void display(void) {
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glClearColor(0.54, 0.61, 0.8, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(7 * cos(-anguloTiempo * PI / 180), 0.5 * sin(-anguloTiempo * PI / 180), 7 * sin(-anguloTiempo * PI / 180), 0, 0, 0, 0, 1, 0);
	// ejes();

	// Base de la noria.
	glPushMatrix();
	glColor3f(0.22, 0.42, 0.22);
	glTranslatef(0, -3.3, 0);
	glScalef(80, 2, 80);
	glutSolidCube(2);
	glPopMatrix();

	// Parte frontal de la noria.
	glPushMatrix();
	glRotatef(anguloTiempo, 0, 0, 1);
	crearRadios();
	glPopMatrix();

	glPushMatrix();
	glRotatef(anguloTiempo, 0, 0, 1);
	glCallList(noria);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.45, 0.45, 0.45);
	glScalef(1, 1, 1.5);
	glutSolidSphere(0.1, 64, 64);
	glPopMatrix();

	// Parte trasera de la noria.
	glPushMatrix();
	glTranslatef(0, 0, 0.5);
	glRotatef(anguloTiempo, 0, 0, 1);
	crearRadios();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 0, 0.5);
	glRotatef(180, 0, 1, 0);
	glRotatef(anguloTiempo, 0, 0, 1);
	glCallList(noria);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 0, 0.5);
	glColor3f(0.45, 0.45, 0.45);
	glScalef(1, 1, 1.5);
	glutSolidSphere(0.1, 64, 64);
	glPopMatrix();

	//Soportes de la noria.
	crearSoportesNoria();

	// Cabinas.
	glPushMatrix();
	glRotatef(anguloTiempo, 0, 0, 1);
	crearCabinas();
	glPopMatrix();

	glutSwapBuffers();
	glFlush();

	FPS();
}

void reshape(GLint w, GLint h)
// Funcion de atencion al redimensionamiento
{
	float relacionAspecto = float(w) / h;
	glViewport(0, 0, w, h);

	// Definimos la camara (matriz de proyeccion).
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(30, relacionAspecto, 0.1, 1000);
}

void onIdle() 
{
	static const float vueltasPorSegundo = 0.05;
	static int antes = glutGet(GLUT_ELAPSED_TIME);
	int ahora = glutGet(GLUT_ELAPSED_TIME);
	anguloTiempo += vueltasPorSegundo * 360 * (ahora - antes) / 1000;

	antes = ahora;

	glutPostRedisplay();
}

void onTimer(int tiempo) 
{
	//Callback de atencion a la cuenta atras
	glutTimerFunc(tiempo, onTimer, tiempo);

	onIdle();
}

void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE); // Alta de buffers a usar
	glutInitWindowSize(600, 600);							  // Tamanyo inicial de la ventana
	glutCreateWindow(PROYECTO);								  // Creacion de la ventana con su titulo
	init();													  // Inicializacion propia.
	std::cout << PROYECTO << " running" << std::endl;		  // Mensaje por consola
	glutDisplayFunc(display);								  // Alta de la funcion de atencion a display
	glutReshapeFunc(reshape);								  // Alta de la funcion de atencion a reshape
	glEnable(GL_DEPTH_TEST);
	glutTimerFunc(1000 / tasaFPS, onTimer, 1000 / tasaFPS);
	glutMainLoop();											  // Puesta en marcha del programa
}
