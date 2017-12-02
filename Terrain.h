#include <vector>
#include <cstdlib>
#include <time.h>
#include <math.h>
using namespace std;
class RGB {
public:
	RGB() {
		//r = g = b = 0;
	}
	RGB(double _r, double _g, double _b) {
		r = _r;
		g = _g;
		b = _b;
	}
	RGB add(RGB rgb) {
		double newR, newG, newB;
		newR = r + rgb.r;
		newG = g + rgb.g;
		newB = b + rgb.b;
		if (newR < 0) newR = 0;
		if (newG < 0) newG = 0;
		if (newB < 0) newB = 0;
		RGB *newRGB = new RGB(newR, newG, newB);
		return *newRGB;
	}
	RGB subtract(RGB rgb) {
		double newR, newG, newB;
		newR = r - rgb.r;
		newG = g - rgb.g;
		newB = b - rgb.b;
		if (newR > 1.0) newR = 1.0;
		if (newG > 1.0) newG = 1.0;
		if (newB > 1.0) newB = 1.0;
		if (newR < 0) newR = 0;
		if (newG < 0) newG = 0;
		if (newB < 0) newB = 0;
		RGB *newRGB = new RGB(newR, newG, newB);
		return *newRGB;
	}
	RGB scale(double scale) {
		double newR, newG, newB;
		newR = r * scale;
		newG = g * scale;
		newB = b * scale;
		if (newR > 1.0) newR = 1.0;
		if (newG > 1.0) newG = 1.0;
		if (newB > 1.0) newB = 1.0;
		if (newR < 0) newR = 0;
		if (newG < 0) newG = 0;
		if (newB < 0) newB = 0;
		RGB *newRGB = new RGB(newR, newG, newB);
		return *newRGB;
	}
	double r, g, b;
};

class Terrain {
public:
	double getAltitude(double i, double j);
	RGB getColor(double i, double j);
};

class FractalTerrain :Terrain {
public:
	FractalTerrain(int lod, double roughness);
	~FractalTerrain();
	double **terrain;
	double rnd();
	double getAltitude(double i, double j);
	RGB getColor(double i, double j);
private:
	void diamond(int x, int y, int side, double scale);
	void square(int x, int y, int side, double scale);

	RGB blue = RGB(0.0, 0.0, 1.0),
		green = RGB(0.0, 1.0, 0.0),
		white = RGB(1.0, 1.0, 1.0);
	double roughness, min, max;
	int divisions;
};

class Triangle {
public:
	Triangle() {

	}
	Triangle(int i0, int j0, int i1, int j1, int i2, int j2) {
		i[0] = i0;
		i[1] = i1;
		i[2] = i2;
		j[0] = j0;
		j[1] = j1;
		j[2] = j2;
	}
	int *i = new int[3];
	int *j = new int[3];
	Triple n;
	RGB *rgb = new RGB[3];
	RGB color;
};

class Triple {
public:
	Triple() {
		//x = y = z = 0;
	}
	Triple(double a, double b, double c) {
		x = a;
		y = b;
		z = c;
	}
	Triple add(Triple t) {
		return Triple(x + t.x, y + t.y, z + t.z);
	}
	Triple subtract(Triple t) {
		return Triple(x - t.x, y - t.y, z - t.z);
	}

	Triple cross(Triple t) {
		return Triple(y * t.z - z * t.y, z * t.x - x * t.z,
			x * t.y - y * t.x);
	}

	double dot(Triple t) {
		return x * t.x + y * t.y + z * t.z;
	}

	double length2() {
		return dot(*this);
	}

	Triple normalize() {
		return scale(1.0 / sqrt(length2()));
	}

	Triple scale(double scale) {
		return Triple(x * scale, y * scale, z * scale);
	}
private:
	double x, y, z;
};