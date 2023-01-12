/*!
	Iluminacion.cpp

	Programa para manejar luces y materiales en OpenGL

	@author		Roberto Vivo' <rvivo@upv.es>
	@date		Nov,2022
 */

#define PROYECTO "ISGI::S7E01::Iluminacion"

#include <iostream>			
#include <Utilidades.h>

using namespace std;

// Globales
static float girox = 0, giroy = 0;
int xanterior, yanterior;
bool L1on = true, L0on = true;



void init()
// Inicializaciones
{
	cout << "Iniciando " << PROYECTO << endl;
	cout << "GL version " << glGetString(GL_VERSION) <<
		endl;

	// Luces --------------------------------
	const GLfloat A[] = { 0.2,0.2,0.2,1 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, A);

	const GLfloat L0D[]{ 0.2,0.2,0.2,1 };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, L0D);
	
	glLightfv(GL_LIGHT1, GL_DIFFUSE, GRISCLARO);
	glLightfv(GL_LIGHT1, GL_SPECULAR, GRISCLARO);

	// Material -----------------------------
	glMaterialfv(GL_FRONT, GL_DIFFUSE, BRONCE);
	glMaterialfv(GL_FRONT, GL_SPECULAR, ORO);
	glMaterialf(GL_FRONT, GL_SHININESS, 40);

	// Configurar el motor de render 
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_NORMALIZE);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
}

void display()
// Funcion de atencion al dibujo
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Seleccionar la MODELVIEW
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Luces solidarias a la cámara
	GLfloat L0P[]{ 0,0,1,0 };
	glLightfv(GL_LIGHT0, GL_POSITION, L0P);


	// Situar y orientar la camara
	gluLookAt(2, 1, 2, 0, 0, 0, 0, 1, 0);

	ejes();

	// Luz fija en la escena
	GLfloat L1P[]{ -20,20,20,1 };
	glLightfv(GL_LIGHT1, GL_POSITION, L1P);

	// Geometria

	// Tetera roja
	glPushMatrix();

	glRotatef(girox, 1, 0, 0);
	glRotatef(giroy, 0, 1, 0);

	// Luz solidaria al objeto
	//--GLfloat L0P[]{ 0,1,0,0 };
	//--glLightfv(GL_LIGHT0, GL_POSITION, L0P);

	glutSolidTeapot(0.5);

	glPopMatrix();

	glutSwapBuffers();
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

void onKey(unsigned char tecla, int x, int y)
{
	// Callback de atencion a los eventos de teclas alfanumericas
	switch (tecla) {
	case '0':
		if (L0on)glDisable(GL_LIGHT0);
		else glEnable(GL_LIGHT0);
		L0on = !L0on;
		break;
	case '1':
		if (L1on)glDisable(GL_LIGHT1);
		else glEnable(GL_LIGHT1);
		L1on = !L1on;
		break;
	case 27:
		exit(0);
	}

	glutPostRedisplay();
}

void onClick(int boton, int estado, int x, int y)
{
	// Callback de atencion al pulsado de un boton del raton

	// Almacenar donde se hizo el click
	if (boton == GLUT_LEFT_BUTTON && estado == GLUT_DOWN) {
		xanterior = x;
		yanterior = y;
	}

}

void onDrag(int x, int y)
{
	static const float pixel2grados = 1;

	giroy += (x - xanterior) * pixel2grados;
	girox += (y - yanterior) * pixel2grados;

	xanterior = x;
	yanterior = y;

	glutPostRedisplay();
}

int main(int argc, char** argv)
// Programa principal
{
	// Inicializaciones
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(600, 600);
	glutCreateWindow(PROYECTO);
	init();

	// Registro de callbacks	
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(onKey);
	glutMouseFunc(onClick);
	glutMotionFunc(onDrag);

	// Bucle de atencion a eventos
	glutMainLoop();
}