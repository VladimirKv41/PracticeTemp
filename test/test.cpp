#include "gtest/gtest.h"
#include <cmath>
#include "cube.h"
#include "selection.h"
#include "variant.h"
#include "cubetime.h"
#include "date.h"
#include "datetime.h"
#include <fstream>
#include <chrono>

double facts_temp[5][20] =
{
	{21,17,14,16,25,20,16,13,12,23,15,17,14,16,21,20,17,13,12,21},
	{19,18,18,20,19,18,17,17,15,23,19,27,21,20,24,24,17,18,19,21},
	{20,18,17,17,20,20,18,17,17,20,17,15,21,17,24,21,18,16,17,19},
	{12,12,14,15,14,13,13,15,14,15,16,17,16,15,13,14,17,20,16,13},
	{25,26,25,24,26,26,27,30,25,26,27,28,28,30,29,29,28,31,29,28}
};

double facts_hum[5][20] =
{
	{81,67,64,66,75,70,66,63,72,73,65,67,64,66,71,70,77,63,62,71},
	{69,78,68,70,69,68,67,77,65,83,69,77,71,70,74,84,67,78,69,71},
	{70,68,77,67,70,70,78,67,67,70,77,65,71,67,84,71,68,66,77,69},
	{62,62,64,75,64,73,63,65,64,65,66,77,66,75,63,64,67,70,66,73},
	{75,76,75,74,86,76,77,80,75,76,77,78,88,80,79,79,78,81,79,78}
};

std::string cities[5] = { "Зеленоград" , "Ржев", "Тверь" , "Санкт-Петербург", "Cевастополь" };
std::string dates[20] = {
	"15 мая" , "16 мая", "17 мая" , "18 мая", "19 мая",
	"20 мая" , "21 мая", "22 мая" , "23 мая", "25 мая",
	"26 мая" , "27 мая", "28 мая" , "29 мая", "30 мая",
	"31 мая" , "1 июня", "2 июня" , "3 июня", "4 июня"
};

TEST(TestCube, Constructor) {
	ASSERT_NO_THROW(Cube* cube = new Cube()) << "Constructor causes error";
}

TEST(TestCube, Destructor) {
	Cube* cube = new Cube();
	ASSERT_NO_THROW(delete cube) << "Destructor causes error";
}

TEST(TestCube, AddingDimensionRussian) {
	Cube* cube = new Cube();
	EXPECT_TRUE(cube->addDimension("Время")) << "New dimension but not added";
	EXPECT_TRUE(cube->addDimension("Время123@@@_033--")) << "New dimension but not added";
	EXPECT_TRUE(cube->addDimension("123@@@_033--")) << "New dimension but not added";
	delete cube;
}

TEST(TestCube, AddingDimensionEnglish) {
	Cube* cube = new Cube();
	EXPECT_TRUE(cube->addDimension("Time")) << "New dimension but not added";
	EXPECT_TRUE(cube->addDimension("Time 123@@@_033--")) << "New dimension but not added";
	EXPECT_TRUE(cube->addDimension(" 123@@@_033--")) << "New dimension but not added";
	delete cube;
}

TEST(TestCube, AddingDimensionEmpty) {
	Cube* cube = new Cube();
	EXPECT_TRUE(cube->addDimension("")) << "New dimension but not added";
	EXPECT_FALSE(cube->addDimension("")) << "Added twice";
	delete cube;
}

TEST(TestCube, AddingMetricRussian) {
	Cube* cube = new Cube();
	EXPECT_TRUE(cube->addMetric("Температура")) << "New metric but not added";
	EXPECT_TRUE(cube->addMetric("Температура 123@@@_033--")) << "New metric but not added";
	EXPECT_TRUE(cube->addMetric(" 123@@@_033--")) << "New metric but not added";
	delete cube;
}

TEST(TestCube, AddingMetricEnglish) {
	Cube* cube = new Cube();
	EXPECT_TRUE(cube->addMetric("Temperature")) << "New metric but not added";
	EXPECT_TRUE(cube->addMetric("Temperature 123@@@_033--")) << "New metric but not added";
	EXPECT_TRUE(cube->addMetric(" 123@@@_033--")) << "New metric but not added";
	delete cube;
}

TEST(TestCube, AddingMetricEmpty) {
	Cube* cube = new Cube();
	EXPECT_TRUE(cube->addMetric("")) << "New metric but not added";
	EXPECT_FALSE(cube->addMetric("")) << "Added twice";
	delete cube;
}

TEST(TestCube, AddingFactUnique) {
	setlocale(LC_ALL, "Russian");
	Cube* cube = new Cube();
	cube->addDimension("Дата");
	cube->addDimension("Город");
	cube->addMetric("Температура");
	ASSERT_EQ(cube->addFact(Variant((long double)-21), "Температура", { "25 января","Зеленоград" }), static_cast<add_result>(1)) << "fact not added";
	EXPECT_EQ(cube->addFact(Variant((long double)14), "Температура", { "25 января","Зеленоград" }), static_cast<add_result>(0)) << "fact must be not added with true metric";
	EXPECT_EQ(cube->addFact(Variant((long double)0), "Температура", { "25 января","Зеленоград" }), static_cast<add_result>(0)) << "fact must be not added with true metric";
	EXPECT_EQ(cube->addFact(Variant((long double)-21.0), "Температура", { "25 января","Зеленоград" }), static_cast<add_result>(0)) << "fact must be not added with true metric";
	EXPECT_EQ(cube->addFact(Variant((long double)14.0), "Температура", { "25 января","Зеленоград" }), static_cast<add_result>(0)) << "fact must be not added with true metric";
	EXPECT_EQ(cube->addFact(Variant((long double)0.0), "Температура", { "25 января","Зеленоград" }), static_cast<add_result>(0)) << "fact must be not added with true metric";
	delete cube;
}

TEST(TestCube, AddingFactWrongMetric) {
	Cube* cube = new Cube();
	cube->addDimension("Дата");
	cube->addDimension("Город");
	cube->addMetric("Температура");
	EXPECT_EQ(cube->addFact(Variant((int64_t)21), "Cкорость ветра", { "25 января","Зеленоград" }), static_cast<add_result>(-1)) << "fact must be not added with wrong metric";
	EXPECT_EQ(cube->addFact(Variant((int64_t)21), "Температура", { "25 января","Зеленоград" }), static_cast<add_result>(1)) << "fact not added";
	EXPECT_EQ(cube->addFact(Variant((int64_t)21), "Cкорость ветра", { "25 января","Зеленоград" }), static_cast<add_result>(-1)) << "fact must be not added with wrong metric";
	delete cube;
}

TEST(TestCube, AddingFactTwoMetric) {
	Cube* cube = new Cube();
	cube->addDimension("Дата");
	cube->addDimension("Город");
	cube->addMetric("Температура");
	cube->addMetric("Влажность");
	EXPECT_EQ(cube->addFact(Variant((int64_t)21), "Температура", { "25 января","Зеленоград" }), static_cast<add_result>(1)) << "fact not added";
	EXPECT_EQ(cube->addFact(Variant((int64_t)21), "Влажность", { "25 января","Зеленоград" }), static_cast<add_result>(1)) << "fact not added";
	delete cube;
}

TEST(TestCube, CleanCubeEmpty) {
	Cube* cube = new Cube();
	EXPECT_NO_THROW(cube->clean());
	EXPECT_NO_THROW(delete cube);
}

TEST(TestSelection, Constructor) {
	Cube* cube = new Cube();
	ASSERT_NO_THROW(Selection* select = new Selection(cube)) << "Constructor causes error";
}

TEST(TestSelection, DestructorCubeSelection) {
	Cube* cube = new Cube();
	Selection* select = new Selection(cube);
	ASSERT_NO_THROW(delete cube) << "Destructor causes error";
	ASSERT_NO_THROW(delete select) << "Destructor causes error";
}

TEST(TestSelection, DestructorSelectionCube) {
	Cube* cube = new Cube();
	Selection* select = new Selection(cube);
	ASSERT_NO_THROW(delete select) << "Destructor causes error";
	ASSERT_NO_THROW(delete cube) << "Destructor causes error";
}

TEST(TestSelection, Found) {
	Cube* cube = new Cube();
	cube->addDimension("Город");
	cube->addDimension("Дата");
	cube->addMetric("Температура");
	cube->addMetric("Влажность");
	for (int city_c = 0; city_c < 5; city_c++) {
		for (int date_c = 0; date_c < 20; date_c++) {
			cube->addFact(Variant((int64_t)facts_temp[city_c][date_c]), "Температура", { cities[city_c],dates[date_c] });
			cube->addFact(Variant((int64_t)facts_hum[city_c][date_c]), "Влажность", { cities[city_c],dates[date_c] });
		}
	}
	Selection* select = new Selection(cube);
	ASSERT_EQ(select->make("Дата", { "15 мая","16 мая" }), static_cast<make_result>(1)) << "Must found something";
	ASSERT_EQ(select->make("Город", { "Санкт-Петербург" }), static_cast<make_result>(0)) << "Nothing must be found";
	delete cube;
	delete select;
}

TEST(TestSelection, NotFound) {
	Cube* cube = new Cube();
	cube->addDimension("Город");
	cube->addDimension("Дата");
	cube->addMetric("Температура");
	cube->addMetric("Влажность");
	for (int city_c = 0; city_c < 5; city_c++) {
		for (int date_c = 0; date_c < 20; date_c++) {
			cube->addFact(Variant((int64_t)facts_temp[city_c][date_c]), "Температура", { cities[city_c],dates[date_c] });
			cube->addFact(Variant((int64_t)facts_hum[city_c][date_c]), "Влажность", { cities[city_c],dates[date_c] });
		}
	}
	Selection* select = new Selection(cube);
	EXPECT_EQ(select->make("Дата", { "11 января" }), static_cast<make_result>(0)) << "Found something or call metric wrong";
	EXPECT_EQ(select->make("Дата", { "11 января" }), static_cast<make_result>(0)) << "Found something or call metric wrong";
	EXPECT_EQ(select->make("Дата", { "" }), static_cast<make_result>(0)) << "Found something or call metric wrong";
	delete cube;
	delete select;
}

TEST(TestSelection, WrongDimension) {
	Cube* cube = new Cube();
	cube->addDimension("Дата");
	cube->addDimension("Город");
	cube->addMetric("Температура");
	cube->addMetric("Влажность");
	for (int city_c = 0; city_c < 5; city_c++) {
		for (int date_c = 0; date_c < 20; date_c++) {
			cube->addFact(Variant((int64_t)facts_temp[city_c][date_c]), "Температура", { cities[city_c],dates[date_c] });
			cube->addFact(Variant((int64_t)facts_hum[city_c][date_c]), "Влажность", { cities[city_c],dates[date_c] });
		}
	}
	Selection* select = new Selection(cube);
	EXPECT_EQ(select->make("Метеослужба", { "15 мая","16 мая" }), static_cast<make_result>(-1)) << "Found something or call metric true";
	EXPECT_EQ(select->make("Метеослужба", { "15 мая","16 мая" }), static_cast<make_result>(-1)) << "Found something or call metric true";
	EXPECT_EQ(select->make("Метеослужба", { }), static_cast<make_result>(-1)) << "Found something or call metric true";
	delete cube;
	delete select;
}

TEST(TestSelection, CubeDeleted) {
	Cube* cube = new Cube();
	cube->addDimension("Дата");
	cube->addDimension("Город");
	cube->addMetric("Температура");
	cube->addMetric("Влажность");
	for (int city_c = 0; city_c < 5; city_c++) {
		for (int date_c = 0; date_c < 20; date_c++) {
			cube->addFact(Variant((int64_t)facts_temp[city_c][date_c]), "Температура", { cities[city_c],dates[date_c] });
			cube->addFact(Variant((int64_t)facts_hum[city_c][date_c]), "Влажность", { cities[city_c],dates[date_c] });
		}
	}
	Selection* select = new Selection(cube);
	delete cube;
	EXPECT_EQ(select->make("Метеослужба", { "15 мая","16 мая" }), static_cast<make_result>(-2)) << "Found something or call metric true";
	EXPECT_EQ(select->make("Метеослужба", { "15 мая","16 мая" }), static_cast<make_result>(-2)) << "Found something or call metric true";
	EXPECT_EQ(select->make("Метеослужба", { }), static_cast<make_result>(-2)) << "Found something or call metric true";
	delete select;
}

TEST(TestSelection, CubeDeletedWithMetric) {
	Cube* cube = new Cube();
	cube->addDimension("Дата");
	cube->addDimension("Город");
	cube->addMetric("Температура");
	cube->addMetric("Влажность");
	for (int city_c = 0; city_c < 5; city_c++) {
		for (int date_c = 0; date_c < 20; date_c++) {
			cube->addFact(Variant((int64_t)facts_temp[city_c][date_c]), "Температура", { cities[city_c],dates[date_c] });
			cube->addFact(Variant((int64_t)facts_hum[city_c][date_c]), "Влажность", { cities[city_c],dates[date_c] });
		}
	}
	Selection* select = new Selection(cube);
	delete cube;
	EXPECT_EQ(select->make("Метеослужба", { "15 мая","16 мая" }, { "Влажность" }), static_cast<make_result>(-2)) << "Found something or call metric true";
	EXPECT_EQ(select->make("Метеослужба", { "15 мая","16 мая" }, { "Влажность" }), static_cast<make_result>(-2)) << "Found something or call metric true";
	EXPECT_EQ(select->make("Метеослужба", { }, { "Влажность" }), static_cast<make_result>(-2)) << "Found something or call metric true";
	delete select;
}

TEST(TestSelection, FoundWithMetric) {
	Cube* cube = new Cube();
	cube->addDimension("Город");
	cube->addDimension("Дата");
	cube->addMetric("Температура");
	cube->addMetric("Влажность");
	for (int city_c = 0; city_c < 5; city_c++) {
		for (int date_c = 0; date_c < 20; date_c++) {
			cube->addFact(Variant((int64_t)facts_temp[city_c][date_c]), "Температура", { cities[city_c],dates[date_c] });
			cube->addFact(Variant((int64_t)facts_hum[city_c][date_c]), "Влажность", { cities[city_c],dates[date_c] });
		}
	}
	Selection* select = new Selection(cube);
	ASSERT_EQ(select->make("Дата", { "15 мая","16 мая" }, { "Влажность" }), static_cast<make_result>(1)) << "Must found something";
	ASSERT_EQ(select->make("Город", { "Санкт-Петербург" }, { "Влажность" }), static_cast<make_result>(1)) << "Must found something";
	delete cube;
	delete select;
}

TEST(TestSelection, NotFoundWithMetric) {
	Cube* cube = new Cube();
	cube->addDimension("Город");
	cube->addDimension("Дата");
	cube->addMetric("Температура");
	cube->addMetric("Влажность");
	for (int city_c = 0; city_c < 5; city_c++) {
		for (int date_c = 0; date_c < 20; date_c++) {
			cube->addFact(Variant((int64_t)facts_temp[city_c][date_c]), "Температура", { cities[city_c],dates[date_c] });
			cube->addFact(Variant((int64_t)facts_hum[city_c][date_c]), "Влажность", { cities[city_c],dates[date_c] });
		}
	}
	Selection* select = new Selection(cube);
	EXPECT_EQ(select->make("Дата", { "11 января" }, { "Влажность" }), static_cast<make_result>(0)) << "Found something or call metric wrong";
	EXPECT_EQ(select->make("Дата", { "11 января" }, { "Влажность" }), static_cast<make_result>(0)) << "Found something or call metric wrong";
	EXPECT_EQ(select->make("Дата", { "" }, { "Влажность" }), static_cast<make_result>(0)) << "Found something or call metric wrong";
	delete cube;
	delete select;
}

TEST(TestSelection, WrongDimensionWithMetric) {
	Cube* cube = new Cube();
	cube->addDimension("Дата");
	cube->addDimension("Город");
	cube->addMetric("Температура");
	cube->addMetric("Влажность");
	for (int city_c = 0; city_c < 5; city_c++) {
		for (int date_c = 0; date_c < 20; date_c++) {
			cube->addFact(Variant((int64_t)facts_temp[city_c][date_c]), "Температура", { cities[city_c],dates[date_c] });
			cube->addFact(Variant((int64_t)facts_hum[city_c][date_c]), "Влажность", { cities[city_c],dates[date_c] });
		}
	}
	Selection* select = new Selection(cube);
	EXPECT_EQ(select->make("Метеослужба", { "15 мая","16 мая" }, { "Влажность" }), static_cast<make_result>(-1)) << "Found something or call metric true";
	EXPECT_EQ(select->make("Метеослужба", { "15 мая","16 мая" }, { "Влажность" }), static_cast<make_result>(-1)) << "Found something or call metric true";
	EXPECT_EQ(select->make("Метеослужба", { }, { "Влажность" }), static_cast<make_result>(-1)) << "Found something or call metric true";
	delete cube;
	delete select;
}

TEST(TestAll, CleanCubeFull) {
	Cube* cube = new Cube();
	cube->addDimension("Дата");
	cube->addDimension("Город");
	cube->addMetric("Температура");
	cube->addMetric("Влажность");
	for (int city_c = 0; city_c < 5; city_c++) {
		for (int date_c = 0; date_c < 20; date_c++) {
			cube->addFact(Variant((int64_t)facts_temp[city_c][date_c]), "Температура", { cities[city_c],dates[date_c] });
			cube->addFact(Variant((int64_t)facts_hum[city_c][date_c]), "Влажность", { cities[city_c],dates[date_c] });
		}
	}
	Selection* select = new Selection(cube);
	select->make("Дата", { "15 мая","16 мая" });
	EXPECT_NO_THROW(cube->clean()) << "cleaning of full cube causes error";
	EXPECT_TRUE(cube->addDimension("Дата")) << "must be added after cleaning";
	EXPECT_TRUE(cube->addDimension("Город")) << "must be added after cleaning";
	EXPECT_TRUE(cube->addMetric("Температура")) << "must be added after cleaning";
	EXPECT_TRUE(cube->addMetric("Влажность")) << "must be added after cleaning";
	for (int city_c = 0; city_c < 5; city_c++) {
		for (int date_c = 0; date_c < 20; date_c++) {
			EXPECT_EQ(cube->addFact(Variant((int64_t)facts_temp[city_c][date_c]), "Температура", { cities[city_c],dates[date_c] }), static_cast<add_result>(1)) << "must be added after cleaning";;
			EXPECT_EQ(cube->addFact(Variant((int64_t)facts_hum[city_c][date_c]), "Влажность", { cities[city_c],dates[date_c] }), static_cast<add_result>(1)) << "must be added after cleaning";;
		}
	}
	delete cube;
	delete select;
}

TEST(TestSelection, Clean) {
	Cube* cube = new Cube();
	cube->addDimension("Дата");
	cube->addDimension("Город");
	cube->addMetric("Температура");
	cube->addMetric("Влажность");
	for (int city_c = 0; city_c < 5; city_c++) {
		for (int date_c = 0; date_c < 20; date_c++) {
			cube->addFact(Variant((int64_t)facts_temp[city_c][date_c]), "Температура", { cities[city_c],dates[date_c] });
			cube->addFact(Variant((int64_t)facts_hum[city_c][date_c]), "Влажность", { cities[city_c],dates[date_c] });
		}
	}
	Selection* select = new Selection(cube);
	select->make("Дата", { "15 мая","16 мая" });
	ASSERT_NO_THROW(select->clean()) << "cleaning of selection causes error";
	//ASSERT_NO_THROW(select->print()) << "printing of cleaned selection causes error";
	select->make("Метеослужба", { });
	ASSERT_NO_THROW(select->clean()) << "cleaning of empty selection causes error";
	//ASSERT_NO_THROW(select->print()) << "printing of cleaned selection causes error";
	delete cube;
	delete select;
}

TEST(TestTypeClasses, DATE) {
	Date date1(12, 12, 1999), date2(12, 12, 1999), date3(11, 12, 2034);
	EXPECT_EQ(date1.day(), 12) << "Wrong value";
	EXPECT_EQ(date1.month(), 12) << "Wrong value";
	EXPECT_EQ(date1.year(), 1999) << "Wrong value";
	EXPECT_EQ(date1, date2) << "Must be identical";
	EXPECT_NE(date1, date3) << "Must be different";
}

TEST(TestTypeClasses, TIME) {
	Time time1(23, 7, 41), time2(23, 7, 41), time3(11, 0, 12);
	EXPECT_EQ(time1.hours(), 23) << "Wrong value";
	EXPECT_EQ(time1.minutes(), 7) << "Wrong value";
	EXPECT_EQ(time1.seconds(), 41) << "Wrong value";
	EXPECT_EQ(time1, time2) << "Must be identical";
	EXPECT_NE(time1, time3) << "Must be different";
}

TEST(TestTypeClasses, DATETIME) {
	DateTime dt1{ Date(12, 12, 1999),Time(23, 7, 41) },
		dt2{ Date(12, 12, 1999),Time(23, 7, 41) },
		dt3{ Date(12, 12, 1999),Time(11, 0, 12) },
		dt4{ Date(11, 12, 2034),Time(11, 0, 12) };
	EXPECT_EQ(dt1.date, Date(12, 12, 1999)) << "Wrong value";
	EXPECT_EQ(dt1.time, Time(23, 7, 41)) << "Wrong value";
	EXPECT_EQ(dt1.date, dt2.date) << "Must be identical";
	EXPECT_EQ(dt1.time, dt2.time) << "Must be identical";
	EXPECT_EQ(dt1, dt2) << "Must be identical";
	EXPECT_EQ(dt1.date, dt3.date) << "Must be identical";
	EXPECT_NE(dt1.time, dt3.time) << "Must be different";
	EXPECT_NE(dt1, dt3) << "Must be different";
	EXPECT_NE(dt1, dt4) << "Must be different";
}

TEST(TestVariant, VOID) {
	Variant var1((void*)1560);
	EXPECT_EQ(*static_cast<int*>(var1.value<void*>()), 1560) << "Wrong value";
	EXPECT_EQ(var1.valueType(), var_type::VOID) << "Wrong value type";
}

TEST(TestVariant, VOID_and_VOID) {
	Variant var1((void*)1560), var2((void*)1560), var3((void*)-1700);
	EXPECT_EQ(*static_cast<int*>(var1.value<void*>()), *static_cast<int*>(var2.value<void*>())) << "Must be identical";
	EXPECT_EQ(var1.valueType(), var2.valueType()) << "Must be identical";
	EXPECT_NE(var1, var2) << "Must be different";
	EXPECT_NE(*static_cast<int*>(var1.value<void*>()), *static_cast<int*>(var3.value<void*>())) << "Must be different";
	EXPECT_EQ(var1.valueType(), var3.valueType()) << "Must be identical";
	EXPECT_NE(var1, var3) << "Must be different";
	var1 = var3;
	EXPECT_NE(var1, var3) << "Must be different";
}

TEST(TestVariant, INT64) {
	Variant var1((int64_t)1560);
	EXPECT_EQ(var1.value<int64_t>(),1560) << "Wrong value";
	EXPECT_EQ(var1.valueType(), var_type::INT64) << "Wrong value type";
}

TEST(TestVariant, INT64_and_INT64) {
	Variant var1((int64_t)1560), var2((int64_t)1560), var3((int64_t)-1900);
	EXPECT_EQ(var1.value<int64_t>(), var2.value<int64_t>()) << "Must be identical";
	EXPECT_EQ(var1.valueType(), var2.valueType()) << "Must be identical";
	EXPECT_EQ(var1, var2) << "Must be identical";
	EXPECT_NE(var1.value<int64_t>(), var3.value<int64_t>()) << "Must be different";
	EXPECT_EQ(var1.valueType(), var3.valueType()) << "Must be identical";
	EXPECT_NE(var1, var3) << "Must be different";
	var1 = var3;
	EXPECT_EQ(var1, var3) << "Must be identical";
}

TEST(TestVariant, UINT64) {
	Variant var((uint64_t)1560);
	EXPECT_EQ(var.value<uint64_t>(), 1560) << "Wrong value";
	EXPECT_EQ(var.valueType(), var_type::UINT64) << "Wrong value type";
}

TEST(TestVariant, UINT64_and_UINT64) {
	Variant var1((uint64_t)1560), var2((uint64_t)1560), var3((uint64_t)2300);
	EXPECT_EQ(var1.value<uint64_t>(), var2.value<uint64_t>()) << "Must be identical";
	EXPECT_EQ(var1.valueType(), var2.valueType()) << "Must be identical";
	EXPECT_EQ(var1, var2) << "Must be identical";
	EXPECT_NE(var1.value<uint64_t>(), var3.value<uint64_t>()) << "Must be different";
	EXPECT_EQ(var1.valueType(), var3.valueType()) << "Must be identical";
	EXPECT_NE(var1, var3) << "Must be different";
	var1 = var3;
	EXPECT_EQ(var1, var3) << "Must be identical";
}

TEST(TestVariant, Boolean) {
	Variant var(false);
	EXPECT_EQ(var.value<bool>(), false) << "Wrong value";
	EXPECT_EQ(var.valueType(), var_type::BOOL) << "Wrong value type";
}

TEST(TestVariant, Boolean_and_Boolean) {
	Variant var1(true), var2(true), var3(false);
	EXPECT_EQ(var1.value<bool>(), var2.value<bool>()) << "Must be identical";
	EXPECT_EQ(var1.valueType(), var2.valueType()) << "Must be identical";
	EXPECT_EQ(var1, var2) << "Must be identical";
	EXPECT_NE(var1.value<bool>(), var3.value<bool>()) << "Must be different";
	EXPECT_EQ(var1.valueType(), var3.valueType()) << "Must be identical";
	EXPECT_NE(var1, var3) << "Must be different";
	var1 = var3;
	EXPECT_EQ(var1, var3) << "Must be identical";
}

TEST(TestVariant, LongDouble) {
	Variant var((long double)344.55);
	EXPECT_EQ(var.value<long double>(), 344.55) << "Wrong value";
	EXPECT_EQ(var.valueType(), var_type::LDOUBLE) << "Wrong value type";
}

TEST(TestVariant, LongDouble_and_LongDouble) {
	Variant var1((long double)344.55), var2((long double)344.55), var3((long double)-217.1);
	EXPECT_EQ(var1.value<long double>(), var2.value<long double>()) << "Must be identical";
	EXPECT_EQ(var1.valueType(), var2.valueType()) << "Must be identical";
	EXPECT_EQ(var1, var2) << "Must be identical";
	EXPECT_NE(var1.value<long double>(), var3.value<long double>()) << "Must be different";
	EXPECT_EQ(var1.valueType(), var3.valueType()) << "Must be identical";
	EXPECT_NE(var1, var3) << "Must be different";
	var1 = var3;
	EXPECT_EQ(var1, var3) << "Must be identical";
}

TEST(TestVariant, Character) {
	Variant var('a');
	EXPECT_EQ(var.value<char>(), 'a') << "Wrong value";
	EXPECT_EQ(var.valueType(), var_type::CHAR) << "Wrong value type";
}

TEST(TestVariant, Character_and_Character) {
	Variant var1((char)'a'), var2((char)'a'), var3((char)'c');
	EXPECT_EQ(var1.value<char>(), var2.value<char>()) << "Must be identical";
	EXPECT_EQ(var1.valueType(), var2.valueType()) << "Must be identical";
	EXPECT_EQ(var1, var2) << "Must be identical";
	EXPECT_NE(var1.value<char>(), var3.value<char>()) << "Must be different";
	EXPECT_EQ(var1.valueType(), var3.valueType()) << "Must be identical";
	EXPECT_NE(var1, var3) << "Must be different";
	var1 = var3;
	EXPECT_EQ(var1, var3) << "Must be identical";
}

TEST(TestVariant, String) {
	Variant var((std::string)"String Строка");
	EXPECT_EQ(var.value<std::string>(), "String Строка") << "Wrong value";
	EXPECT_EQ(var.valueType(), var_type::STRING) << "Wrong value type";
}

TEST(TestVariant, String_and_String) {
	Variant var1((std::string)"String Строка"), var2((std::string)"String Строка"), var3((std::string)"123");
	EXPECT_EQ(var1.value<std::string>(), var2.value<std::string>()) << "Must be identical";
	EXPECT_EQ(var1.valueType(), var2.valueType()) << "Must be identical";
	EXPECT_EQ(var1, var2) << "Must be identical";
	EXPECT_NE(var1.value<std::string>(), var3.value<std::string>()) << "Must be different";
	EXPECT_EQ(var1.valueType(), var3.valueType()) << "Must be identical";
	EXPECT_NE(var1, var3) << "Must be different";
	var1 = var3;
	EXPECT_EQ(var1, var3) << "Must be identical";
}

TEST(TestVariant, WideString) {
	Variant var((std::wstring)L"String Строка");
	EXPECT_EQ(var.value<std::wstring>(), L"String Строка") << "Wrong value";
	EXPECT_EQ(var.valueType(), var_type::WSTRING) << "Wrong value type";
}

TEST(TestVariant, WideString_and_WideString) {
	Variant var1((std::wstring)L"String Строка"), var2((std::wstring)L"String Строка"), var3((std::wstring)L"123");
	EXPECT_EQ(var1.value<std::wstring>(), var2.value<std::wstring>()) << "Must be identical";
	EXPECT_EQ(var1.valueType(), var2.valueType()) << "Must be identical";
	EXPECT_EQ(var1, var2) << "Must be identical";
	EXPECT_NE(var1.value<std::wstring>(), var3.value<std::wstring>()) << "Must be different";
	EXPECT_EQ(var1.valueType(), var3.valueType()) << "Must be identical";
	EXPECT_NE(var1, var3) << "Must be different";
	var1 = var3;
	EXPECT_EQ(var1, var3) << "Must be identical";
}

TEST(TestVariant, TIME) {
	Variant var(Time(23,46,00));
	EXPECT_EQ(var.value<Time>(), Time(23, 46, 00)) << "Wrong value";
	EXPECT_EQ(var.valueType(), var_type::TIME) << "Wrong value type";
}

TEST(TestVariant, TIME_and_TIME) {
	Variant var1(Time(23, 46, 00)), var2(Time(23, 46, 00)), var3(Time(8, 23, 14));
	EXPECT_EQ(var1.value<Time>(), var2.value<Time>()) << "Must be identical";
	EXPECT_EQ(var1.valueType(), var2.valueType()) << "Must be identical";
	EXPECT_EQ(var1, var2) << "Must be identical";
	EXPECT_NE(var1.value<Time>(), var3.value<Time>()) << "Must be different";
	EXPECT_EQ(var1.valueType(), var3.valueType()) << "Must be identical";
	EXPECT_NE(var1, var3) << "Must be different";
	var1 = var3;
	EXPECT_EQ(var1, var3) << "Must be identical";
}

TEST(TestVariant, DATE) {
	Variant var(Date(23, 2, 2000));
	EXPECT_EQ(var.value<Date>(), Date(23, 2, 2000)) << "Wrong value";
	EXPECT_EQ(var.valueType(), var_type::DATE) << "Wrong value type";
}

TEST(TestVariant, DATE_and_DATE) {
	Variant var1(Date(23, 2, 2000)), var2(Date(23, 2, 2000)), var3(Date(14, 5, 1986));
	EXPECT_EQ(var1.value<Date>(), var2.value<Date>()) << "Must be identical";
	EXPECT_EQ(var1.valueType(), var2.valueType()) << "Must be identical";
	EXPECT_EQ(var1, var2) << "Must be identical";
	EXPECT_NE(var1.value<Date>(), var3.value<Date>()) << "Must be different";
	EXPECT_EQ(var1.valueType(), var3.valueType()) << "Must be identical";
	EXPECT_NE(var1, var3) << "Must be different";
	var1 = var3;
	EXPECT_EQ(var1, var3) << "Must be identical";
}

TEST(TestVariant, DATETIME) {
	Variant var(DateTime{Date(23, 2, 2000), Time(23, 46, 00) });
	DateTime v = {Date(23, 2, 2000),Time(23, 46, 00)};
	EXPECT_TRUE(var.value<DateTime>() == v) << "Wrong value";
	EXPECT_EQ(var.valueType(), var_type::DATETIME) << "Wrong value type";
}

TEST(TestVariant, DATETIME_and_DATETIME) {
	Variant var1(DateTime{ Date(23, 2, 2000), Time(23, 46, 00) }),
		var2(DateTime{ Date(23, 2, 2000), Time(23, 46, 00) }), 
		var3(DateTime{ Date(14, 5, 1986), Time(8, 23, 14) });
	EXPECT_EQ(var1.value<DateTime>(), var2.value<DateTime>()) << "Must be identical";
	EXPECT_EQ(var1.valueType(), var2.valueType()) << "Must be identical";
	EXPECT_EQ(var1, var2) << "Must be identical";
	EXPECT_NE(var1.value<DateTime>(), var3.value<DateTime>()) << "Must be different";
	EXPECT_EQ(var1.valueType(), var3.valueType()) << "Must be identical";
	EXPECT_NE(var1, var3) << "Must be different";
	var1 = var3;
	EXPECT_EQ(var1, var3) << "Must be identical";
}

TEST(TestVariant, DifferentTypes) {
	Variant var1(DateTime{ Date(23, 2, 2000), Time(23, 46, 00) }),
		var2((std::string)"123");
	Variant var3(Time(23, 46, 00));
	EXPECT_NE(var1, var2) << "Must be different";
	ASSERT_NO_THROW(var1 = var2);
	EXPECT_EQ(var1, var2) << "Must be identical";
	ASSERT_NO_THROW(var1.value<std::string>());
	EXPECT_EQ(var1.value<std::string>(), var2.value<std::string>()) << "Must be identical";
	EXPECT_EQ(var1.value<std::string>(), "123") << "Wrong value";
	EXPECT_EQ(var1.valueType(), var2.valueType()) << "Must be identical";
	EXPECT_EQ(var1.valueType(), var_type::STRING) << "Wrong value type";
}