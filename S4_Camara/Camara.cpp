/*!
	Camara.cpp

	Programa para dibujar dos teteras con visibilidad y
	camaras diferentes

	@author		Roberto Vivo' <rvivo@upv.es>
	@date		Oct,2022
 */

#define PROYECTO "ISGI::S4E01::Camaras"

#include <iostream>			
#include <Utilidades.h>

using namespace std;


void init()
// Inicializaciones
{
	cout << "Iniciando " << PROYECTO << endl;
	cout << "GL version " << glGetString(GL_VERSION) << endl;

	// Inicializaciones 
	glClearColor(0.0f, 0.0f, 0.3f, 1.0f);
	glEnable(GL_DEPTH_TEST);
}

void display()
// Funcion de atencion al dibujo
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	// Seleccionar la MODELVIEW
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Situar y orientar la camara
	gluLookAt(2, 1, 2, 0, 0, 0, 0,1,0);

	ejes();

	glColor3f(1, 0, 0);
	glutSolidTeapot(0.5);
	glColor3fv(BLANCO);
	glutWireTeapot(0.51);

	glPushMatrix();
	glTranslatef(-1, 0, -1);
	glColor3f(0, 0, 1);
	glutSolidTeapot(0.5);
	glColor3fv(AMARILLO);
	glutWireTeapot(0.51);
	glPopMatrix();

	glFlush();
}

void reshape(GLint w, GLint h)
// Funcion de atencion al redimensionamiento
{
	float relacionAspecto = float(w) / h;
	glViewport(0, 0, w, h);

	// Definir la camara
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	/*
	if(relacionAspecto>1)
		glOrtho(-2*relacionAspecto, 2*relacionAspecto, -2, 2, -20, 20);
	else
		glOrtho(-2, 2, -2/relacionAspecto, 2/relacionAspecto, -20, 20);
	*/

	gluPerspective(30, relacionAspecto, 0.1, 100);
}

int main(int argc, char** argv)
// Programa principal
{
	// Inicializaciones
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(600, 600);
	glutCreateWindow(PROYECTO);
	init();

	// Registro de callbacks	
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);

	// Bucle de atencion a eventos
	glutMainLoop();
}