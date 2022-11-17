/*
*	Practica2.cpp
*	Codigo para la realizacion de 
*   la estrella de david
*
*	@author	Mireia Pires State
*/

#define PROYECTO "ISGI::P02::Estrella de David"
#define PI 3.1415926

// Bibliotecas de C++
#include <iostream>
#include <cmath>

// Bibliotecas graficas
#include <GL/freeglut.h>

static GLuint estrella;

void init()
{
	estrella = glGenLists(1);

	glNewList(estrella, GL_COMPILE);

		glBegin(GL_TRIANGLE_STRIP);
		for (int i = 0; i < 4; i++)
		{
			double angle = (1.0 + (i * 4) % 12) * PI / 6;
			glVertex3f(1.0 * cos(angle), 1.0 * sin(angle), 0.0);
			glVertex3f(0.7 * cos(angle), 0.7 * sin(angle), 0.0);
		}
		glEnd();

		glBegin(GL_TRIANGLE_STRIP);
		for (int i = 0; i < 4; i++)
		{
			double angle = (3.0 + (i * 4) % 12) * PI / 6;
			glVertex3f(1.0 * cos(angle), 1.0 * sin(angle), 0.0);
			glVertex3f(0.7 * cos(angle), 0.7 * sin(angle), 0.0);
		}
		glEnd();
	glEndList();
}

void display()
{
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT); // Borra la pantalla
	// Dibujo como POLYGON relleno
	glColor3f(0.0, 0.0, 0.3);
	glCallList(estrella);
	// Dibujo como POLYGON solo aristas
	glFlush(); // Finaliza el dibujo
}

void reshape(GLint w, GLint h)
{
	// w,h son las nuevas dimensiones de la ventana
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);								// Inicializacion de GLUT
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);		// Alta de buffers a usar
	glutInitWindowSize(400, 400);						// Tamanyo inicial de la ventana
	glutCreateWindow(PROYECTO);							// Creacion de la ventana con su titulo
	init();
	std::cout << PROYECTO << " running" << std::endl;   // Mensaje por consola
	glutDisplayFunc(display);							// Alta de la funcion de atencion a display
	glutReshapeFunc(reshape);							// Alta de la funcion de atencion a reshape
	glutMainLoop();										// Puesta en marcha del programa
}