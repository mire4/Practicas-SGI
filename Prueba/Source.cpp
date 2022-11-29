/* ---------------------------------------------------------------------------
** This application is in the public domain, furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** Interactive Ferris Wheel demo with basic opengl glu methods
**
** Author: Srivigneshwar R Prasad
** Month/Year:  Dec/2011
** -------------------------------------------------------------------------*/

#include <iostream>
#include <stdlib.h>
#include <GL/freeglut.h>
#include <sstream>
#include <string>
#include <math.h>
//#pragma comment( lib, "glaux.lib" )
using namespace std;

//Variables//
stringstream charval;
string stringval;
int MAP = 40;
GLuint texName;
float xpos = 0, ypos = 0, zpos = 0, xrot = 0, yrot = 0, angle = 0.0; //http://www.swiftless.com/tutorials/opengl/camera.html

float _angle = -70.0f;
GLUquadric* qobj;
GLuint cylinder;

//float gapBtwWheels = 6;

  ///////////////////////////////////////
 //			Displaying text			  //
///////////////////////////////////////

void drawText(const char* stringval, int length, int x, int y) {
	glMatrixMode(GL_PROJECTION);
	double* matrix = new double[16];
	glGetDoublev(GL_PROJECTION_MATRIX, matrix);
	glLoadIdentity();
	glOrtho(0, 800, 0, 600, -5, 5);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPushMatrix();
	glLoadIdentity();
	glRasterPos2i(x, y);
	for (int i = 0; i < length; i++) {
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, (int)stringval[i]);
	}
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixd(matrix);
	glMatrixMode(GL_MODELVIEW);

}


///////////////////////////////////////
//			Keyboard controls		  //
///////////////////////////////////////
float lasty = 0, lastx = 0, zoom = 0, rotx = 0, roty = 0, tx = 0, ty = 0;
void handleKeypress(unsigned char key, int x, int y) {
	if (key == 'q')
	{
		xrot += 1;
		if (xrot > 360) xrot -= 360;
	}

	if (key == 'z')
	{
		xrot -= 1;
		if (xrot < -360) xrot += 360;
	}

	if (key == 's')
	{
		float xrotrad, yrotrad;
		yrotrad = (yrot / 180 * 3.141592654f);
		xrotrad = (xrot / 180 * 3.141592654f);
		xpos += float(sin(yrotrad));
		zpos -= float(cos(yrotrad));
		ypos -= float(sin(xrotrad));
	}

	if (key == 'w')
	{
		float xrotrad, yrotrad;
		yrotrad = (yrot / 180 * 3.141592654f);
		xrotrad = (xrot / 180 * 3.141592654f);
		xpos -= float(sin(yrotrad));
		zpos += float(cos(yrotrad));
		ypos += float(sin(xrotrad));
	}

	if (key == 'd')
	{
		yrot += 1;
		if (yrot > 360) yrot -= 360;
	}

	if (key == 'a')
	{
		yrot -= 1;
		if (yrot < -360)yrot += 360;
	}
	if (key == 27)
	{
		exit(0);
	}


}


/////////////////////////////////////////
//		Initializes 3D rendering	    //
/////////////////////////////////////////

void initRendering() {
	//AUX_RGBImageRec *texture[1];
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	//glEnable(GL_LIGHT3);
	//glEnable(GL_LIGHT4);
	glEnable(GL_NORMALIZE);
	glShadeModel(GL_SMOOTH);
	qobj = gluNewQuadric();
	gluQuadricNormals(qobj, GLU_SMOOTH);
	//load texture





}


/////////////////////////////////////////
//	Called when the window is resized   //
/////////////////////////////////////////

void handleResize(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (double)w / (double)h, 1.0, 200.0);
}



///////////////////////////////////////////
//				Camera					  //
///////////////////////////////////////////
void camera(void) {
	//glPushMatrix();
	glRotatef(xrot, 1.0, 0.0, 0.0);  //rotate our camera on the x-axis (left and right)
	glRotatef(yrot, 0.0, 1.0, 0.0);  //rotate our camera on the y-axis (up and down)
	//glPopMatrix();
	glTranslated(-xpos, -ypos, -zpos); //translate the screen to the position of our camera
}



///////////////////////////////////////////
//				Draw Ground				  //
///////////////////////////////////////////

void DrawGround()
{
	glPushAttrib(GL_CURRENT_BIT);
	glEnable(GL_BLEND);
	glPushMatrix();
	glColor3f(0.5f, 0.7f, 1.0f);
	glTranslatef(0, 0.0f, 0);
	int size0 = (int)(MAP * 2);
	glBegin(GL_LINES);
	for (int x = -size0; x < size0; x += 4)
	{
		glVertex3i(x, 0, -size0); glVertex3i(x, 0, size0);
	}
	for (int z = -size0; z < size0; z += 4)
	{
		glVertex3i(-size0, 0, z); glVertex3i(size0, 0, z);
	}
	glEnd();
	glPopMatrix();
	glDisable(GL_BLEND);
	glPopAttrib();
}


void wheelSupport() {
	//wheel support
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 0.0f);
	glBegin(GL_LINES);
	glVertex2f(-7, 7);
	glVertex2f(7, -7);
	glEnd();

	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 0.f);
	glBegin(GL_LINES);
	glVertex2f(7, 7);
	glVertex2f(-7, -7);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 0.f);
	glBegin(GL_LINES);
	glVertex2f(-10, 0);
	glVertex2f(10, 0);
	glEnd();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 0.f);
	glBegin(GL_LINES);
	glVertex2f(0, 10);
	glVertex2f(0, -10);
	glEnd();
	glPopMatrix();
}

///wheel support ends //
void chair_base_model() {

	glBegin(GL_POLYGON);
	glColor3f(0.57f, 0.31f, 0.11f);
	glVertex3f(0.1, 0.8, 1.2);
	glVertex3f(-0.1, 0.8, 1.2);
	glVertex3f(-0.1, -0, 1.2);
	glVertex3f(0.1, -0, 1.2);
	glEnd();

	glBegin(GL_POLYGON);
	glColor3f(0.59f, 0.36f, 0.17f);
	glVertex3f(0.1, 0.8, 1.2);
	glVertex3f(0.1, -0, 1.2);
	glVertex3f(0.1, -0, -1.2);
	glVertex3f(0.1, 0.8, -1.2);
	glEnd();

	glBegin(GL_POLYGON);
	glColor3f(0.54f, 0.33f, 0.14f);
	glVertex3f(0.1, 0.8, -1.2);
	glVertex3f(0.1, -0, -1.2);
	glVertex3f(-0.1, -0, -1.2);
	glVertex3f(-0.1, 0.8, -1.2);
	glEnd();

	glBegin(GL_POLYGON);
	glColor3f(0.57f, 0.31f, 0.11f);
	glVertex3f(-0.1, 0.8, 1.2);
	glVertex3f(-0.1, 0.8, -1.2);
	glVertex3f(-0.1, -0, -1.2);
	glVertex3f(-0.1, -0, 1.2);
	glEnd();

	glBegin(GL_POLYGON);
	glColor3f(0.57f, 0.31f, 0.11f);
	glVertex3f(0.1, 0.8, 1.2);
	glVertex3f(0.1, 0.8, -1.2);
	glVertex3f(-0.1, 0.8, -1.2);
	glVertex3f(-0.1, 0.8, 1.2);
	glEnd();

	glBegin(GL_POLYGON);
	glColor3f(0.54f, 0.36f, 0.15f);
	glVertex3f(0.1, -0, 1.2);
	glVertex3f(-0.1, -0, 1.2);
	glVertex3f(-0.1, -0, -1.2);
	glVertex3f(0.1, -0, -1.2);
	glEnd();





}

void chairs() {

	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 0.0f);
	glRotatef(0.0, 0.00, 0.00, 0.0);
	chair_base_model();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.10f, 0.7f, 0.0f);
	glRotatef(90.0, 0.00, 0.00, 0.10);
	chair_base_model();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.8f, 0.7f, 0.0f);
	glRotatef(0.0, 0.00, 0.00, 0.10);
	chair_base_model();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.8f, 2.0f, -0.7f);
	glRotatef(90.0, 0.10, 0.00, 0.0);
	gluCylinder(qobj, .02, .02, 0.5, 15, 1);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-0.8f, 2.0f, 0.7f);
	glRotatef(90.0, 0.10, 0.00, 0.0);
	gluCylinder(qobj, .02, .02, 0.5, 15, 1);
	glPopMatrix();

}



///////////////////////////////////////////
//				Draw WHEEL				  //
///////////////////////////////////////////
void wheels() {

	glColor3f(0.85f, 0.85f, 0.85f);

	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 2.0f);
	glutSolidTorus(.12, 10, 8, 8);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 6.0f);
	glutSolidTorus(.12, 10, 8, 8);
	glPopMatrix();

	glColor3f(0.57f, 0.31f, 0.11f);
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 0.0f);
	gluCylinder(qobj, 1.0, 1.0, 8, 8, 1);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 0.f);
	gluDisk(qobj, 0, 1.0, 8, 11);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 8.f);
	gluDisk(qobj, 0, 1.0, 8, 11);
	glPopMatrix();

	glColor3f(0.85f, 0.85f, 0.85f);
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 2.f);
	wheelSupport();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 6.f);
	wheelSupport();
	glPopMatrix();

	//between 2 wheels
	glPushMatrix();
	glTranslatef(10.0f, 0.0f, 2.0f);
	gluCylinder(qobj, .07, .07, 4, 15, 1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(7.0f, 7.0f, 2.0f);
	gluCylinder(qobj, .07, .07, 4, 15, 1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0f, 10.0f, 2.0f);
	gluCylinder(qobj, .07, .07, 4, 15, 1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-7.0f, 7.0f, 2.0f);
	gluCylinder(qobj, .07, .07, 4, 15, 1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-10.0f, 0.0f, 2.0f);
	gluCylinder(qobj, .07, .07, 4, 15, 1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-7.0f, -7.0f, 2.0f);
	gluCylinder(qobj, .07, .07, 4, 15, 1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0f, -10.0f, 2.0f);
	gluCylinder(qobj, .07, .07, 4, 15, 1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(7.0f, -7.0f, 2.0f);
	gluCylinder(qobj, .07, .07, 4, 15, 1);
	glPopMatrix();
	//between 2 wheels ends here//

				//keeping the chairs //


}

void wheelsAnimation() {

	glTranslatef(0.0f, 13.0f, 0.0f);
	glRotatef(_angle, 0.0f, 0.0f, -0.1f);
	wheels();

	glPushMatrix();
	//glTranslatef(7.8f, -9.0f,0.0f);
	glTranslatef(0.0f, -10.0f, 0.0f);
	glRotatef(_angle, 0.0f, 0.0f, 0.01f);
	glTranslatef(0.8f, -2.0f, 4.0f);
	chairs();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(10.0f, 0.0f, 0.0f);
	glRotatef(_angle, 0.0f, 0.0f, 0.01f);
	glTranslatef(0.8f, -2.0f, 4.0f);
	chairs();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0f, 10.0f, 0.0f);
	glRotatef(_angle, 0.0f, 0.0f, 0.01f);
	glTranslatef(0.8f, -2.0f, 4.0f);
	chairs();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-10.0f, 0.0f, 0.0f);
	glRotatef(_angle, 0.0f, 0.0f, 0.01f);
	glTranslatef(0.8f, -2.0f, 4.0f);
	chairs();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-7.0f, 7.0f, 0.0f);
	glRotatef(_angle, 0.0f, 0.0f, 0.01f);
	glTranslatef(0.8f, -2.0f, 4.0f);
	chairs();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(7.0f, 7.0f, 0.0f);
	glRotatef(_angle, 0.0f, 0.0f, 0.01f);
	glTranslatef(0.8f, -2.0f, 4.0f);
	chairs();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(7.0f, -7.0f, 0.0f);
	glRotatef(_angle, 0.0f, 0.0f, 0.01f);
	glTranslatef(0.8f, -2.0f, 4.0f);
	chairs();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-7.0f, -7.0f, 0.0f);
	glRotatef(_angle, 0.0f, 0.0f, 0.01f);
	glTranslatef(0.8f, -2.0f, 4.0f);
	chairs();
	glPopMatrix();




}


void wheelsSupportGroundBase() {
	glBegin(GL_POLYGON);
	glColor3f(0.57f, 0.31f, 0.11f);
	glVertex3f(0.5, 15, 0.5);
	glVertex3f(-0.5, 15, 0.5);
	glVertex3f(-0.5, -0, 0.5);
	glVertex3f(0.5, -0, 0.5);
	glEnd();

	glBegin(GL_POLYGON);
	glColor3f(0.59f, 0.36f, 0.17f);
	glVertex3f(0.5, 15, 0.5);
	glVertex3f(0.5, -0, 0.5);
	glVertex3f(0.5, -0, -0.5);
	glVertex3f(0.5, 15, -0.5);
	glEnd();

	glBegin(GL_POLYGON);
	glColor3f(0.54f, 0.33f, 0.14f);
	glVertex3f(0.5, 15, -0.5);
	glVertex3f(0.5, -0, -0.5);
	glVertex3f(-0.5, -0, -0.5);
	glVertex3f(-0.5, 15, -0.5);
	glEnd();

	glBegin(GL_POLYGON);
	glColor3f(0.57f, 0.31f, 0.11f);
	glVertex3f(-0.5, 15, 0.5);
	glVertex3f(-0.5, 15, -0.5);
	glVertex3f(-0.5, -0, -0.5);
	glVertex3f(-0.5, -0, 0.5);
	glEnd();

	glBegin(GL_POLYGON);
	glColor3f(0.57f, 0.31f, 0.11f);
	glVertex3f(0.5, 15, 0.5);
	glVertex3f(0.5, 15, -0.5);
	glVertex3f(-0.5, 15, -0.5);
	glVertex3f(-0.5, 15, 0.5);
	glEnd();

	glBegin(GL_POLYGON);
	glColor3f(0.54f, 0.36f, 0.15f);
	glVertex3f(0.5, -0, 0.5);
	glVertex3f(-0.5, -0, 0.5);
	glVertex3f(-0.5, -0, -0.5);
	glVertex3f(0.5, -0, -0.5);
	glEnd();
}

void wheelsSupportGround() {
	//    glColor3f(0.85f, 0.85f, 0.85f);
	glPushMatrix();
	glTranslatef(9.0f, 0.0f, 1.0f);
	glRotatef(35.0, 0.00, 0.00, 0.10);
	wheelsSupportGroundBase();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-9.0f, 0.0f, 1.0f);
	glRotatef(-35.0, 0.00, 0.00, 0.10);
	wheelsSupportGroundBase();
	glPopMatrix();


	glPushMatrix();
	glTranslatef(-9.0f, 0.0f, 7.0f);
	glRotatef(-35.0, 0.00, 0.00, 0.10);
	wheelsSupportGroundBase();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(9.0f, 0.0f, 7.0f);
	glRotatef(35.0, 0.00, 0.00, 0.10);
	wheelsSupportGroundBase();
	glPopMatrix();
}


void display() {
	DrawGround();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//gluLookAt (-30,20,-21.0,0,0,0,0,1,0 ) ;
	gluLookAt(0, 0, -0.10, 0, 0, 0, 0, 1, 0);

	camera();
	DrawGround();
	//glCallList(cylinder);
	//cylinder_();
  //wheels();
  //chairs();
	wheelsSupportGround();
	wheelsAnimation();

	////Add ambient light

	//GLfloat ambientColor[] = {0.2f, 0.2f, 0.2f, 1.0f}; //Color (0.2, 0.2, 0.2)
	//glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);

	////Add positioned light

	GLfloat lightColor0[] = { 0.5f, 0.5f, 0.5f, 1.0f }; //Color (0.5, 0.5, 0.5)
	GLfloat lightPos0[] = { 4.0f, 0.0f, 8.0f, 1.0f }; //Positioned at (4, 0, 8)
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);

	//Add directed light
	GLfloat lightColor1[] = { 0.1f, 0.1f, 0.1f, 1.0f }; //Color (0.5, 0.2, 0.2)
	//Coming from the direction (-1, 0.5, 0.5)
	GLfloat lightPos1[] = { -1.0f, 0.5f, 0.5f, 0.0f };
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor1);
	glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);

	//char text[10];
	//itoa (fast,text,10);
	////text="sample text";
	//charval <<text;
	//charval >>stringval;
	//drawText(stringval.data(), stringval.size(), 0,0);
	glutSwapBuffers();
}


void update(int value) {
	_angle += 0.2f;
	if (_angle > 0) {
		_angle += 0;
	}

	glutPostRedisplay();
	glutTimerFunc(10, update, 0);
}

int main(int argc, char** argv) {
	//Initialize GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 600);

	//Create the window
	glutCreateWindow("Assignment - Ferris Wheel");

	initRendering();
	//glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
   //Set handler functions
   //drawString("testing");

	glutDisplayFunc(display);

	//glutDisplayFunc(drawSupport);
	glutKeyboardFunc(handleKeypress);
	glutReshapeFunc(handleResize);
	// glutIdleFunc(drawWheel);

	glutTimerFunc(25, update, 0); //Add a timer

	glutMainLoop();
	return 0;
}