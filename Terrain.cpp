#include "Terrain.h"
FractalTerrain::FractalTerrain(int lod, double rough) {
	srand(time(NULL));
	terrain = new double*[divisions + 1];
	for (int i = 0; i < divisions + 1; i++) {
		terrain[i] = new double[divisions + 1];
	}
	roughness = rough;
	divisions = 1 << lod;
	terrain[0][0] = (rand() % 400000) / 100000;
	terrain[0][divisions] = (rand() % 400000) / 100000;
	terrain[divisions][divisions] = (rand() % 400000) / 100000;
	terrain[divisions][0] = (rand() % 400000) / 100000;
	for (int i = 0; i < lod; i++) {
		int q = 1 << i, r = 1 << (lod - i), s = r >> 1;
		for (int j = 0; j < divisions; j += r) {
			for (int k = 0; k < divisions; k += r)
				diamond(j, k, r, rough);
		}
		if (s > 0) {
			for (int j = 0; j <= divisions; j += s) {
				for (int k = (j + s) % r; k <= divisions; k += r) {
					square (j - s, k - s, r, rough);
				}
			}
		}
		rough *= roughness;
	}
	min = max = terrain[0][0];
}

FractalTerrain::~FractalTerrain() {
	for (int i = 0; i < divisions + 1; i++) {
		delete [] terrain [i];
	}
	delete [] terrain;
}