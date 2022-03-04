<<<<<<< HEAD
﻿#include <iostream>
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
	std::vector<std::string> food = { "Пицца" , "Оливье", "Чай" , "Вода", "Кофе" };
	std::vector<std::string> datetime = {
		"15 мая 16:00" , "16 мая 13:46", "16 мая 15:06" , "16 мая 16:45", "17 мая 11:13",
		"20 мая 14:07" , "21 мая 10:22", "21 мая 15:54" , "22 мая 15:00", "22 мая 16:37",
		"26 мая 12:03" , "27 мая 14:32", "28 мая 15:11" , "29 мая 11:56", "30 мая 14:23",
		"31 мая 11:07" , "1 июня 10:15", "2 июня 16:17" , "3 июня 13:23", "4 июня 15:13"
	};
	std::vector<std::string> year = {
		"2019" , "2020", "2021"
	};
	std::vector<std::string> dimensions = { "Покупка","Дата","Год" };
	std::vector<std::string> measures = { "Цена", "Масса", "Расход" };
	srand(time(NULL));
	setlocale(LC_ALL, "Russian");
	Cube* cube = new Cube();
	cube->add_Dimension(dimensions[2]);
	cube->add_Dimension(dimensions[1]);
	cube->add_Dimension(dimensions[0]);
	cube->add_Measure(measures[2]);
	cube->add_Measure(measures[1]);
	cube->add_Measure(measures[0]);
	for (int year_c = 0; year_c < 10; year_c++) {
		for (int datetime_c = 0; datetime_c < 365; datetime_c++) {
			for (int food_c = 0; food_c < 1000; food_c++) {
				cube->add_Fact(rand() % 1000 + 100, "Цена", { "200" + std::to_string(year_c),"День " + std::to_string(datetime_c),"Еда " + std::to_string(food_c) });
			}
		}
	}
	Selection* select = new Selection(cube);
	std::vector<int> maxs;
	std::vector<int> mins;
	std::chrono::steady_clock::time_point begin, end, begin_offset, end_offset;
	std::chrono::microseconds elapsed_mks = std::chrono::microseconds::zero(), elapsed_mks_offset= std::chrono::microseconds::zero();
	for (int j = 1; j < 3; j++) {
		unsigned long long tens = pow(2,j);
		unsigned long long min = 500'000'000'000, max = 0;
		for (int i = 0; i < 10; i++) {
			unsigned long long k = 0;
			begin = std::chrono::steady_clock::now();
			while (k < tens) {
				select->make("Год", { "2005", "2009" }, { "Цена" });
				select->make("Дата", { "День 14" , "День 16" , "День 321", "День 788", "День 4124" , "День 4421" });
				begin_offset = std::chrono::steady_clock::now();
				select->clean();
				k++;
				end_offset = std::chrono::steady_clock::now();
				elapsed_mks_offset += std::chrono::duration_cast<std::chrono::microseconds>(end_offset - begin_offset);
			}
			end = std::chrono::steady_clock::now();
			elapsed_mks = std::chrono::duration_cast<std::chrono::microseconds>(end - begin);
			elapsed_mks -= elapsed_mks_offset;
			elapsed_mks_offset = std::chrono::microseconds::zero();
			if (elapsed_mks.count() / tens > max)
				max = elapsed_mks.count() / tens;
			if (elapsed_mks.count() / tens < min)
				min = elapsed_mks.count() / tens;
		}
		maxs.push_back(max);
		mins.push_back(min);
	}
	for (auto it = maxs.begin(); it != maxs.end(); it++) {
		std::cout << *it << " ";
	}
	std::cout << std::endl;
	for (auto it = mins.begin(); it != mins.end(); it++) {
		std::cout << *it << " ";
	}

	delete cube;
	delete select;

	_CrtDumpMemoryLeaks();
	return 0;
}
=======
﻿#include <iostream>
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
	std::vector<std::string> food = { "Пицца" , "Оливье", "Чай" , "Вода", "Кофе" };
	std::vector<std::string> datetime = {
		"15 мая 16:00" , "16 мая 13:46", "16 мая 15:06" , "16 мая 16:45", "17 мая 11:13",
		"20 мая 14:07" , "21 мая 10:22", "21 мая 15:54" , "22 мая 15:00", "22 мая 16:37",
		"26 мая 12:03" , "27 мая 14:32", "28 мая 15:11" , "29 мая 11:56", "30 мая 14:23",
		"31 мая 11:07" , "1 июня 10:15", "2 июня 16:17" , "3 июня 13:23", "4 июня 15:13"
	};
	std::vector<std::string> year = {
		"2019" , "2020", "2021"
	};
	std::vector<std::string> dimensions = { "Покупка","Дата","Год" };
	std::vector<std::string> measures = { "Цена", "Масса", "Расход" };
	srand(time(NULL));
	setlocale(LC_ALL, "Russian");
	Cube* cube = new Cube();
	cube->add_Dimension(dimensions[2]);
	cube->add_Dimension(dimensions[1]);
	cube->add_Dimension(dimensions[0]);
	cube->add_Measure(measures[2]);
	cube->add_Measure(measures[1]);
	cube->add_Measure(measures[0]);
	for (int year_c = 0; year_c < 10; year_c++) {

		for (int datetime_c = 0; datetime_c < 365; datetime_c++) {
			
			for (int food_c = 0; food_c < 1000; food_c++) {
				cube->add_Fact(rand() % 1000 + 100, "Цена", { "200" + std::to_string(year_c),"День " + std::to_string(datetime_c),"Еда " + std::to_string(food_c) });
						}
		}
	}
	Selection* select = new Selection(cube);
	std::vector<int> maxs;
	std::vector<int> mins;
	std::chrono::steady_clock::time_point begin, end, begin_offset, end_offset;
	std::chrono::microseconds elapsed_ms = std::chrono::microseconds::zero(), elapsed_mks_offset= std::chrono::microseconds::zero();
	for (int j = 1; j < 3; j++) {
		unsigned long long tens = pow(2,j);
		unsigned long long min = 500'000'000'000, max = 0;
		for (int i = 0; i < 10; i++) {
			unsigned long long k = 0;
			begin = std::chrono::steady_clock::now();
			while (k < tens) {
				select->make("Год", { "2005", "2009" }, { "Цена" });
				select->make("Дата", { "День 14" , "День 16" , "День 321", "День 788", "День 4124" , "День 4421" });
				begin_offset = std::chrono::steady_clock::now();
				select->clean();
				k++;
				end_offset = std::chrono::steady_clock::now();
				elapsed_mks_offset += std::chrono::duration_cast<std::chrono::microseconds>(end_offset - begin_offset);
			}
			end = std::chrono::steady_clock::now();
			elapsed_mks = std::chrono::duration_cast<std::chrono::microseconds>(end - begin);
			
			elapsed_mks -= elapsed_mks_offset;
			
			elapsed_mks_offset = std::chrono::microseconds::zero();
			
			if (elapsed_mks.count() / tens > max)
				max = elapsed_mks.count() / tens;
			if (elapsed_mks.count() / tens < min)
				min = elapsed_mks.count() / tens;
		}
		maxs.push_back(max);
		mins.push_back(min);
	}
	for (auto it = maxs.begin(); it != maxs.end(); it++) {
		std::cout << *it << " ";
	}
	std::cout << std::endl;
	for (auto it = mins.begin(); it != mins.end(); it++) {
		std::cout << *it << " ";
	}

	delete cube;
	delete select;

	_CrtDumpMemoryLeaks();
	return 0;
}
>>>>>>> 414c979a3c9e4aba3a567d604bec3690b13c15c5
