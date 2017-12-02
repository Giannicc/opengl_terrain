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
Triangle *triangles;

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
	/*
	Creating array of Triangles
	*/
	int numTriangles = (steps * steps * 2);
	triangles = new Triangle[numTriangles];

	int triangle = 0;
	for (int i = 0; i < steps; ++i) {
		for (int j = 0; j < steps; ++j) {
			triangles[triangle++] = Triangle(i, j, i + 1, j, i, j + 1);
			triangles[triangle++] = Triangle(i + 1, j, i + 1, j + 1, i, j + 1);
		}
	}
	double ambient = .3;
	double diffuse = 4.0;
	Triple **normals = new Triple*[steps + 1];
	for (int i = 0; i < steps + 1; i++) {
		normals[i] = new Triple[steps + 1];
	}
	Triple sun = Triple(3.6, 3.9, 0.6);

	for (int i = 0; i < numTriangles; ++i)
		for (int j = 0; j < 3; ++j)
			normals[i][j] = Triple(0.0, 0.0, 0.0);
	/* compute triangle normals and vertex averaged normals */
	for (int i = 0; i < numTriangles; ++i) {
		Triple v0 = map[triangles[i].i[0]][triangles[i].j[0]],
			v1 = map[triangles[i].i[1]][triangles[i].j[1]],
			v2 = map[triangles[i].i[2]][triangles[i].j[2]];
		Triple normal = v0.subtract(v1).cross(v2.subtract(v1)).normalize();
		triangles[i].n = normal;
		for (int j = 0; j < 3; ++j) {
			normals[triangles[i].i[j]][triangles[i].j[j]] =
				normals[triangles[i].i[j]][triangles[i].j[j]].add(normal);
		}
	}
	/* compute vertex colors and triangle average colors */
	for (int i = 0; i < numTriangles; ++i) {
		RGB avg = RGB(0.0, 0.0, 0.0);
		for (int j = 0; j < 3; ++j) {
			int k = triangles[i].i[j], l = triangles[i].j[j];
			Triple vertex = map[k][l];
			RGB color = colors[k][l];
			Triple normal = normals[k][l].normalize();
			Triple light = vertex.subtract(sun);
			double distance2 = light.length2();
			double dot = light.normalize().dot(normal);
			double lighting = ambient + diffuse * ((dot < 0.0) ? -dot : 0.0) / distance2;
			color = color.scale(lighting);
			triangles[i].color[j] = color;
			avg = avg.add(color);
		}
		triangles[i].color = new RGB(avg.scale(1.0 / 3.0).toRGB());
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
