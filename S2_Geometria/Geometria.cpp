/*!
	Geometria.cpp
	
	Programa para dibujar un pentagono con listas

	@author		Roberto Vivo' <rvivo@upv.es>
	@date		Oct,2022
 */

#define PROYECTO "ISGI::S2E02::Listas de dibujo"

#include <iostream>			
#include <Utilidades.h>

using namespace std;

static GLuint pentagono;
static GLfloat coordenadas[10];

void init()
// Inicializaciones
{
	cout << "Iniciando " << PROYECTO << endl;
	cout << "GL version " << glGetString(GL_VERSION) << endl;

	float radio = 1;
	for (int i = 0; i < 10; i += 2) {
		coordenadas[i] = radio * cosf((float(i) / 2) * 2 * PI / 5);
		coordenadas[i + 1] = radio * sinf((float(i) / 2) * 2 * PI / 5);
	}

	// Lista de dibujo
	pentagono = glGenLists(1);			

	glNewList(pentagono, GL_COMPILE);
		glBegin(GL_POLYGON);
		for (int i = 0; i < 10; i+=2) {
			glVertex3f(coordenadas[i], coordenadas[i+1], 0);
		}
		glEnd();
	glEndList();

	// Inicializaciones 
	glClearColor(0.0f, 0.0f, 0.3f, 1.0f);
}

void display()
// Funcion de atencion al dibujo
{
	glClear(GL_COLOR_BUFFER_BIT);

	ejes();

	// Uso de Display Lists
	glPolygonMode(GL_FRONT, GL_FILL);
	glColor3f(1, 0, 0);
	glCallList(pentagono);
		
	glPushAttrib(GL_CURRENT_BIT|GL_LINE_BIT);
	glLineWidth(4);
	glPolygonMode(GL_FRONT, GL_LINE);
	glColor3f(1, 1, 1);
	glCallList(pentagono);
	glPopAttrib();

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