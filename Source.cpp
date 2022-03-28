#include <iostream>
#include <vector>
#include <map>
#include <locale>
#include <chrono>
#include <iomanip>
#include "Cube.h"
#include "Selection.h"
#define __CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW



int main() {
	srand(time(NULL));
	setlocale(LC_ALL, "Russian");
	Cube* cube = new Cube();
	Selection* select = new Selection(cube);
	delete cube;
	delete select;

	_CrtDumpMemoryLeaks();
	return 0;
}