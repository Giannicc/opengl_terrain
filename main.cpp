#include "Terrain.h"
#include "ReadObj.h"
#include <stdlib.h>
#ifdef _WIN32
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

FractalTerrain *terrain;
vector<Triangle> triangles;
vector<vector<Triple>> map;
vector<vector<RGB>> colors;
Model *palmTree;
Model *palmLeaves;
double exaggeration = 50;
int lod = 6;
int steps = 1 << lod;
int numTriangles = (steps * steps * 2);

void terrainGen() {
	terrain = new (nothrow) FractalTerrain(lod, 0.5);
	for (int i = 0; i < steps + 1; i++) {
		vector<Triple> newTrip;
		map.push_back(newTrip);
		vector<RGB> newColorVec;
		colors.push_back(newColorVec);
		for (int j = 0; j < steps + 1; j++) {
			map[i].push_back(Triple());
			colors[i].push_back(RGB());
		}
	}
	for (int i = 0; i <= steps; ++i) {
		for (int j = 0; j <= steps; ++j) {
			double x = 1.0 * i / steps, z = 1.0 * j / steps;
			double altitude = (*terrain).getAltitude(x, z);
			map[i][j] = Triple(x, altitude * exaggeration, z);
			colors[i][j] = (*terrain).getColor(x, z);
		}
	}

    /*
      Shading
    */
    double ambient = .3;
    double diffuse = 4.0;
    Triple **normals = new (nothrow) Triple*[steps + 1];
    for (int i = 0; i < steps + 1; i++) {
	normals[i] = new (nothrow) Triple[steps + 1];
    }
    Triple sun = Triple(3.6, 3.9, 0.6);
    double **shade = new (nothrow) double*[steps + 1];
    for (int i = 0; i < steps + 1; i++) {
	shade[i] = new (nothrow) double[steps + 1];
    }
    for (int i = 0; i <= steps; i++) {
	for (int j = 0; j <= steps; j++) {
	    shade[i][j] = 1.0;
	    Triple vertex = map[i][j];
	    Triple ray = sun.subtract(vertex);
	    double distance = steps * sqrt(ray.x * ray.x + ray.z * ray.z);
	    /* step along ray in horizontal units of grid width */
	    for (double place = 1.0; place < distance; place += 1.0) {
		Triple sample = vertex.add(ray.scale(place / distance));
		double sx = sample.x, sy = sample.y, sz = sample.z;
		if ((sx < 0.0) || (sx > 1.0) || (sz < 0.0) || (sz > 1.0))
		    break; /* stepped off terrain */
		double ground = exaggeration * (*terrain).getAltitude(sx, sz);
		if (ground >= sy) {
		    shade[i][j] = 0.0;
		    break;
		}
	    }
	}
    }
    /*
      Creating array of Triangles
    */
   	for (int i = 0; i < numTriangles; i++) {
		triangles.push_back(Triangle());
	}

    int triangle = 0;
	for (int i = 0; i < steps; i++) {
		for (int j = 0; j < steps; j++) {
			triangles[triangle++] = Triangle(i, j, i + 1, j, i, j + 1);
			triangles[triangle++] = Triangle(i + 1, j, i + 1, j + 1, i, j + 1);
		}
	}
	/*
	for (int i = 0; i < numTriangles; i++)
	for (int j = 0; j < 3; ++j)
	    normals[i][j] = Triple(0.0, 0.0, 0.0);
		*/
	/*
    // compute triangle normals and vertex averaged normals 
    for (int i = 0; i < numTriangles; i++) {
	if (i > 32) i = 0;
	Triple v0 = map[triangles[i].i[0]][triangles[i].j[0]],
	    v1 = map[triangles[i].i[1]][triangles[i].j[1]],
	    v2 = map[triangles[i].i[2]][triangles[i].j[2]];
	Triple normal = v0.subtract(v1).cross(v2.subtract(v1)).normalize();
	triangles[i].normal = normal;
	for (int j = 0; j < 3; ++j) {
	    normals[triangles[i].i[j]][triangles[i].j[j]] =
		normals[triangles[i].i[j]][triangles[i].j[j]].add(normal);
	}
    }
    for (int i = 0; i < numTriangles; i++) {
	RGB avg = RGB(0.0, 0.0, 0.0);
	for (int j = 0; j < 3; j++) {
	    int k = triangles[i].i[j], l = triangles[i].j[j];
	    Triple vertex = map[k][l];
	    RGB color = colors[k][l];
	    Triple normal = normals[k][l].normalize();
	    Triple light = vertex.subtract(sun);
	    double distance2 = light.length2();
	    double dot = light.normalize().dot(normal);
	    double shadow = shade[k][l];
	    double lighting = ambient + diffuse * ((dot < 0.0) ? -dot : 0.0) /
		distance2 * shadow;
	    color = color.scale(lighting);
	    //triangles[i].color[j] = color;
	    avg = avg.add(color);
	}
	triangles[i].color = RGB(avg.scale(1.0 / 3.0));
    }
	*/
}

void drawTerrain() {
	for (int i = 0; i < steps; i++) {
		for (int j = 0; j < steps; j++) {
			glBegin(GL_TRIANGLES);
			glColor3f(0.0, 0.7, 0.3);
			glVertex3f(i, map[i][j].y, j);
			glVertex3f(i + 1, map[i + 1][j].y, j);
			glVertex3f(i, map[i][j + 1].y, j + 1);
			glEnd();
		}
	}
}

void drawPalmTree() {
	(*palmTree).drawTextured();
	(*palmLeaves).drawTextured();
}

void display() {
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glPushMatrix();
    glTranslatef(-(steps / 2), -exaggeration, - (steps));
    //glColor3ub(255, 0, 0);
    //glutSolidTeapot(1.0);
    drawTerrain();
	glTranslatef(steps/2, map[steps/2][steps/2].y + 10, 0);
	glColor3f(1.0, 1.0, 1.0);
	drawPalmTree();
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
	Model *tree = new Model("palm_tree.obj", 0, 0, 0, {}, "trunk_texture.jpg");
	Model *leaves = new Model("palm_leaves.obj", 0, 0, 0, {}, "leaf.png");
	palmTree = tree;
	palmLeaves = leaves;

    glutDisplayFunc(display);
    glutIdleFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glEnable(GL_DEPTH_TEST);
    terrainGen();
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
