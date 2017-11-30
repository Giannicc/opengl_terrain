#include "Terrain.h"

FractalTerrain::FractalTerrain(int lod, double rough) {
	roughness = rough;
	divisions = 1 << lod;
}