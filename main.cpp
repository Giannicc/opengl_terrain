#ifdef _WIN32
#include <iostream>
#include <GLUT/GLUT.H>
#elif __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#elif __linux__
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif
#include "Terrain.h"
using namespace std;

FractalTerrain *terrain;

void terrainGen() {
	double exaggeration = .7;
	int lod = 5;
	int steps = 1 << lod;
	Triple **map = new Triple*[steps + 1];
	for (int i = 0; i < steps + 1; i++) {
		map[i] = new Triple[steps + 1];
	}
	RGB **colors = new RGB*[steps + 1];
	for (int i = 0; i < steps + 1; i++) {
		colors[i] = new RGB[steps + 1];
	}
	terrain = new FractalTerrain(lod, .5);
	for (int i = 0; i <= steps; ++i) {
		for (int j = 0; j <= steps; ++j) {
			double x = 1.0 * i / steps, z = 1.0 * j / steps;
			double altitude = (*terrain).getAltitude(x, z);
			map[i][j] = Triple(x, altitude * exaggeration, z);
			colors[i][j] = (*terrain).getColor(x, z);
		}
	}
}

void display() {
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -5.0f);
	glPushMatrix();
	glColor3ub(255, 0, 0);
	glutSolidTeapot(1.0);
	glPopMatrix();
	glutSwapBuffers();
}

//OpenGL reshape function
void reshape(int w, int h) {
    if(h == 0) h = 1;
    
    float ratio = (float)w / (float)h;
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    glViewport(0, 0, w, h);
    gluPerspective(45, ratio, 1, 100);
}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'q':
		exit(0);
		break;
	}
	return;
}

void callbacks() {
	glutDisplayFunc(display);
	glutIdleFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glEnable(GL_DEPTH_TEST);
	//Now enable some lighting stuff...
}


int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("OpenGL Terrain");
	callbacks();
	glutMainLoop();
	return 0;
}
