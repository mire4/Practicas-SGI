/*!
	Simulador.cpp
	Simulador de vuelo
	@author		Mireia Pires State
	@date		Diciembre, 2022
 */

#define PROYECTO "ISGI::Simulador"

#include <iostream>
#include <Utilidades.h>

using namespace std;

// Variables globales:
// Caracteristicas del terreno.
const int EXTENSION = 50;
const int RESOLUCION_TERRENO = 50;
const float LADO = (float) EXTENSION / (float) RESOLUCION_TERRENO;
static int HONDO;
static float puntoMin = 2147483647;
static float puntoMax = -2147483647;

// Variables para el calculo de la altura.
const int seed = time(NULL);
const float dureza = 0.15f;
const int nFunciones = 4;
const float amplitud = 5.0f;

void init()
{
	cout << "Iniciando " << PROYECTO << endl;
	cout << "GL version " << glGetString(GL_VERSION) << endl;

	// Inicializaciones 
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.54, 0.61, 0.8, 1);
} 

#pragma region CalculoDeLaAltura
/* Para el calculo de la altura se ha tomado como punto
 de partida el ruido en escala de grises, pero generandolo 
 mediante numeros aleatorios. 

 Para obtener los numeros aleatorios se toma como semilla la 
 fecha del equipo, asi cada vez que se ejecute el simulador se 
 crea un terreno diferente. 
 
 Finalmente, se procesa la altura generada mediante una funcion
 de interpolacion para crear el Perlin Noise.*/

// Metodo que devuelve una altura con valor entre -1 y 1.
// Se tiene en cuenta el -1 para poder crear valles, lagos, etc.
float obtenerAltura(int x, int y) {
	srand((unsigned int)seed * x * y);
	float res = float(rand()) / float((RAND_MAX)) * 2.0f - 1.0f;
	return res;
}

// Metodo que devuelve la suma de la altura del vertice 
// que se mira mas de los que estan a su alrededor.
float obtenerAlturaSuavizada(int x, int y) {
	float esquinas = (obtenerAltura(x - LADO, y + LADO) + obtenerAltura(x + LADO, y + LADO)
		+ obtenerAltura(x - LADO, y - LADO) + obtenerAltura(x + LADO, y - LADO)) / 16.0f;
	float lados = (obtenerAltura(x - LADO, y) + obtenerAltura(x + LADO, y)
		+ obtenerAltura(x, y + LADO) + obtenerAltura(x, y - LADO)) / 8.0f;
	float centro = obtenerAltura(x, y) / 4.0f;
	return esquinas + lados + centro;
}

// Metodo que devuelve la interpolacion entre dos 
// alturas dadas con la funcion del coseno.
float interpolar(float h1, float h2, float suavizado) {
	double theta = suavizado * PI;
	float f = (float)((1.0f - cos(theta)) * 0.5f);
	return h1 * (1 - f) + h2 * f;
}

// Metodo que obtiene la altura interpolada de un vertice.
float obtenerAlturaInterpolada(float x, float y) {
	int intX = (int) x; //Si x = 2.7, intX = 2
	float restoX = x - intX;
	int intY = (int) y;
	float restoY = y - intY;

	float h1 = obtenerAlturaSuavizada(intX, intY);
	float h2 = obtenerAlturaSuavizada(intX + 1, intY);
	float h3 = obtenerAlturaSuavizada(intX, intY + 1);
	float h4 = obtenerAlturaSuavizada(intX + 1, intY + 1);
	float i1 = interpolar(h1, h2, restoX);
	float i2 = interpolar(h3, h4, restoX);
	return interpolar(i1, i2, restoY);
}

// Metodo que devuelve la altura final del vertice despues de 
// sumar el calculo de varias alturas para el mismo vertice 
// con diferentes valores para la frecuencia y amplitud.
float obtenerAlturaFinal(int x, int y) {
	float total = 0;
	float d = (float) pow(2, nFunciones - 1);
	for (int i = 0; i < nFunciones; i++) {
		float freq = (float)(pow(2, i) / d);
		float amp = (float) pow(dureza, i) * amplitud;
		total += obtenerAlturaInterpolada(x * freq, y * freq) * amp;
	}

	if (total < puntoMin) { puntoMin = total; }
	if (total >= puntoMax) { puntoMax = total; }

	return total;
}

#pragma endregion

void malla() {
// Metodo para dibujar una malla de cuadrilateros.
// Cada cuadrilatero esta formado por dos triangulos.
	int aux = 0;
	bool exterior = true;
	float h1, h2, h3, h4;
	glPushMatrix();
	for (float i = 0; i < EXTENSION; i += LADO) {
		for (float j = 0; j < EXTENSION; j += LADO) {

			h1 = obtenerAlturaFinal(i, j + LADO);
			h2 = obtenerAlturaFinal(i, j);
			h3 = obtenerAlturaFinal(i + LADO, j);
			h4 = obtenerAlturaFinal(i + LADO, j + LADO);

			glPushMatrix();
			if (aux % 2 == 0) { glColor3fv(GRISCLARO); }
			else { glColor3fv(GRISOSCURO); }
			glBegin(GL_TRIANGLE_STRIP);
			glVertex3f(i, j + LADO, h1);
			glVertex3f(i, j, h2);
			glVertex3f(i + LADO, j, h3);
			glEnd();
			glPopMatrix();

			aux = 1;

			glPushMatrix();
			if (aux % 2 != 0) { glColor3fv(GRISOSCURO); }
			else { glColor3fv(GRISCLARO); }
			glBegin(GL_TRIANGLE_STRIP);
			glVertex3f(i, j + LADO, h1);
			glVertex3f(i + LADO, j + LADO, h4);
			glVertex3f(i + LADO, j, h3);
			glEnd();
			glPopMatrix();

			aux = 0;
		}
	}

	HONDO = abs(puntoMax - puntoMin) / 2;

	for (float i = 0; i < EXTENSION; i += LADO) {
		for (float j = 0; j < EXTENSION; j += LADO) {
			glPushMatrix();
			if (exterior) {
				glPushMatrix();
				GLfloat v3[3] = { j + LADO, 0, obtenerAlturaFinal(j + LADO, 0) };
				GLfloat v2[3] = { j + LADO, 0, -HONDO * 3 };
				GLfloat v1[3] = { j, 0, -HONDO * 3 };
				GLfloat v0[3] = { j, 0, obtenerAlturaFinal(j, 0) };
				quad(v0, v1, v2, v3, HONDO, HONDO);
				glPopMatrix();

				glPushMatrix();
				glRotatef(90, 0, 0, 1);
				GLfloat v03[3] = { j + LADO, 0, obtenerAlturaFinal(j + LADO, 0) };
				GLfloat v02[3] = { j + LADO, 0, -HONDO * 3 };
				GLfloat v01[3] = { j, 0, -HONDO * 3 };
				GLfloat v00[3] = { j, 0, obtenerAlturaFinal(j, 0) };
				quad(v00, v01, v02, v03, HONDO, HONDO);
				glPopMatrix();
			}
			glPopMatrix();
		}
		exterior = false;
		glPushMatrix();
		GLfloat v3[3] = { i + LADO, EXTENSION, obtenerAlturaFinal(i + LADO, EXTENSION) };
		GLfloat v2[3] = { i + LADO, EXTENSION, -HONDO * 3 };
		GLfloat v1[3] = { i, EXTENSION, -HONDO * 3 };
		GLfloat v0[3] = { i, EXTENSION, obtenerAlturaFinal(i, EXTENSION) };
		quad(v0, v1, v2, v3, HONDO, HONDO);
		glPopMatrix();

		glPushMatrix();
		GLfloat v03[3] = { EXTENSION, i + LADO, obtenerAlturaFinal(i + LADO, EXTENSION) };
		GLfloat v02[3] = { EXTENSION, i + LADO, -HONDO * 3 };
		GLfloat v01[3] = { EXTENSION, i, -HONDO * 3 };
		GLfloat v00[3] = { EXTENSION, i, obtenerAlturaFinal(i, EXTENSION) };
		quad(v00, v01, v02, v03, HONDO, HONDO);
		glPopMatrix();
	}

	glPopMatrix();
}

void display()
// Funcion de atencion al dibujo
{
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Seleccionar la MODELVIEW
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Situar y orientar la camara
	gluLookAt(70, 70, 40, 0, 0, 0, 0, 0, 1);

	glPushMatrix();
	ejes();
	glPopMatrix();

	// Crear el terreno
	glPushMatrix();
	glTranslatef(- (float) (EXTENSION) / 2, - (float) (EXTENSION) / 2, 0);
	malla();
	glPopMatrix(); 

	// Crear agua
	cout << "seed = " << seed << endl;
	cout << " " << endl;
	cout << "min = " << puntoMin << endl;
	cout << " " << endl;
	cout << "max = " << puntoMax << endl;
	glPushMatrix();
	glColor4f(37.0 / 255, 109.0 / 255, 123.0 / 255, 0.5);
	glTranslatef(0, 0, puntoMin);
	glScalef(EXTENSION - 0.1, EXTENSION - 0.1, HONDO);
	glutSolidCube(1);
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

	gluPerspective(30, relacionAspecto, 0.1, 100);
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

	// Bucle de atencion a eventos
	glutMainLoop();
}