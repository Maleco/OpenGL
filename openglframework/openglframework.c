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

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "glslshaders.h"


typedef int bool;
#define true 1
#define false 0
#define SPHERE_N (20)
#define apertureSamples 2.0
#define CONST_C 10 / sqrt(apertureSamples)
#define GOLDEN_ANGLE 137.508
#define eyeDistance	1000
#define focusDistance 200

int oldX, oldY, deltaX, deltaY;		//Keep track of mouse-movements
bool Rotate, Zoom;			//Keep track of transformations
float zoomLevel;

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


void init(void)
{
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

   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   glEnable(GL_DEPTH_TEST);
   initGLSLProgram("vertexshader.glsl","fragmentshader.glsl");
}



void display(void){
	float i;
	float x = 0.0, y = 0.0;

	glClear(GL_ACCUM_BUFFER_BIT);

	for(i = 0.0; i < apertureSamples; i++){
		float r = CONST_C * sqrt(i);
		float theta = i * GOLDEN_ANGLE;
		x = r * cos(theta);
		y = r * sin(theta);

		/* Clear all pixels */
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();
		gluLookAt(200.0+x,200.0+y,eyeDistance,200.0,200.0,focusDistance,0.0,1.0,0.0);

		//Zoom
		glTranslatef(1.0, 1.0, zoomLevel*100);

		//Rotate around point (200,200,200)
		glTranslatef(200, 200, 200);
		glRotatef(deltaY,1,0,0);
		glRotatef(deltaX,0,1,0);
		glTranslatef(-200, -200, -200);

		//Insert spheres
		setGlMaterial(0.0f,0.0f,1.0f,0.2,0.7,0.5,64);
		glPushMatrix();
		glTranslated(90,320,100);
		glutSolidSphere(50,SPHERE_N,SPHERE_N);
		glPopMatrix();

		setGlMaterial(0.0f,1.0f,0.0f,0.2,0.3,0.5,8);
		glPushMatrix();
		glTranslated(210,270,300);
		glutSolidSphere(50,SPHERE_N,SPHERE_N);
		glPopMatrix();

		setGlMaterial(1.0f,0.0f,0.0f,0.2,0.7,0.8,32);
		glPushMatrix();
		glTranslated(290,170,150);
		glutSolidSphere(50,SPHERE_N,SPHERE_N);
		glPopMatrix();

		setGlMaterial(1.0f,0.8f,0.0f,0.2,0.8,0.0,1);
		glPushMatrix();
		glTranslated(140,220,400);
		glutSolidSphere(50,SPHERE_N,SPHERE_N);
		glPopMatrix();

		setGlMaterial(1.0f,0.5f,0.0f,0.2,0.8,0.5,32);
		glPushMatrix();
		glTranslated(110,130,200);
		glutSolidSphere(50,SPHERE_N,SPHERE_N);
		glPopMatrix();

		glAccum(GL_ACCUM, (1/apertureSamples));
		glFlush();
	}
	glAccum(GL_RETURN, 1.0);
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
    gluPerspective(2.0*atan2(h/2.0,1000.0)*180.0/M_PI,(GLdouble)w/(GLdouble)h,500,1000);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv)
{
#if defined(NEED_GLEW)
    GLenum err;
#endif

    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_ACCUM);
    glutInitWindowSize(400,400);
    glutInitWindowPosition(220,100);
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
