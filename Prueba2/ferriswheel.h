#pragma once
/* defines properties for a material */
#include <GL/glut.h>
#include <GL/glui.h>

typedef struct {
    GLfloat ambient[4];
    GLfloat diffuse[4];
    GLfloat specular[4];
    GLfloat shininess;
} mProps;

/* defines properties for lighting */
typedef struct {
    GLfloat ambient[4];
    GLfloat diffuse[4];
    GLfloat specular[4];
} lProps;

/* function declarations */
void look(int id);
void setMaterial(mProps*);
void update();