#include <vector>
using namespace std;
class RGB {
public:
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
	vector<vector<double>> terrainMap;
private:
	double roughness, min, max;
	int divisions;
	//Random rng;
};