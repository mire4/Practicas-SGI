/***************************************************
ISGI::Noria
Roberto Vivo', 2013 (v1.0)
Dimulación del movimiento de una noria
Dependencias:
+GLUT
***************************************************/
#define PROYECTO "Noria3D"
#include <iostream> // Biblioteca de entrada salida
#include <cmath> // Biblioteca matematica de C
#include <GL/freeglut.h> // Biblioteca grafica

#define _USE_MATH_DEFINES 
#include <math.h> 
static GLuint noria;

void init()
{
	// Se crea una lista que contiene todas las piezas de la noria.
	noria = glGenLists(1)
	glNewList(noria, GL_COMPILE);
	// Color por defecto de los componentes (gris).
	glColor3f(0.6, 0.6, 0.6);

	// Se crea la pieza central de la noria.
	glBegin(GL_POLYGON);
	glVertex3f(0, 0, 15); //Posicionamos la pieza
	glutSolidCylinder(0.5, 15, 0, 0);

	// Se crea la pieza de la rueda. 
	glBegin(GL_POLYGON);
	glutWireTorus(0.65, 0.75, 28, 28);
	glEnd();

	
}
void display()
// Funcion de atencion al dibujo
{
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Lugar y orientacion de la camara
			  // los 3 primeros son la cercania al punto de enfonque
			  // los 3 de en medio son hacia donde mira
			  // los 3 ultimos es la orientacion de la camara
	gluLookAt(2, 2, 2, 0, 0, 0, 0, 1, 0);

	glEnable(GL_DEPTH_TEST);

	glPushMatrix();

	// Generamos las estrellas
	for (int i = 0; i < 6; i++) {
		glColor3f(0.8 - (i / 10.0), 1 - (i / 10.0), 0.2 + (i / 10.0));
		glRotatef(30, 0, 1, 0);
		glCallList(estrella);
	}

	glPopMatrix();


	// Wired esfera
	glColor3f(0, 0, 1);
	glutWireSphere(1, 20, 20);


	glFlush();
}
void reshape(GLint w, GLint h)
// Funcion de atencion al redimensionamiento
{
	glViewport(0, 0, w, h);

	// Relacion de aspecto
	float ra = float(w) / float(h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	float distancia = sqrt(pow(2, 2) + pow(2, 2) + pow(2, 2));
	float angulo = (asin(1 / (distancia / 2.0))) * 180 / M_PI;

	gluPerspective(angulo, ra, 1, 10);
}
void main(int argc, char** argv)
// Programa principal
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH); // Alta de buffers a usar
	glutInitWindowSize(400, 400); // Tamanyo inicial de la ventana
	glutCreateWindow(PROYECTO); // Creacion de la ventana con su titulo
	init(); // Inicializacion propia. IMPORTANTE SITUAR AQUI
	std::cout << PROYECTO << " running" << std::endl; // Mensaje por consola
	glutDisplayFunc(display); // Alta de la funcion de atencion a display
	glutReshapeFunc(reshape); // Alta de la funcion de atencion a reshape
	glutMainLoop(); // Puesta en marcha del programa
}
