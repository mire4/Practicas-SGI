/*!
	Simulador.cpp
	Simulador de vuelo
	@author		Mireia Pires State
	@date		Enero, 2022
 */

#define PROYECTO "ISGI::Simulador de vuelo"

#include <iostream>
#include <Utilidades.h>
#include <vector>
#include <iomanip>
#include <sstream>

using namespace std;

#pragma region VariablesGlobales:

// Caracteristicas del terreno.
const GLfloat EXTENSION = 400;
const GLfloat RESOLUCION_TERRENO = 200;
const GLfloat LADO = EXTENSION / RESOLUCION_TERRENO;
static GLint HONDO;
static GLfloat puntoMin = 84165454851;
static GLfloat puntoMax = -84165454851;
static GLint aumento = 1;
static GLint aux = 1;
static GLfloat profundidadAgua = 0;

// Caracteristicas el calculo de la altura.
const GLfloat dureza = 0.002f;
const GLint nFunciones = 4;
const GLfloat amplitud = 10.0f;

// Caracteristicas movimiento de la camara.
GLdouble eye[3] = { (EXTENSION / 2) * aumento, (EXTENSION / 2) * aumento, 5 };
GLdouble center[3] = { 1, 0, 0 };
GLdouble up[3] = { 0, 0, 1 };
static GLfloat xanterior, yanterior;
static GLfloat guinyada = 0.00f;
static GLfloat cabeceo = 0.00f;
bool botonIzq = false;

const GLfloat aceleracion = 0.00005f;
const GLfloat velocidadMaxima = 0.001f;
static GLfloat desplazamiento = 0.0;
static GLfloat velocidad = 0;

static GLfloat anguloGiro = 15.0f;
static GLfloat desplazamientoGiro = 0.0f;

static GLfloat incrementaAltitud = 0.5f;
static GLfloat alturaMax = 10.0f;

// Caracteristicas del piloto automatico.
static GLboolean activarPiloto = false;
static GLdouble alturaSuelo = 0.0f;
static GLdouble diferencia = 0.0f;

// Caracteristicas del tiempo.
static GLfloat antes = glutGet(GLUT_ELAPSED_TIME);
static GLfloat ahora;
static GLfloat delta;
static const GLint tasaFPS = 60;

// Caracteristicas de la niebla.
GLfloat densidad = 0.015;
GLfloat colorNieblaDia[3] = { 234.0/255.0, 198.0/255.0, 132.0/255.0 };
GLfloat colorNieblaNoche[3] = { 0.001, 0.001, 0.0015 };

// Caracteristicas de la luz ambiente.
static enum MODO { DIA, NOCHE } modo;
GLfloat posicion0[] = { 0, 0, 1, 0 };
GLfloat colorNoche[3] = { 0.2, 0.4, 0.21 };

// Caracteristicas de la luz focal movil.
static enum FOCO_ACTIVADO { SI, NO } focoMovil;
GLfloat posicion2[] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat direccion2[] = { 0.0, 0.0, -5.0 };
GLfloat posicion4[] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat direccion4[] = { 0.0, 0.0, -5.0 };

// Caracteristicas de las texturas.
GLuint cieloDia, cieloNoche, hierba1, hierba2, hierba3, nave, agua;

// Caracteristicas de la cabina.
static enum CABINA { NO_CREAR, CREAR } cabina;

// Caracteristicas para el texto
static GLfloat cap_w = 600;
static GLfloat cap_h = 600;
static GLint margen_w = 20;
static GLint margen_h = 30;
static GLint letra = 12;
static GLint interlineado = 10;
static GLfloat color[] = { NEGRO[0], NEGRO[1], NEGRO[2], NEGRO[3] };

#pragma endregion

void comentariosTerminal() {
	// Informacion general
	cout << "Iniciando " << PROYECTO << endl;
	cout << "GL version " << glGetString(GL_VERSION) << endl;

	cout << "" << endl;
	cout << "CONTEXTO" << endl;
	cout << "		Formas parte de un equipo de investigacion y te han enviado a una isla que jamas ha sido visitada." << endl;
	cout << "	Tu mision es explorar la isla con una nave super chula de ultima generacion! Con esta nave da igual que" << endl;
	cout << "	sea de dia o de noche, podras seguir volando igualmente. Ademas, si necesitas un descanso siempre puedes" << endl;
	cout << "	activar el piloto automatico sin miedo a que la nave se estrelle!" << endl;
	cout << "	El tiempo corre y una nueva aventura te esta esperando..." << endl;

	// Controles
	cout << "" << endl;
	cout << "CONTROLES GENERALES" << endl;
	cout << "	  HORA DEL DIA: Alterne con \"L/l\" la hora del dia entre por la manyana o noche." << endl;
	cout << "	  MOSTRAR NAVE: Alterne con \"C/c\" para mostrar u ocultar la nave." << endl;
	cout << "	LINTERNA MOVIL: Alterne con \"F/f\" para utilizar la linterna o apagarla." << endl;

	cout << "" << endl;
	cout << "CONTROLES MOVIMIENTO" << endl;
	cout << "	CONTROL DE VELOCIDAD: Para aumentar la velocidad pulse \"A/a\", para disminuirla pulse \"Z/z\"." << endl;
	cout << "	     GIRO DE LA NAVE: Para girar pulse \"flecha derecha\" o pulse \"flecha izquierda\"." << endl;
	cout << "	   ALTURA DE LA NAVE: Para aumentar la altura pulse \"flecha arriba\", para disminuirla pulse \"flecha abajo\"." << endl;
	cout << "	   PILOTO AUTOMATICO: Alterne con \"Q/q\" para utilizar el piloto automatico o quitarlo." << endl;
}

void texturas() {
	// Fondos
	glGenTextures(1, &cieloDia);
	glBindTexture(GL_TEXTURE_2D, cieloDia);
	loadImageFile((char*)"cieloDia.jpg");

	glGenTextures(1, &cieloNoche);
	glBindTexture(GL_TEXTURE_2D, cieloNoche);
	loadImageFile((char*)"cieloNoche.jpg");

	// Montanya
	glGenTextures(1, &hierba1);
	glBindTexture(GL_TEXTURE_2D, hierba1);
	loadImageFile((char*)"hierba1.png");

	glGenTextures(1, &hierba2);
	glBindTexture(GL_TEXTURE_2D, hierba2);
	loadImageFile((char*)"hierba2.png");

	glGenTextures(1, &hierba3);
	glBindTexture(GL_TEXTURE_2D, hierba3);
	loadImageFile((char*)"hierba3.png");

	// Nave
	glGenTextures(1, &nave);
	glBindTexture(GL_TEXTURE_2D, nave);
	loadImageFile((char*)"nave.png");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// Agua
	glGenTextures(1, &agua);
	glBindTexture(GL_TEXTURE_2D, agua);
	loadImageFile((char*)"agua.png");
}

void texturarCielo() {
	glDisable(GL_TEXTURE_2D);
	if (modo == DIA) {
		glBindTexture(GL_TEXTURE_2D, cieloDia);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		texturarFondo();
	}
	else {
		glBindTexture(GL_TEXTURE_2D, cieloNoche);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		texturarFondo();
	}
}

void init()
{
	comentariosTerminal();

	// Texturas
	texturas();

	// Configurar motor de render 
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_FOG);
	//glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
	//glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);
	glEnable(GL_NORMALIZE);
	glEnable(GL_TEXTURE_2D);
}

#pragma region CalculoDeLaAltura 
/* Para el calculo de la altura se ha tomado como punto
 de partida el ruido en escala de grises, pero generandolo
 mediante numeros aleatorios.

 Finalmente, se procesa la altura generada mediante una funcion
 de interpolacion para crear el Perlin Noise.*/

 // Metodo que devuelve una altura con valor aleatorio.
float obtenerAltura(float x, float y) {
	int n = x + y * 57;
	n = (n << 13) ^ n;
	return (1.0 - ((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0);
}

// Metodo que devuelve la suma de la altura del vertice 
// que se mira mas de los que estan a su alrededor.
float obtenerAlturaSuavizada(float x, float y) {
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
	int intX = (int)x; //Si x = 2.7, intX = 2
	float restoX = x - intX;
	int intY = (int)y;
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
float obtenerAlturaFinal(float x, float y) {
	float total = 0;
	float d = (float)pow(2, nFunciones - 1);
	for (int i = 0; i < nFunciones; i++) {
		float freq = (float)(pow(2, i) / d);
		float amp = (float)pow(dureza, i) * amplitud;
		total += obtenerAlturaInterpolada(x * freq, y * freq) * amp;
	}

	if (total < puntoMin) { puntoMin = total; }
	if (total >= puntoMax) { puntoMax = total; }

	return total;
}

#pragma endregion 

void crearMalla() {
	// Metodo para dibujar una malla de cuadrilateros.
	// Cada cuadrilatero esta formado por dos triangulos.
	int num = 0;
	int aux = 0;
	bool exterior = true;
	GLfloat h1, h2, h3, h4;
	GLfloat verdeO[4] = { 0.18, 0.34, 0.17, 1.0 };

	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);

	glPushMatrix();
	glScalef(aumento, aumento, 1);
	for (GLfloat i = 0; i < EXTENSION; i += LADO) {
		for (GLfloat j = 0; j < EXTENSION; j += LADO) {
			h1 = obtenerAlturaFinal(i, j + LADO);
			h2 = obtenerAlturaFinal(i, j);
			h3 = obtenerAlturaFinal(i + LADO, j);
			h4 = obtenerAlturaFinal(i + LADO, j + LADO);
			num = obtenerAlturaFinal(i, j);
			
			glEnable(GL_COLOR_MATERIAL);
			if (num % 5 == 0 && !(h1 >= puntoMax - 2 || h2 >= puntoMax - 2 || h3 >= puntoMax - 2 || h4 >= puntoMax - 2)) {
				glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, verdeO);
				glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, GRISOSCURO);
				glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 10);
				glBindTexture(GL_TEXTURE_2D, hierba2);
			}
			else if (num % 5 != 0 && !(h1 >= puntoMax - 2 || h2 >= puntoMax - 2 || h3 >= puntoMax - 2 || h4 >= puntoMax - 2)) {
				glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, verdeO);
				glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, VERDE);
				glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, GRISOSCURO);
				glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 10);
				glBindTexture(GL_TEXTURE_2D, hierba1); 
			}
			else if (h1 >= puntoMax - 2 || h2 >= puntoMax - 2 || h3 >= puntoMax - 2 || h4 >= puntoMax - 2) {
				glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, BLANCO);
				glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, AZUL);
				glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, BLANCO);
				glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 5);
				glBindTexture(GL_TEXTURE_2D, hierba3);
			}
			glDisable(GL_COLOR_MATERIAL);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);

			/*if (aux % 2 == 0) { glColor3fv(GRISCLARO); }
			else { glColor3fv(GRISOSCURO); }*/
			glBegin(GL_TRIANGLE_STRIP);
			glTexCoord2f(i, j + LADO);
			glVertex3f(i, j + LADO, h1);
			glTexCoord2f(i, j);
			glVertex3f(i, j, h2);
			glTexCoord2f(i + LADO, j);
			glVertex3f(i + LADO, j, h3);
			glEnd();

			//aux = 1;

			/*if (aux % 2 != 0) { glColor3fv(GRISOSCURO); }
			else { glColor3fv(GRISCLARO); }*/
			glBegin(GL_TRIANGLE_STRIP);
			glTexCoord2f(i, j + LADO);
			glVertex3f(i, j + LADO, h1);
			glTexCoord2f(i + LADO, j + LADO);
			glVertex3f(i + LADO, j + LADO, h4);
			glTexCoord2f(i + LADO, j);
			glVertex3f(i + LADO, j, h3);
			glEnd();
			//aux = 0;
		}
	}

	HONDO = abs(puntoMax - puntoMin) / 3 + 0.5;
	glPopMatrix();

	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	
	glPopAttrib();
}

void crearAgua() {
	profundidadAgua = puntoMin + 4.5;
	glPushAttrib(GL_ALL_ATTRIB_BITS);
		glEnable(GL_LIGHTING);
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_COLOR_MATERIAL);

		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MARINO);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, AZUL);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, BLANCO);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 6);
		glBindTexture(GL_TEXTURE_2D, agua);
		glDisable(GL_COLOR_MATERIAL);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);

		glPushMatrix();
			GLfloat v0[3] = { 0, 0, profundidadAgua };
			GLfloat v1[3] = { EXTENSION, 0, profundidadAgua };
			GLfloat v2[3] = { EXTENSION, EXTENSION, profundidadAgua };
			GLfloat v3[3] = { 0, EXTENSION, profundidadAgua };
			glScalef(aumento, aumento, 1);
			quad(v0, v1, v2, v3);
		glPopMatrix();

		glDisable(GL_LIGHTING);
		glDisable(GL_TEXTURE_2D);
	glPopAttrib();
}

void crearCabina() {
	GLfloat ambiente[] = { 0.3, 0.3, 0.3, 1.0 };
	GLfloat difuso[] = { 0.8, 0.8, 0.8, 1.0 };
	GLfloat especular[] = { 0.9, 0.9, 0.9, 1.0 };
	glPushAttrib(GL_ALL_ATTRIB_BITS);
		glEnable(GL_TEXTURE_2D);
		glDisable(GL_LIGHTING);
		glDepthMask(GL_FALSE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glEnable(GL_TEXTURE_GEN_S);
		glEnable(GL_TEXTURE_GEN_T);

		glMaterialfv(GL_FRONT, GL_AMBIENT, ambiente);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, difuso);
		glMaterialfv(GL_FRONT, GL_SPECULAR, especular);
		glMaterialf(GL_FRONT, GL_SHININESS, 100);
		glBindTexture(GL_TEXTURE_2D, nave);
		glDisable(GL_COLOR_MATERIAL);
		GLfloat planoS[] = { 0, 0.45, 0, 0.5 };
		GLfloat planoT[] = { 0, 0, 0.5, 0.5 };
		glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
		glTexGenfv(GL_S, GL_OBJECT_PLANE, planoS);
		glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
		glTexGenfv(GL_T, GL_OBJECT_PLANE, planoT);

		glPushMatrix();
			glTranslatef(eye[0], eye[1], eye[2]);
			glRotatef(desplazamientoGiro, 0, 0, 1);
			glutSolidSphere(1.3, 20, 20);
		glPopMatrix();
		glDepthMask(GL_TRUE);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_TEXTURE_GEN_S);
		glDisable(GL_TEXTURE_GEN_T);
		glEnable(GL_LIGHTING);
	glPopAttrib();
}

void pilotoAutonomo() {
	alturaSuelo = obtenerAlturaFinal(eye[0], eye[1]);
	eye[2] = diferencia + alturaSuelo;
}

void habilitarLuces() {
	// Niebla
	glFogi(GL_FOG_MODE, GL_EXP2);
	glFogf(GL_FOG_DENSITY, densidad);
	glHint(GL_FOG_HINT, GL_NICEST);

	glEnable(GL_LIGHT0);

	if (modo == DIA) {
		glDisable(GL_LIGHT1);
		glDisable(GL_LIGHT2);

		// Luz sol
		glLightfv(GL_LIGHT0, GL_DIFFUSE, colorNieblaDia);

		// Color niebla
		glFogfv(GL_FOG_COLOR, colorNieblaDia);
	}
	else {
		glEnable(GL_LIGHT1);
		glEnable(GL_LIGHT2);

		// Luz luna
		glLightfv(GL_LIGHT0, GL_DIFFUSE, colorNieblaNoche);

		// Luz foco fijo
		glLightfv(GL_LIGHT1, GL_AMBIENT, BLANCO);
		glLightfv(GL_LIGHT1, GL_DIFFUSE, AMARILLO);
		glLightfv(GL_LIGHT1, GL_SPECULAR, AMARILLO);
		glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 8.0);
		glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 2.0);

		if (focoMovil == SI) {
			// Luz foco movil
			glLightfv(GL_LIGHT2, GL_AMBIENT, BLANCO);
			glLightfv(GL_LIGHT2, GL_DIFFUSE, BLANCO);
			glLightfv(GL_LIGHT2, GL_SPECULAR, BLANCO);
			glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 2.0);
			glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 1);
		}
		else { 
			glDisable(GL_LIGHT2);
			glDisable(GL_LIGHT4);
		}

		// Color niebla
		glFogfv(GL_FOG_COLOR, colorNieblaNoche);
	}
}

void posicionarLuces(bool focoMovil) {
	if (focoMovil) {
		glLightfv(GL_LIGHT2, GL_POSITION, posicion2);
		glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, direccion2);

		glLightfv(GL_LIGHT4, GL_POSITION, posicion4);
		glLightfv(GL_LIGHT4, GL_SPOT_DIRECTION, direccion4);
	}
	else {
		// Sol/Luna
		glLightfv(GL_LIGHT0, GL_POSITION, posicion0);

		// Foco fijo
		GLfloat posicion1[] = { eye[0], eye[1], eye[2], 1 };
		GLfloat direccion1[] = { cos(rad(desplazamientoGiro)), sin(rad(desplazamientoGiro)), sin(rad(-10)) };
		glLightfv(GL_LIGHT1, GL_POSITION, posicion1);
		glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, direccion1);

		GLfloat posicion3[] = { eye[0], eye[1], eye[2], 1 };
		GLfloat direccion3[] = { cos(rad(desplazamientoGiro)), sin(rad(desplazamientoGiro)), sin(rad(-10)) };
		glLightfv(GL_LIGHT3, GL_POSITION, posicion3);
		glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, direccion3);
	}
}

void posicionarTexto() {
	glPushAttrib(GL_ALL_ATTRIB_BITS);
		glDisable(GL_LIGHTING);
		glPushMatrix();
			string cascoInfo = "HUD::Nave de exploracion";
			stringstream ss;

			// Titulo
			texto(margen_w, cap_h - margen_h, (char*)cascoInfo.c_str(), BLANCO, GLUT_BITMAP_HELVETICA_18, false);

			if (modo == NOCHE) {
				color[0] = BLANCO[0];
				color[1] = BLANCO[1];
				color[2] = BLANCO[2];
				color[3] = BLANCO[3];
			}
			else {
				color[0] = NEGRO[0];
				color[1] = NEGRO[1];
				color[2] = NEGRO[2];
				color[3] = NEGRO[3];
			}

			// Velocidad
			ss << setw(16) << setfill(' ') << velocidad * 10000 * 12;
			cascoInfo = "Velocidad: " + ss.str() + "km/h";
			texto(margen_w, cap_h - margen_h - interlineado - letra, (char*)cascoInfo.c_str(), color, GLUT_BITMAP_HELVETICA_12, false);

			// Altura nave
			ss.clear();
			ss.str("");
			ss << setw(18) << setfill(' ') << eye[2] * 7;
			cascoInfo = "Altura nave: " + ss.str() + "m";
			texto(margen_w, cap_h - margen_h - interlineado - letra * 2, (char*)cascoInfo.c_str(), color, GLUT_BITMAP_HELVETICA_12, false);

			// Altura suelo
			ss.clear();
			ss.str("");
			ss << setw(17) << setfill(' ') << round(abs(obtenerAlturaFinal(eye[0], eye[1]) * 100)) / 100 * 7;
			cascoInfo = "Altura suelo: " + ss.str() + "m";
			texto(margen_w, cap_h - margen_h - interlineado - letra * 3, (char*)cascoInfo.c_str(), color, GLUT_BITMAP_HELVETICA_12, false);

			// Localizacion
			ss.clear();
			ss.str("");
			ss << setw(11) << setfill(' ') << eye[0];
			ss << ", ";
			ss << eye[1];
			cascoInfo = "Localizacion: " + ss.str();
			texto(margen_w, cap_h - margen_h - interlineado - letra * 4, (char*)cascoInfo.c_str(), color, GLUT_BITMAP_HELVETICA_12, false);

			// Brujula
			ss.clear();
			ss.str("");
			ss << setw(20) << setfill(' ') << round(guinyada);
			cascoInfo = "Brujula: " + ss.str() + "degN";
			texto(margen_w, cap_h - margen_h - interlineado - letra * 5, (char*)cascoInfo.c_str(), color, GLUT_BITMAP_HELVETICA_12, false);

			// Mira 1
			ss.clear();
			ss.str("");
			ss << setw(12) << setfill(' ') << round(guinyada * 10) / 10;
			cascoInfo = "Mira guinyada: " + ss.str() + "deg";
			texto(margen_w, cap_h - margen_h - interlineado - letra * 6, (char*)cascoInfo.c_str(), color, GLUT_BITMAP_HELVETICA_12, false);

			// Mira 2
			ss.clear();
			ss.str("");
			ss << setw(12) << setfill(' ') << round(cabeceo * 10) / 10;
			cascoInfo = "Mira cabeceo: " + ss.str() + "deg";
			texto(margen_w, cap_h - margen_h - interlineado - letra * 7, (char*)cascoInfo.c_str(), color, GLUT_BITMAP_HELVETICA_12, false);
		glPopMatrix();
	glPopAttrib();
}

void display()
// Funcion de atencion al dibujo
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Texturar fondo
	texturarCielo();

	// Seleccionar la MODELVIEW
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	// Luces
	habilitarLuces();

	// Posicionamos el foco movil antes de la camara para que dependa de ella.
	posicionarLuces(true);

	// Piloto automatico
	if (activarPiloto) { pilotoAutonomo(); }

	// Situar y orientar la camaras
	gluLookAt(eye[0], eye[1], eye[2], center[0], center[1], center[2], up[0], up[1], up[2]);

	// Posicionamos la luna/sol y el foco fijo despues de la camara para que no dependan de ella.
	posicionarLuces(false);

	// Crear el terreno
	crearMalla();

	// Crear agua
	crearAgua();

	// Poner cabina
	if (cabina == CREAR) {
		crearCabina();
	}

	// Posicionar texto
	posicionarTexto();

	glutSwapBuffers();
}

void reshape(GLint w, GLint h)
// Funcion de atencion al redimensionamiento
{
	cap_w = w;
	cap_h = h;
	float relacionAspecto = float(w) / h;
	glViewport(0, 0, w, h);

	// Definir la camara
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(30, relacionAspecto, 0.1, 100);
}

void onClick(int boton, int estado, int x, int y) {
	if (boton == GLUT_LEFT_BUTTON && estado == GLUT_DOWN) {
		xanterior = x;
		yanterior = y;
		botonIzq = true;
	}
}

void onDrag(int x, int y)
// Funcion de atencion al raton con el boton pulsado
{
	if (botonIzq) {
		float sensibilidad = 0.05;

		float xoffset = (x - xanterior) * sensibilidad;
		float yoffset = (y - yanterior) * sensibilidad;

		if (yoffset >= 45 * sensibilidad) {
			yoffset = 45 * sensibilidad;
		}
		if (yoffset <= -89 * sensibilidad) {
			yoffset = -89 * sensibilidad;
		}
		if (xoffset >= 90 * sensibilidad) {
			xoffset = 90 * sensibilidad;
		}
		if (xoffset <= -90 * sensibilidad) {
			xoffset = -90 * sensibilidad;
		}

		guinyada += xoffset;
		cabeceo += yoffset;

		if (cabeceo >= 360) {
			cabeceo = cabeceo - 360;
		}
		if (cabeceo <= -360) {
			cabeceo = cabeceo + 360;
		}
		if (guinyada >= 360) {
			guinyada = guinyada - 360;
		}
		if (guinyada <= -360) {
			guinyada = guinyada + 360;
		}

		//cout << "yaw = " << guinyada << endl;
		//cout << "pitch = " << cabeceo << endl;

		center[0] = cos(rad(guinyada)) * cos(rad(cabeceo)) + eye[0];
		center[1] = sin(rad(guinyada)) * cos(rad(cabeceo)) + eye[1];
		center[2] = sin(rad(cabeceo)) + eye[2];

		xanterior = x;
		yanterior = y;

		glutPostRedisplay();
	}
}

void onIdle() {
	ahora = glutGet(GLUT_ELAPSED_TIME);
	delta = ahora - antes;
	antes = ahora;

	// Movimiento de la camara con velocidad
	desplazamiento = delta * velocidad;
	//cout << "velocidad = " << velocidad << endl;
	//cout << "giro = " << desplazamientoGiro << endl;
	
	eye[0] += desplazamiento * cos(rad(desplazamientoGiro));
	eye[1] += desplazamiento * sin(rad(desplazamientoGiro));

	center[0] = cos(rad(guinyada)) * cos(rad(cabeceo)) + eye[0];
	center[1] = sin(rad(guinyada)) * cos(rad(cabeceo)) + eye[1];
	center[2] = sin(rad(cabeceo)) + eye[2];

	glutPostRedisplay();
}

void onTimer(int tiempo) {
	//Callback de atencion a la cuenta atras
	glutTimerFunc(tiempo, onTimer, tiempo);

	onIdle();
}

void onKey(unsigned char tecla, int x, int y)
{
	// Callback de atencion a los eventos de teclas alfanumericas
	switch (tecla) {

	// Aumenta la velocidad
	case 'a':
	case 'A':
		if (velocidad  + aceleracion < velocidadMaxima) {
			velocidad += aceleracion;
		}
		break;

	// Disminuye la velocidad
	case 'z':
	case 'Z':
		if (velocidad > 0.0001) {
			velocidad -= aceleracion;
		}
		else {
			velocidad = 0;
		}
		break;

	// Activa o desactiva el piloto automatico
	case 'q':
	case 'Q':
		if (activarPiloto) {
			activarPiloto = false;
		}
		else {
			alturaSuelo = obtenerAlturaFinal(eye[0], eye[1]);
			diferencia = eye[2] - max(alturaSuelo, 0);
			activarPiloto = true;
		}
		break;

	// Alterna entre modo diurno y nocturno
	case 'l':
	case 'L':
		if (modo == DIA) {
			modo = NOCHE;
		}
		else {
			modo = DIA;
		}
		break;

	// Enciende o apaga el foco movil
	case 'f':
	case 'F':
		if (focoMovil == SI && modo == NOCHE) {
			focoMovil = NO;
		}
		else if (focoMovil == NO && modo == NOCHE) {
			focoMovil = SI;
		}
		break;

	// Control de la cabina
	case 'c':
	case 'C':
		if (cabina == CREAR) {
			cabina = NO_CREAR;
		}
		else {
			cabina = CREAR;
		}
		break;
	case 27:
		exit(0);
	}

	glutPostRedisplay();
}

void onSpecialKey(int tecla, int x, int y) 
{
	switch (tecla) {
	case GLUT_KEY_UP:
		if (eye[2]  + incrementaAltitud < alturaMax) {
			eye[2] += incrementaAltitud;
		}
		break;
	case GLUT_KEY_DOWN:
		if (eye[2] - incrementaAltitud > obtenerAlturaFinal(eye[0], eye[1])) {
			eye[2] -= incrementaAltitud;
		}
		break;
	case GLUT_KEY_LEFT:
		desplazamientoGiro += anguloGiro;
		if (desplazamientoGiro > 360.0) {
			desplazamientoGiro = desplazamientoGiro - 360.0;
		}
		break;
	case GLUT_KEY_RIGHT:
		desplazamientoGiro -= anguloGiro;
		if (desplazamientoGiro < - 360.0) {
			desplazamientoGiro = desplazamientoGiro + 360.0;
		}
		break;
	}

	glutPostRedisplay();
}

int main(int argc, char** argv)
// Programa principal
{
	// Inicializaciones
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(cap_w, cap_h);
	glutCreateWindow(PROYECTO);
	init();

	// Registro de callbacks	
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(onKey);
	glutSpecialFunc(onSpecialKey);
	glutMouseFunc(onClick);
	glutMotionFunc(onDrag);
	glutTimerFunc(1000 / tasaFPS, onTimer, 1000 / tasaFPS);

	// Bucle de atencion a eventos
	glutMainLoop();
}