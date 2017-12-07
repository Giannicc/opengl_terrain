/*
Created by Gianni Ciccarelli
This program creates a Model class object by reading and storing the
data found in the .obj file specified in the constructor parameter.

Call <this>.drawNonTextured(colorArray) to draw the model with OpenGL
function calls for a Polygon using the color specified as a parameter

Call <this>.drawTextured() to draw the model with the texture specified
by textureFileName
*/
#ifndef __ReadObj_h__
#define __ReadObj_h__
#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#include <GLUT/glut.h>
#endif
//#include "SOIL.h"
#include <math.h>
using namespace std;

/*
A Face object stores its vertices, normals, and texture coordinates as
integer indices corresponding to the actual data stored in vectors in the
Model class
*/
class Face {
public:
	vector<int> faceVertexNums, faceNormNums, faceTexNums;
	Face & operator=(const Face &original) {
		faceVertexNums = original.faceVertexNums;
		faceNormNums = original.faceNormNums;
		faceTexNums = original.faceTexNums;
		return *this;
	}
};

class Model {
public:
	/*
	Construct a Model object using the data provided in the .obj file passed
	as a string containing its full name, the x/y/z coordinates
	of its pivot point, a vector of the names of all its submodels, and
	the name of its texture file
	*/
	Model(string objSource, 
		float _x, float _y, float _z,
		vector<string> children,
		string texName);
	/*
	Pass an array with 3 integers from 0 to 255 to specify the color in
	RGB order (colorArray[0] for red, [1] for blue, [2] for green)
	*/
	void drawNonTextured(int colorArray[3]);

	//Uses openGL to draw the model with the texture from the texture file
	//specified by textureFileName
	void drawTextured();
	//Set whether or not we want to draw normal vector lines coming off the
	//model faces, useful for  making sure the model's normals are loaded right
	void doNorms(bool veracity);
	//Changes model rotation data
	void rotateModel(float x, float y, float z);
	//Changes model position data
	void moveModel(float x, float y, float z);
	string name, textureFileName;
	
	/*
	The names of models in a Rig are checked, and if they match
	the name of a string in childNames that model is passed by reference
	to the children model pointer vector
	*/
	vector<string> childNames;
	vector<Model *> children;
	vector<Face> modelFaces;
	vector<vector<double>> modelVertices;
	vector<vector<double>> normVectors;
	vector<vector<double>> textureCoords;
	bool drawNormals;
	//pivotX, pivotY, and pivotZ store the position of the pivot point in the model
	//while x, y, and z store the corresponding offset for moving that part of the model specifically
	float pivotX, pivotY, pivotZ, x = 0, y = 0, z = 0, rotX, rotY, rotZ;
	GLuint textureID;
    unsigned char *texImage;
};
#endif