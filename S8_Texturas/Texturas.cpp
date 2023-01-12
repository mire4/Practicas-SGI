/*!
	Texturas.cpp

	Programa ejemplo de aplicacion de texturas en OpenGL

	@author		Roberto Vivo' <rvivo@upv.es>
	@date		Nov,2022
 */

#define PROYECTO "ISGI::S8E01::Texturas"

#include <iostream>			
#include <Utilidades.h>

using namespace std;

// Globales
static float girox = 0, giroy = 0;
int xanterior, yanterior;
bool L1on = true, L0on = true;
GLuint erre, nubes;



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

	// Texturas -----------------------------
	glGenTextures(1, &erre);
	glBindTexture(GL_TEXTURE_2D, erre);
	loadImageFile((char*)"erre.jpg");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glGenTextures(1, &nubes);
	glBindTexture(GL_TEXTURE_2D, nubes);
	loadImageFile((char*)"nubes.jpg");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// Configurar el motor de render 
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_NORMALIZE);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_TEXTURE_2D);
}

void display()
// Funcion de atencion al dibujo
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindTexture(GL_TEXTURE_2D, nubes);
	texturarFondo();

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

	// Tetera
	glPushMatrix();

	glRotatef(girox, 1, 0, 0);
	glRotatef(giroy, 0, 1, 0);

	// Luz solidaria al objeto
	//--GLfloat L0P[]{ 0,1,0,0 };
	//--glLightfv(GL_LIGHT0, GL_POSITION, L0P);

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	glBindTexture(GL_TEXTURE_2D, nubes);

	//glutSolidTeapot(0.5);

	// Para el toro generar automaticamente las coordenadas de textura
	// con un mapa esferico (de entorno)
	glPushAttrib(GL_ALL_ATTRIB_BITS);

	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);

	glutSolidTorus(0.2, 0.5, 40, 40);

	glPopAttrib();

	glBindTexture(GL_TEXTURE_2D, erre);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	glPushMatrix();
	glTranslatef(0, -0.5, 0);
	glRotatef(-90, 1, 0, 0);
	glBegin(GL_POLYGON);

	glNormal3f(0, 0, 1);

	glTexCoord2f(2, 0);
	glVertex3f(-0.5, -0.5, 0);
	glTexCoord2f(0, 0);
	glVertex3f(0.5, -0.5, 0);
	glTexCoord2f(0, 2);
	glVertex3f(0.5, 0.5, 0);
	glTexCoord2f(2, 2);
	glVertex3f(-0.5, 0.5, 0);

	glEnd();
	glPopMatrix();

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
	FreeImage_Initialise();
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

	FreeImage_DeInitialise();
}