#include <iostream>
#include <vector>
#include <map>
#include <locale>
#include <chrono>
#include <iomanip>
#include "cube.h"
#include "selection.h"
#define __CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW

struct timenn {
	uint8_t a;
	uint8_t b;
};

int main() {
	srand(time(NULL));
	setlocale(LC_ALL, "Russian");
	cube* cube = new cube();
	selection* select = new selection(cube);
	delete cube;
	delete select;
	_CrtDumpMemoryLeaks();
	return 0;
}