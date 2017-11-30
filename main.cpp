#ifdef _WIN32
#include <iostream>
#include <GLUT/GLUT.H>
#elif __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#elif __linux__
/*
	HEEEEeeeeey Phillip do your includes heeeeeeere
*/
#endif

using namespace std;
void init() {

}

int main(int argc, char **argv) {
	/*
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("OpenGL Terrain");
	init();
	*/
	cout << "Hey some working separate includes" << endl;
	return 0;
}