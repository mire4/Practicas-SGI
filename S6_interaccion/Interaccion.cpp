/*!
	Interaccion.cpp

	Programa capturar eventos de usuario y modificar el
	grafico en consecuencia

	@author		Roberto Vivo' <rvivo@upv.es>
	@date		Nov,2022
 */

#define PROYECTO "ISGI::S6E01::Interaccion"

#include <iostream>			
#include <Utilidades.h>

using namespace std;

// Globales
static enum {ALAMBRICO,SOLIDO,DOBLE} modo;
static float giroxTR = 0, giroyTR = 0, giroxTA = 0, giroyTA = 0;
int xanterior, yanterior;
GLubyte objeto[1];

void onMenu(int valor)
{
	if (valor == 0) modo = ALAMBRICO;
	else if (valor == 1) modo = SOLIDO;
	else modo = DOBLE;

	glutPostRedisplay();
}
void init()
// Inicializaciones
{
	cout << "Iniciando " << PROYECTO << endl;
	cout << "GL version " << glGetString(GL_VERSION) << endl;

	// Menu de popup
	
	glutCreateMenu(onMenu);
	glutAddMenuEntry("ALAMBRICO", 0);
	glutAddMenuEntry("SOLIDO", 1);
	glutAddMenuEntry("DOBLE", 2);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	// Inicializaciones 
	glClearColor(0.0f, 0.0f, 0.3f, 1.0f);
	glEnable(GL_DEPTH_TEST);
}

void displaySinMostrar()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Seleccionar la MODELVIEW
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Situar y orientar la camara
	gluLookAt(2, 1, 2, 0, 0, 0, 0, 1, 0);

	// Geometria

	// Tetera roja
	glPushMatrix();

	glRotatef(giroxTR, 1, 0, 0);
	glRotatef(giroyTR, 0, 1, 0);

	glColor3fv(ROJO);
	glutSolidTeapot(0.5);

	glPopMatrix();

	// Tetera azul
	glPushMatrix();
	glTranslatef(-1, 0, -1);
	glRotatef(giroxTA, 1, 0, 0);
	glRotatef(giroyTA, 0, 1, 0);

	glColor3fv(AZUL);
	glutSolidTeapot(0.5);

	glPopMatrix();

}

void display()
// Funcion de atencion al dibujo
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Seleccionar la MODELVIEW
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Situar y orientar la camara
	gluLookAt(2, 1, 2, 0, 0, 0, 0, 1, 0);

	ejes();

	// Geometria

	// Tetera roja
	glPushMatrix();

	glRotatef(giroxTR, 1, 0, 0);
	glRotatef(giroyTR, 0, 1, 0);

	if (modo == ALAMBRICO) {
		glColor3fv(ROJO);
		glutWireTeapot(0.5);
	}
	else if (modo == SOLIDO) {
		glColor3fv(ROJO);
		glutSolidTeapot(0.5);
	}
	else {
		glColor3fv(ROJO);
		glutSolidTeapot(0.5);
		glColor3fv(BLANCO);
		glutWireTeapot(0.51);
	}

	glPopMatrix();

	// Tetera azul
	glPushMatrix();
	glTranslatef(-1, 0, -1);
	glRotatef(giroxTA, 1, 0, 0);
	glRotatef(giroyTA, 0, 1, 0);
	if (modo == ALAMBRICO) {
		glColor3fv(AZUL);
		glutWireTeapot(0.5);
	}
	else if (modo == SOLIDO) {
		glColor3fv(AZUL);
		glutSolidTeapot(0.5);
	}
	else {
		glColor3fv(AZUL);
		glutSolidTeapot(0.5);
		glColor3fv(BLANCO);
		glutWireTeapot(0.51);
	}
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
	case 'a':
		modo = ALAMBRICO;
		break;
	case 's':
		modo = SOLIDO;
		break;
	case 'd':
		modo = DOBLE;
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

	// Para preguntar por el pixel tengo que cambiar la Y de sentido
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	GLfloat vy = viewport[3] - y;
	displaySinMostrar();
	glReadPixels(x, vy, 1, 1, GL_RED, GL_UNSIGNED_BYTE, objeto);
}

void onDrag(int x, int y)
{
	static const float pixel2grados = 1;

	if (objeto[0] == 0xFF) { // Tetera roja
		// Al mover el raton hacia la derecha, la x aumenta y el giro es 
		// alrededor del eje y positivo
		giroyTR += (x - xanterior) * pixel2grados;
		// AL mover el raton hacia abajo, la y aumenta y el giro es 
		// alrededor del eje x positivo
		giroxTR += (y - yanterior) * pixel2grados;
	}
	else {
		// Al mover el raton hacia la derecha, la x aumenta y el giro es 
		// alrededor del eje y positivo
		giroyTA += (x - xanterior) * pixel2grados;
		// AL mover el raton hacia abajo, la y aumenta y el giro es 
		// alrededor del eje x positivo
		giroxTA += (y - yanterior) * pixel2grados;
	}

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