#include "ReadObj.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

/**
Model class functions
**/
/*
Model object constructor,
creates a Model with the given parameters
*/
Model::Model(string objSource, 
	float _x, float _y, float _z, 
	vector<string> children, string texName) {

	//Set data for the model
	childNames = children;
	name = objSource.substr(0, objSource.length() - 4);
	textureFileName = texName;
	rotX = rotY = rotZ = 0;
	pivotX = _x;
	pivotY = _y;
	pivotZ = _z;
	drawNormals = false;

	/*
	Start reading from the file specified by objSource
	*/
	ifstream source;
	source.open(objSource, ios::in);
	if (source.is_open()) {
		cout << "Reading " << objSource << endl;
		string input;	//We put lines gotten from "source" into input
		while (!source.eof()) {
			getline(source, input);
			istringstream iss(input);
			string lineHead;
			iss >> lineHead;	//We put the strings in input into a string stream
			if (lineHead == "v") {	//Read 3 doubles for the vertex coords
				vector<double> coords;
				double x, y, z;
				iss >> x >> y >> z;
				coords.push_back(x);
				coords.push_back(y);
				coords.push_back(z);
				modelVertices.push_back(coords);
			}
			else if (lineHead == "vn") {	//Reading vertex normal vector
				vector<double> norm;
				double x, y, z;
				iss >> x >> y >> z;
				norm.push_back(x);
				norm.push_back(y);
				norm.push_back(z);
				normVectors.push_back(norm);
			}
			else if (lineHead == "vt") {	//Texture coordinate!
				vector<double> coords;
				double x, y;
				iss >> x >> y;
				coords.push_back(x);
				coords.push_back(y);
				textureCoords.push_back(coords);
			}
			else if (lineHead == "f") {	//Reading face data
				Face newFace;
				string faceData;
				while (iss >> faceData) {
					int pos, index = 0;
					string vertNum, texNum, normNum;
					while (faceData[index] != '/') {	//Get the vertex number
						vertNum += faceData[index];
						index++;
					}
					if (vertNum != "") {
						newFace.faceVertexNums.push_back(stoi(vertNum) - 1);
					}
					else cout << "Error when reading face vertex data" << endl;
					index++;
					while (faceData[index] != '/') {	//Get the tex coord num
						texNum += faceData[index];
						index++;
					}
					if (texNum != "") {
						newFace.faceTexNums.push_back(stoi(texNum) - 1);
					}
					//else cout << "Error when reading face texture data" << endl;
					index++;
					while (faceData[index] != '\0') {
						normNum += faceData[index];
						index++;
					}
					if (normNum != "") {
						newFace.faceNormNums.push_back(stoi(normNum) - 1);
					}
					else cout << "Error when reading face normal data" << endl;
				}
				modelFaces.push_back(newFace);
			}
		}
	}
	else {
		cout << "Failed to open file" << endl;
		source.close();
		exit(1);
	}
	source.close();
    int w, h, comp;
    texImage = stbi_load(
                         textureFileName.c_str(),
                         &w,
                         &h,
                         &comp,
                         STBI_rgb_alpha);
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    GLenum image_format = GL_RGBA;
    GLint internal_format = GL_RGBA;
    GLint level = 0;
    glTexImage2D(GL_TEXTURE_2D, level, internal_format, w, h, 0, image_format, GL_UNSIGNED_BYTE, texImage);
    stbi_image_free(texImage);
	/*
	SOIL image loading stuff
	*/
    /*
	if (!textureCoords.empty()) {
		textureID = SOIL_load_OGL_texture(
			textureFileName.c_str(),
			SOIL_LOAD_AUTO,
			SOIL_CREATE_NEW_ID,
			SOIL_FLAG_INVERT_Y
		);
		// check for an error during the load process
		if (0 == textureID)
		{
			printf("SOIL loading error: '%s'\n", SOIL_last_result());
		}
		else {
			cout << "Successfully loaded texture for " << name << endl;
		}
	}
*/
}

//Draw the model as polygons with the solid color specified in the colorArray parameter
void Model::drawNonTextured(int colorArray[3]) {
	for (int i = 0; i < modelFaces.size(); i++) {
		glBegin(GL_POLYGON);
		//Ensure the colors don't go out of bounds
		int r = colorArray[0] % 255,
			g = colorArray[1] % 255,
			b = colorArray[2] % 255;
		if (r == 0 && colorArray[0] != 0) r = 255;
		if (g == 0 && colorArray[0] != 0) g = 255;
		if (b == 0 && colorArray[0] != 0) b = 255;
		glColor3ub(r, g, b);

		//Normal vector vector
		vector<double> norm = normVectors[modelFaces[i].faceNormNums[0]];
		for (int j = 0; j < modelFaces[i].faceVertexNums.size(); j++) {
			glNormal3f(norm[0], norm[1], norm[2]);
			//Get the vertex from the vector of vertices stored in the Model
			//object at the corresponding index given by the indices specified
			//in the vertex vector in the Face class
			vector<double> verts =
				modelVertices[modelFaces[i].faceVertexNums[j]];
			glVertex3f(verts[0], verts[1], verts[2]);
		}
		glEnd();
		
		//This is just stuff to draw normal vectors to test if my faces have
		//the correct normals
		if (drawNormals) {
			glBegin(GL_LINES);
			glColor3ub(255, 0, 0);
			vector<double> verts = 
				modelVertices[modelFaces[i].faceVertexNums[0]];
			glVertex3f(verts[0], verts[1], verts[2]);
			glVertex3f(
				verts[0] + norm[0], 
				verts[1] + norm[1], 
				verts[2] + norm[2]);
			glEnd();
		}
	}
}

//Draw the model by using openGL texture calls
void Model::drawTextured() {
	if (!textureCoords.empty()){
		for (int i = 0; i < modelFaces.size(); i++) {
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, textureID);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glBegin(GL_POLYGON);
			//Normal vector vector
			vector<double> norm = normVectors[modelFaces[i].faceNormNums[0]];
			for (int j = 0; j < modelFaces[i].faceVertexNums.size(); j++) {
				glNormal3f(norm[0], norm[1], norm[2]);
				//Get the vertex from the vector of vertices stored in the Model
				//object at the corresponding index given by the indices specified
				//in the vertex vector in the Face class
				vector<double> texCoords = textureCoords[modelFaces[i].faceTexNums[j]];
				//Implement the texture coordinates
				glTexCoord2f(texCoords[0], texCoords[1]);
				vector<double> verts =
					modelVertices[modelFaces[i].faceVertexNums[j]];
				glVertex3f(verts[0], verts[1], verts[2]);
			}
			glEnd();
			glDisable(GL_TEXTURE_2D);
		}
	}
	//If the model didn't have texture data we will just draw it as a plain white polygon
	else {
		int colorArray[] = {
			255, 255, 255
		};
		this->drawNonTextured(colorArray);
	}
}

//This just sets the bool we reference when we check if we want to draw
//normal vectors on the model
void Model::doNorms(bool veracity) {
	drawNormals = veracity;
}

//Changes model rotation data
void Model::rotateModel(float x, float y, float z) {
	rotX = x;
	rotY = y;
	rotZ = z;
}

//Changes model position data
void Model::moveModel(float _x, float _y, float _z) {
	x += _x;
	y += _y;
	z += _z;
}