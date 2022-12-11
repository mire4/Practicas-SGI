/*
 * ferriswheel.cpp
 *
 * Implementation of a ferris wheel that rotates about its pivot.
 * Includes lighting.
 *
 * Author: Luke Lovett
 * Began: Tue Oct 30 17:09:04 EDT 2012
 *
 * */

#include <GL/glut.h>
#include <GL/glui.h>
#include <math.h>
#include "ferriswheel.h"

 /* constants */
#define PI 3.1415926535
#define WHEEL_SIZE 10.0f
#define WHEEL_INNER_SIZE WHEEL_SIZE/1.2
#define WHEEL_DEPTH 2.0f
#define WHEEL_POINTS 8
#define BENCH_WIDTH 5.0f
#define BENCH_HEIGHT 4.0f
#define BENCH_DEPTH 2.0f
#define BASE_HEIGHT 10.0f
#define BASE_WIDTH 2.0f
#define BASE_DEPTH 4.0f
#define RECURSIVE_SHRINK 3.0f

/* global variables */
GLfloat Theta = 0.5f;
GLfloat Speed = 0.1f;
int RecursiveDepth = 0;
GLfloat EyeX, EyeY, EyeZ;
GLfloat LookAtX, LookAtY, LookAtZ;

GLfloat TORSO_RADIUS = 2.0;
GLfloat TORSO_HEIGHT = 6.0;
GLfloat HEADX = 0.0, HEADY = 7.5;
GLfloat HEAD_RADIUS = 2.0;
GLfloat ARM_RADIUS = 0.4;
GLfloat UPPER_ARM_LENGTH = 3.0, LOWER_ARM_LENGTH = 3.0;
GLfloat LUAX = -2.0, LUAY = 5.0, LLAY = -3.0;
GLfloat RUAX = 2.0, RUAY = 5.0, RLAY = -3.0;
GLfloat LEG_RADIUS = 0.4;
GLfloat UPPER_LEG_LENGTH = 4.0, LOWER_LEG_LENGTH = 4.0;
GLfloat LULX = -2.0, LULY = 0.0, LLLY = -4.0;
GLfloat RULX = 2.0, RULY = 0.0, RLLY = -4.0;

/* various materials */
mProps redPlasticMaterials = {
    {0.3, 0.0, 0.0, 1.0},
    {0.9, 0.0, 0.0, 1.0},
    {0.8, 0.6, 0.6, 1.0},
    32.0
};

mProps bluePlasticMaterials = {
    { 0.0, 0.0, 0.3, 1.0 },
    { 0.0, 0.0, 0.9, 1.0 },
    { 0.7, 0.7, 0.9, 1.0 },
    40.0
};

mProps spindleMats = {
    { 0.0, 0.3, 0.0, 1.0 },
    { 0.0, 0.9, 0.0, 1.0 },
    { 0.0, 0.7, 0.0, 1.0 },
    40.0
};

mProps benchMats = {
    { 0.4, 0.4, 0.0, 1.0 },
    { 0.4, 0.4, 0.0, 1.0 },
    { 0.8, 0.8, 0.0, 1.0 },
    40.0
};

/* various lighting */
lProps whiteLighting = {
    {0.0, 0.0, 0.0, 1.0},
    {1.0, 1.0, 1.0, 1.0},
    {1.0, 1.0, 1.0, 1.0}
};

lProps coloredLighting = {
    {0.2, 0.0, 0.0, 1.0},
    {0.0, 1.0, 0.0, 1.0},
    {0.0, 0.0, 1.0, 1.0}
};

GLfloat light0_pos[4] = { 4.0, 10.0, 20.0, 0.0 };

mProps* currentMaterials;
lProps* currentLighting;

GLfloat theta[] = { 65.0, 0.0, 0.0, -42.0, -12.0, -33.0, -48.0,18.0, 74.0, -14.0, 11.0 };

GLUquadricObj* p, * q;
int main_window;

void init() {
    /* setup some globals */
    EyeX = 50.0f;
    EyeY = 10.0f;
    EyeZ = 0.0f;

    glClearColor(1.0, 1.0, 1.0, 1.0);
    glColor3f(0.0, 0.0, 0.0);
    look(0);
}

void look(int id) {
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(EyeX, EyeY, EyeZ,
        LookAtX, LookAtY, LookAtZ,
        0.0, 1.0, 0.0);
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);

    float aspect = w * 1.0 / h;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, aspect, 0.5, 200.0);
    glMatrixMode(GL_MODELVIEW);
    glutSetWindow(main_window);
    glutPostWindowRedisplay(main_window);
}

void box(GLfloat width, GLfloat height, GLfloat depth) {
    glBegin(GL_QUADS);

    glNormal3f(0, -1, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(width, 0, 0);
    glVertex3f(width, 0, depth);
    glVertex3f(0, 0, depth);

    glNormal3f(0, 0, -1);
    glVertex3f(0, 0, 0);
    glVertex3f(0, height, 0);
    glVertex3f(width, height, 0);
    glVertex3f(width, 0, 0);

    glNormal3f(-1, 0, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, depth);
    glVertex3f(0, height, depth);
    glVertex3f(0, height, 0);

    glNormal3f(1, 0, 0);
    glVertex3f(width, 0, 0);
    glVertex3f(width, 0, depth);
    glVertex3f(width, height, depth);
    glVertex3f(width, height, 0);

    glNormal3f(0, 1, 0);
    glVertex3f(0, height, 0);
    glVertex3f(width, height, 0);
    glVertex3f(width, height, depth);
    glVertex3f(0, height, depth);

    glNormal3f(0, 0, 1);
    glVertex3f(0, 0, depth);
    glVertex3f(width, 0, depth);
    glVertex3f(width, height, depth);
    glVertex3f(0, height, depth);

    glEnd();
}

void bench(float shrink) {
    setMaterial(&benchMats);

    glPushMatrix();
    glTranslatef(0, 0, -shrink * BENCH_DEPTH);
    box(shrink * BENCH_WIDTH, shrink * BENCH_HEIGHT, shrink * BENCH_DEPTH);
    glPopMatrix();

    box(shrink * BENCH_WIDTH, shrink * BENCH_HEIGHT / 2, shrink * BENCH_DEPTH);
}

void wheelBase() {
    setMaterial(&redPlasticMaterials);

    glPushMatrix();

    glBegin(GL_QUADS);

    glNormal3f(0, -1, 0);
    glVertex3f(0, 0, -BASE_DEPTH / 2);
    glVertex3f(BASE_WIDTH, 0, -BASE_DEPTH / 2);
    glVertex3f(BASE_WIDTH, 0, BASE_DEPTH / 2);
    glVertex3f(0, 0, BASE_DEPTH / 2);

    glNormal3f(0, -BASE_DEPTH / 2, -BASE_HEIGHT);
    glVertex3f(0, 0, -BASE_DEPTH / 2);
    glVertex3f(BASE_WIDTH, 0, -BASE_DEPTH / 2);
    glVertex3f(BASE_WIDTH, BASE_HEIGHT, 0);
    glVertex3f(0, BASE_HEIGHT, 0);

    glNormal3f(0, BASE_DEPTH / 2, BASE_HEIGHT);
    glVertex3f(0, 0, BASE_DEPTH / 2);
    glVertex3f(BASE_WIDTH, 0, BASE_DEPTH / 2);
    glVertex3f(BASE_WIDTH, BASE_HEIGHT, 0);
    glVertex3f(0, BASE_HEIGHT, 0);

    glEnd();

    glBegin(GL_TRIANGLES);

    glNormal3f(-1, 0, 0);
    glVertex3f(0, 0, -BASE_DEPTH / 2);
    glVertex3f(0, 0, BASE_DEPTH / 2);
    glVertex3f(0, BASE_HEIGHT, 0);

    glNormal3f(1, 0, 0);
    glVertex3f(BASE_WIDTH, 0, -BASE_DEPTH / 2);
    glVertex3f(BASE_WIDTH, 0, BASE_DEPTH / 2);
    glVertex3f(BASE_WIDTH, BASE_HEIGHT, 0);

    glEnd();

    glPopMatrix();
}

void recWheelSide(float shrink) {
    setMaterial(&bluePlasticMaterials);

    glPushMatrix();

    gluCylinder(p, shrink * WHEEL_SIZE, shrink * WHEEL_SIZE, shrink * WHEEL_DEPTH, WHEEL_POINTS, 8);
    gluCylinder(p, shrink * WHEEL_INNER_SIZE, shrink * WHEEL_INNER_SIZE, shrink * WHEEL_DEPTH, WHEEL_POINTS, 8);

    glPushMatrix();
    glTranslatef(0.0, 0.0, shrink * WHEEL_DEPTH);
    gluDisk(p, shrink * WHEEL_INNER_SIZE, shrink * WHEEL_SIZE, WHEEL_POINTS, 8);
    glPopMatrix();

    glPushMatrix();
    glRotatef(180.0f, 0.0, 1.0, 0.0);
    gluDisk(p, shrink * WHEEL_INNER_SIZE, shrink * WHEEL_SIZE, WHEEL_POINTS, 8);
    glPopMatrix();

    /* begin spindles */
    setMaterial(&spindleMats);
    GLfloat spindleLength = WHEEL_SIZE / 1.1;
    glPushMatrix();
    glRotatef(90.0, 0, 1, 0);
    glTranslatef(-shrink * WHEEL_DEPTH / 2, 0, 0);
    for (int i = 0; i < WHEEL_POINTS; i++) {
        glPushMatrix();
        glRotatef(i * 360.0 / WHEEL_POINTS, 1, 0, 0);
        gluCylinder(p, shrink * WHEEL_DEPTH / 4, shrink * WHEEL_DEPTH / 4, shrink * spindleLength, WHEEL_POINTS, 8);
        glPopMatrix();
    }
    glPopMatrix();
    /* end spindles */

    glPopMatrix();
}

void recWheel(int depth, float shrink, float netRotation) {
    setMaterial(&bluePlasticMaterials);

    /* sides of the wheel */
    glPushMatrix();
    glRotatef(90.0, 0, 1, 0);
    recWheelSide(shrink);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(shrink * (WHEEL_DEPTH + BENCH_WIDTH), 0, 0);
    glRotatef(90.0, 0, 1, 0);
    recWheelSide(shrink);
    glPopMatrix();
    /* end wheel sides */

    /* axle */
    glPushMatrix();
    glRotatef(90.0, 0, 1, 0);
    gluCylinder(p, shrink * 0.75, shrink * 0.75, shrink * (2 * WHEEL_DEPTH + BENCH_WIDTH), 8, 8);
    glPopMatrix();
    /* end axle */

    if (depth == 0) {
        /* benches */
        for (int i = 0; i < WHEEL_POINTS; i++) {
            GLfloat phi = i * 360.0 / WHEEL_POINTS;

            glPushMatrix();
            glRotatef(phi, 1, 0, 0);
            glTranslatef(shrink * WHEEL_DEPTH, 0, shrink * WHEEL_INNER_SIZE);

            glPushMatrix();
            glRotatef(-Theta - phi - netRotation, 1, 0, 0);
            bench(shrink);
            glPopMatrix();

            glPopMatrix();
        }
    }
    else {
        /* recursive call to make more wheels */
        for (int i = 0; i < WHEEL_POINTS; i++) {
            GLfloat phi = i * 360.0 / WHEEL_POINTS;

            glPushMatrix();
            glRotatef(phi, 1, 0, 0);
            GLfloat xOffset = shrink * (WHEEL_DEPTH + BENCH_WIDTH / 2);
            xOffset -= (shrink / RECURSIVE_SHRINK) * (WHEEL_DEPTH + BENCH_WIDTH / 2);
            glTranslatef(xOffset, 0, shrink * WHEEL_INNER_SIZE);

            /* axle to hold smaller wheel */
            setMaterial(&spindleMats);
            glPushMatrix();
            glTranslatef(-xOffset, 0, 0);
            glRotatef(90.0, 0, 1, 0);
            gluCylinder(p, (shrink / RECURSIVE_SHRINK) * 0.75, (shrink / RECURSIVE_SHRINK) * 0.75, shrink * (2 * WHEEL_DEPTH + BENCH_WIDTH), 8, 8);
            glPopMatrix();
            /* end smaller-axle */

            glPushMatrix();
            glRotatef((depth % 2 == 0 ? 2 : -2) * Theta, 1, 0, 0);
            recWheel(depth - 1, shrink / RECURSIVE_SHRINK,
                netRotation + (depth % 2 == 0 ? 2 : -2) * Theta + phi);
            glPopMatrix();

            glPopMatrix();
        }
    }

    /* end benches */
}

void display() {
    glutSetWindow(main_window);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);

    /* ferris wheel base */
    wheelBase();
    glPushMatrix();
    glTranslatef(2 * WHEEL_DEPTH + BASE_WIDTH + BENCH_WIDTH, 0, 0);
    wheelBase();
    glPopMatrix();
    /* end ferris wheel base */

    /* wheel */
    glPushMatrix();
    glTranslatef(BASE_WIDTH, BASE_HEIGHT, 0);
    glRotatef(Theta, 1, 0, 0);
    // wheel();
    recWheel(RecursiveDepth, 1.0, 0.0);
    glPopMatrix();
    /* end wheel */

    glutSwapBuffers();
}

void setMaterial(mProps* props) {
    currentMaterials = props;
    glMaterialfv(GL_FRONT, GL_AMBIENT, currentMaterials->ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, currentMaterials->diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, currentMaterials->specular);
    glMaterialf(GL_FRONT, GL_SHININESS, currentMaterials->shininess);
}

void update() {
    Theta += Speed / 10.0;
    display();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(650, 650);
    glutInitWindowPosition(50, 50);
    main_window = glutCreateWindow("Ferris Wheel");

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_NORMALIZE);

    p = gluNewQuadric();
    q = gluNewQuadric();
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    init();

    glEnable(GL_LIGHT0);
    setMaterial(&redPlasticMaterials);

    currentLighting = &whiteLighting;
    glLightfv(GL_LIGHT0, GL_AMBIENT, currentLighting->ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, currentLighting->diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, currentLighting->specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light0_pos);

    GLUI* control_panel = GLUI_Master.create_glui("Controls", 0, 50, 700);

    new GLUI_Button(control_panel, "Quit", 0, (GLUI_Update_CB)exit);

    new GLUI_Column(control_panel, true);

    GLUI_Rollout* eyePosRollout = new GLUI_Rollout(control_panel, "Camera Position", false);
    GLUI_Rollout* lookAtRollout = new GLUI_Rollout(control_panel, "Lookat Point", false);

    GLUI_Spinner* epxSpin = new GLUI_Spinner(eyePosRollout, "Camera X", GLUI_SPINNER_FLOAT, &EyeX, 0, look);
    GLUI_Spinner* epySpin = new GLUI_Spinner(eyePosRollout, "Camera Y", GLUI_SPINNER_FLOAT, &EyeY, 0, look);
    GLUI_Spinner* epzSpin = new GLUI_Spinner(eyePosRollout, "Camera Z", GLUI_SPINNER_FLOAT, &EyeZ, 0, look);


    GLUI_Spinner* laxSpin = new GLUI_Spinner(lookAtRollout, "Lookat X", GLUI_SPINNER_FLOAT, &LookAtX, 0, look);
    GLUI_Spinner* laySpin = new GLUI_Spinner(lookAtRollout, "Lookat Y", GLUI_SPINNER_FLOAT, &LookAtY, 0, look);
    GLUI_Spinner* lazSpin = new GLUI_Spinner(lookAtRollout, "Lookat Z", GLUI_SPINNER_FLOAT, &LookAtZ, 0, look);

    new GLUI_Column(control_panel, true);

    GLUI_Spinner* speedSpin = new GLUI_Spinner(control_panel, "Rotation Speed", GLUI_SPINNER_FLOAT, &Speed, 0, (GLUI_Update_CB)NULL);
    speedSpin->set_float_limits(-5.0f, 5.0f, GLUI_LIMIT_CLAMP);
    GLUI_Spinner* recSpin = new GLUI_Spinner(control_panel, "Recursive Depth", GLUI_SPINNER_INT, &RecursiveDepth, 0, (GLUI_Update_CB)NULL);
    recSpin->set_int_limits(0, 5, GLUI_LIMIT_CLAMP);

    control_panel->set_main_gfx_window(main_window);
    GLUI_Master.set_glutIdleFunc(update);

    glutMainLoop();
}