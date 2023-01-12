/*!
	Animacion.cpp

	Programa para manejar la variable temporal y
	actualizar la escena en cada frame

	@author		Roberto Vivo' <rvivo@upv.es>
	@date		Oct,2022
 */

#define PROYECTO "ISGI::S5E01::Animacion"

#include <iostream>		
#include <sstream>
#include <Utilidades.h>

// Variables dependientes del tiempo
static float angulo = 0;

static const int tasaFPS = 60;

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

void FPS()
{
	// Cuenta los fotogramas por segundo y los muestra en
	// la barra de titulo de la ventana

	static int antes = glutGet(GLUT_ELAPSED_TIME);
	static int fotogramas = 0;
	int ahora, tiempoTranscurrido;

	fotogramas++;

	ahora = glutGet(GLUT_ELAPSED_TIME);
	tiempoTranscurrido = (ahora - antes) / 1000;

	// si ha transcurrido mas de un segundo, muestro los FPS y reinicio
	// la cuenta y el reloj
	if (tiempoTranscurrido >= 1) {
		stringstream titulo;
		titulo << "FPS= " << fotogramas;
		glutSetWindowTitle(titulo.str().c_str());
		// reinicio
		antes = ahora;
		fotogramas = 0;
	}
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

	glPushMatrix();
	glRotatef(angulo, 0, 1, 0);
	glColor3f(1, 0, 0);
	glutSolidTeapot(0.5);
	glColor3fv(BLANCO);
	glutWireTeapot(0.51);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1, 0, -1);
	glRotatef(angulo / 2, 0, 1, 0);
	glColor3f(0, 0, 1);
	glutSolidTeapot(0.5);
	glColor3fv(AMARILLO);
	glutWireTeapot(0.51);
	glPopMatrix();

	glutSwapBuffers();

	FPS();
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

void onIdle()
{
	// Callback de atencion al evento idle (sin faena)

	// sin control del tiempo
	///angulo += 0.1;

	// con control del tiempo
	static const float vueltasPorSegundo = 1;
	static int antes = glutGet(GLUT_ELAPSED_TIME);
	int ahora = glutGet(GLUT_ELAPSED_TIME);
	angulo += vueltasPorSegundo * 360 * (ahora - antes) / 1000;

	antes = ahora;

	glutPostRedisplay();

}

void onTimer(int tiempo)
{
	// Callback de atencion a la cuenta atras
	glutTimerFunc(tiempo, onTimer, tiempo);

	onIdle();
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
	//glutIdleFunc(onIdle);
	glutTimerFunc(1000 / tasaFPS, onTimer, 1000 / tasaFPS);

	// Bucle de atencion a eventos
	glutMainLoop();
}