/*!
	Tranformaciones.cpp

	Programa para dibujar una flor con lista y transformaciones

	@author		Roberto Vivo' <rvivo@upv.es>
	@date		Oct,2022
 */

#define PROYECTO "ISGI::S3E01::Flor"

#include <iostream>			
#include <Utilidades.h>

using namespace std;

// Globales
static GLuint petalo, corola, flor;

void init()
// Inicializaciones
{
	cout << "Iniciando " << PROYECTO << endl;
	cout << "GL version " << glGetString(GL_VERSION) << endl;

	// Petalo
	petalo = glGenLists(1);
	glNewList(petalo, GL_COMPILE);
	glPushAttrib(GL_CURRENT_BIT);
	glPushMatrix();
	glScalef(0.15, 0.5, 0.15);
	glColor3f(1, 1, 1);
	glutSolidSphere(1, 20, 20);
	glPopMatrix();
	glPopAttrib();
	glEndList();

	// Corola
	corola = glGenLists(1);
	glNewList(corola, GL_COMPILE);
	glPushAttrib(GL_CURRENT_BIT);

	for (int i = 0; i < 12; i++) {
		glPushMatrix();
		glRotatef(i * 360.0 / 12, 0, 0, 1);
		glTranslatef(0.0, 0.25, 0.0);
		glScalef(0.5, 0.5, 0.5);
		glCallList(petalo);
		glPopMatrix();
	}

	glColor3f(1, 1, 0);
	glPushMatrix();
	glScalef(0.2, 0.2, 0.2);
	glutSolidSphere(1, 20, 20);
	glPopMatrix();

	glPopAttrib();
	glEndList();

	// flor
	flor = glGenLists(1);
	glNewList(flor, GL_COMPILE);
	glPushAttrib(GL_CURRENT_BIT);

	//-tallo
	glPushMatrix();
	glColor3f(0, 1, 0);
	glTranslatef(0.0, -0.125, 0);
	glScalef(0.05, 0.75, 0.05);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, 0.25, 0.0);
	glScalef(0.5, 0.5, 0.5);
	glRotatef(15, 0, 0, 1);
	glCallList(corola);
	glPopMatrix();

	glPopAttrib();
	glEndList();


	// Inicializaciones 
	glClearColor(0.0f, 0.0f, 0.3f, 1.0f);
}

void display()
// Funcion de atencion al dibujo
{
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//ejes();
	for (int i = 0; i < 100; i++) {
		glPushMatrix();
		glTranslatef((rand() % 200 - 100) / 100.0, (rand() % 100 - 50) / 100.0, 0);
		glScalef(0.7, 0.7, 0.7);
		glCallList(flor);
		glPopMatrix();
	}

	glFlush();
}

void reshape(GLint w, GLint h)
// Funcion de atencion al redimensionamiento
{
}

int main(int argc, char** argv)
// Programa principal
{
	// Inicializaciones
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(600, 600);
	glutCreateWindow(PROYECTO);
	init();

	// Registro de callbacks	
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);

	// Bucle de atencion a eventos
	glutMainLoop();
}