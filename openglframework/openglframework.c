/*
* An OpenGL template/framework file for the Computer Graphics course
* at the University of Groningen.
*/

// If windows is used, windows.h should be included (before gl.h and glu.h)
#if defined(_WIN32)
#include <windows.h>
#endif

// If you use GLUT you do not need to include gl.h and glu.h
// as glut.h guarantees that gl.h and glu.h are properly
// included.
// So if you do not wish to use  glut.h, uncomment the following lines.
//#include <GL/gl.h>
//#include <GL/glu.h>
#define GL_GLEXT_PROTOTYPES
#if defined(NEED_GLEW)
#include "glew.h"
#endif

// Apparently Mac OS X puts the GLUT headers somewhere different.
// For windows we use freeglut.
#if defined(__APPLE__)&& defined(__MACH__)
#include <GLUT/glut.h>
#elif defined(_WIN32)
#include <GL/freeglut.h>
#else
#include <GL/glut.h>
#endif

#include "glm.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "glslshaders.h"


typedef int bool;
#define true 1
#define false 0
#define SPHERE_N (20)
#define apertureSamples 1.0
#define CONST_C 10 / sqrt(apertureSamples)
#define GOLDEN_ANGLE 137.508
#define eyeDistance	1000
#define focusDistance 200

int oldX, oldY, deltaX, deltaY;		//Keep track of mouse-movements
bool Rotate, Zoom;			//Keep track of transformations
float zoomLevel;
GLMmodel *model;
GLuint vboDots;
GLuint vboTriangles;

///Functions
void setGlMaterial(GLfloat r, GLfloat g, GLfloat b, GLfloat ka, GLfloat kd, GLfloat ks, GLfloat n)
{
    GLfloat ambient[] = {ka*r,ka*g,ka*b,1.0};
    GLfloat diffuse[] = {kd*r,kd*g,kd*b,1.0};
    GLfloat specular[] = {ks,ks,ks,1.0};
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, n);
}

void glmInitVBO(GLMmodel *model)
{
	glGenBuffersARB(1, &vboDots);
	glGenBuffersARB(1, &vboTriangles);

	//Hold the model's vertices (points)
	//vboDots = calloc(model->numvertices, sizeof(GLuint));
	//vboDots = model->vertices;

	//Hold the model's triangles
	//vboTriangles = calloc(model->numtriangles, sizeof(GLuint));
	//vboTriangles = model->triangles;

	glBindBufferARB(GL_ARRAY_BUFFER_ARB, vboDots);
	glBufferDataARB(GL_ElEMENT_ARRAY_BUFFER_ARB, sizeof(model->numvertices), model->numvertices, GL_STATIC_DRAW_ARB);

	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, vboTriangles);
	glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, sizeof(model->triangles), model->triangles, GL_STATIC_DRAW_ARB);


}

void glmDrawVBO(){

}

void init(void){

	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 50.0 };

	GLfloat light_position[] = {-200.0,600.0,1500.0, 1.0};
	GLfloat light_ambient[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };

	glClearColor (0.0, 0.0, 0.0, 0.0);
	glShadeModel (GL_SMOOTH);

	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);


	model = glmReadOBJ("obj/devilduk.obj");
	if(!model) printf("Object niet geladen\n");
	glmFacetNormals(model);
	glmVertexNormals(model, 90.0 );

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
	initGLSLProgram("vertexshader.glsl","fragmentshader.glsl");
}


void display(void){

	/* Clear all pixels */
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(0.0,0.0,3.0,0.0,0.0,0.0,0.0,1.0,0.0);

	//Zoom
	glTranslatef(1.0, 1.0, zoomLevel);

	//Rotate around origin
	glRotatef(deltaY,1,0,0);
	glRotatef(deltaX,0,1,0);

	//Draw model
	glmUnitize(model);
	glmScale(model, 2);
	glmDraw(model, GLM_SMOOTH);
	glPushMatrix();

	glAccum(GL_ACCUM, (1/apertureSamples));
	glFlush();

	glutSwapBuffers();
}

void mouse(int button, int state, int x, int y){
    oldX = x;
	oldY = y;

    if(state == GLUT_DOWN){
        if(button == GLUT_LEFT_BUTTON){
          Rotate = true;
        }
        if(button == GLUT_RIGHT_BUTTON){
		  Zoom = true;
        }
    } else {
        if(button == GLUT_LEFT_BUTTON){
		  Rotate = false;
        }
        if(button == GLUT_RIGHT_BUTTON){
		  Zoom = false;
        }
    }
}

void motion(int x, int y){
	if(Rotate){
		deltaX += (x - oldX);
		deltaY += (y - oldY);
	}
	if(Zoom){
		zoomLevel += (y - oldY) / 20.0;
	}
	oldX = x; oldY = y;
	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y){
    switch (key) {
        case 'q':
        case 'Q':
        case 27: // ESC key
            printf("Exiting...\n");
            exit(0);
            break;
    }
}

void reshape(int w, int h){
    glViewport(0,0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0,(GLdouble)w/(GLdouble)h,1.5,20.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv)
{
#if defined(NEED_GLEW)
    GLenum err;
#endif

    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_ACCUM);
    glutInitWindowSize(800,800);
    glutInitWindowPosition(400, 300);
    glutCreateWindow("Computer Graphics - OpenGL framework");

#if defined(NEED_GLEW)
    /* Init GLEW if needed */
    err = glewInit();
    if (GLEW_OK != err) {
        /* Problem: glewInit failed, something is seriously wrong. */
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
        exit(1);
    }
    fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
#endif

    /* Select clearing (background) color */
    glClearColor(0.0,0.0,0.0,0.0);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);

    /* Register GLUT callback functions */
    init();
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutKeyboardFunc(keyboard);
    glutReshapeFunc(reshape);

    glutMainLoop();

    return 0;
}
