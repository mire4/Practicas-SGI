/***************************************************
ISGI::Pentagono como display list
Roberto Vivo', 2013 (v1.0)
Dibujo de un pentagono con vertices y aristas diferenciados
como display list
Dependencias:
+GLUT
***************************************************/
#define PROYECTO "Mosaico"
#include <iostream> // Biblioteca de entrada salida
#include <cmath> // Biblioteca matem�tica de C
#include <GL/freeglut.h> // Biblioteca grafica

#define _USE_MATH_DEFINES 
#include <math.h> 
static GLuint estrella; // Identificador del objeto
static GLuint estrellaDoble;
void init()
// Funcion de inicializacion propia
{
	estrella = glGenLists(1); // Obtiene el identificador de la lista
	estrellaDoble = glGenLists(2);
	glNewList(estrella, GL_COMPILE); // Abre la lista
	// Dibuja el pentagono en la lista
	glBegin(GL_TRIANGLE_STRIP);
	glVertex3f(1.0 * cos(0 * (2 * M_PI / 6) + (M_PI / 2)), 1.0 * sin(0 * (2 * M_PI / 6) + (M_PI / 2)), 0.0);
	glVertex3f(0.7 * cos(0 * (2 * M_PI / 6) + (M_PI / 2)), 0.7 * sin(0 * (2 * M_PI / 6) + (M_PI / 2)), 0.0);
	glVertex3f(1.0 * cos(2 * (2 * M_PI / 6) + (M_PI / 2)), 1.0 * sin(2 * (2 * M_PI / 6) + (M_PI / 2)), 0.0);
	glVertex3f(0.7 * cos(2 * (2 * M_PI / 6) + (M_PI / 2)), 0.7 * sin(2 * (2 * M_PI / 6) + (M_PI / 2)), 0.0);
	glVertex3f(1.0 * cos(4 * (2 * M_PI / 6) + (M_PI / 2)), 1.0 * sin(4 * (2 * M_PI / 6) + (M_PI / 2)), 0.0);
	glVertex3f(0.7 * cos(4 * (2 * M_PI / 6) + (M_PI / 2)), 0.7 * sin(4 * (2 * M_PI / 6) + (M_PI / 2)), 0.0);
	glVertex3f(1.0 * cos(0 * (2 * M_PI / 6) + (M_PI / 2)), 1.0 * sin(0 * (2 * M_PI / 6) + (M_PI / 2)), 0.0);
	glVertex3f(0.7 * cos(0 * (2 * M_PI / 6) + (M_PI / 2)), 0.7 * sin(0 * (2 * M_PI / 6) + (M_PI / 2)), 0.0);
	glEnd();
	glBegin(GL_TRIANGLE_STRIP);
	glVertex3f(1.0 * cos(1 * (2 * M_PI / 6) + (M_PI / 2)), 1.0 * sin(1 * (2 * M_PI / 6) + (M_PI / 2)), 0.0);
	glVertex3f(0.7 * cos(1 * (2 * M_PI / 6) + (M_PI / 2)), 0.7 * sin(1 * (2 * M_PI / 6) + (M_PI / 2)), 0.0);
	glVertex3f(1.0 * cos(3 * (2 * M_PI / 6) + (M_PI / 2)), 1.0 * sin(3 * (2 * M_PI / 6) + (M_PI / 2)), 0.0);
	glVertex3f(0.7 * cos(3 * (2 * M_PI / 6) + (M_PI / 2)), 0.7 * sin(3 * (2 * M_PI / 6) + (M_PI / 2)), 0.0);
	glVertex3f(1.0 * cos(5 * (2 * M_PI / 6) + (M_PI / 2)), 1.0 * sin(5 * (2 * M_PI / 6) + (M_PI / 2)), 0.0);
	glVertex3f(0.7 * cos(5 * (2 * M_PI / 6) + (M_PI / 2)), 0.7 * sin(5 * (2 * M_PI / 6) + (M_PI / 2)), 0.0);
	glVertex3f(1.0 * cos(1 * (2 * M_PI / 6) + (M_PI / 2)), 1.0 * sin(1 * (2 * M_PI / 6) + (M_PI / 2)), 0.0);
	glVertex3f(0.7 * cos(1 * (2 * M_PI / 6) + (M_PI / 2)), 0.7 * sin(1 * (2 * M_PI / 6) + (M_PI / 2)), 0.0);
	glEnd();
	glEndList(); // Cierra la lista
	glNewList(estrellaDoble, GL_COMPILE); // Abre la lista
	glPushMatrix();
	glScalef(0.5, 0.5, 0.5);
	glCallList(estrella);
	glRotatef(30, 0, 0, 1);
	glScalef(0.4, 0.4, 0.4);
	glCallList(estrella);
	glPopMatrix();
	glPopMatrix();
	glEndList(); // Cierra la lista
}
void display()
// Funcion de atencion al dibujo
{
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT); // Borra la pantalla
	// Dibujo como POLYGON relleno
	glMatrixMode(GL_MODELVIEW); // Selecciona la modelview
	//glLoadIdentity(); // Carga la matriz identidad
	glColor3f(0.0, 0.0, 0.8);
	glPushMatrix();
	glTranslatef(-0.5, -0.5, 0);
	glRotatef(-15, 0, 0, 1);
	glCallList(estrellaDoble);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.5, 0.5, 0);
	glRotatef(-15, 0, 0, 1);
	glCallList(estrellaDoble);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-0.5, 0.5, 0);
	glRotatef(15, 0, 0, 1);
	glCallList(estrellaDoble);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.5, -0.5, 0);
	glRotatef(15, 0, 0, 1);
	glCallList(estrellaDoble);
	glPopMatrix();
	// Dibujo como POLYGON solo aristas
	glFlush(); // Finaliza el dibujo
}
void reshape(GLint w, GLint h)
// Funcion de atencion al redimensionamiento
{
}
void main(int argc, char** argv)
// Programa principal
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); // Alta de buffers a usar
	glutInitWindowSize(400, 400); // Tamanyo inicial de la ventana
	glutCreateWindow(PROYECTO); // Creacion de la ventana con su titulo
	init(); // Inicializacion propia. IMPORTANTE SITUAR AQUI
	std::cout << PROYECTO << " running" << std::endl; // Mensaje por consola
	glutDisplayFunc(display); // Alta de la funcion de atencion a display
	glutReshapeFunc(reshape); // Alta de la funcion de atencion a reshape
	glutMainLoop(); // Puesta en marcha del programa
}