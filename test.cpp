#include "gtest/gtest.h"
#include "cube.h"
#include "selection.h"
#include "variant.h"
#include "time.h"
#include "date.h"
#include "datetime.h"

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
	EXPECT_TRUE(cube->add_Dimension("Время")) << "New dimension but not added";
	EXPECT_TRUE(cube->add_Dimension("Время123@@@_033--")) << "New dimension but not added";
	EXPECT_TRUE(cube->add_Dimension("123@@@_033--")) << "New dimension but not added";
	delete cube;
}

TEST(TestCube, AddingDimensionEnglish) {
	Cube* cube = new Cube();
	EXPECT_TRUE(cube->add_Dimension("Time")) << "New dimension but not added";
	EXPECT_TRUE(cube->add_Dimension("Time 123@@@_033--")) << "New dimension but not added";
	EXPECT_TRUE(cube->add_Dimension(" 123@@@_033--")) << "New dimension but not added";
	delete cube;
}

TEST(TestCube, AddingDimensionEmpty) {
	Cube* cube = new Cube();
	EXPECT_TRUE(cube->add_Dimension("")) << "New dimension but not added";
	EXPECT_FALSE(cube->add_Dimension("")) << "Added twice";
	delete cube;
}

TEST(TestCube, AddingMetricRussian) {
	Cube* cube = new Cube();
	EXPECT_TRUE(cube->add_Metric("Температура")) << "New metric but not added";
	EXPECT_TRUE(cube->add_Metric("Температура 123@@@_033--")) << "New metric but not added";
	EXPECT_TRUE(cube->add_Metric(" 123@@@_033--")) << "New metric but not added";
	delete cube;
}

TEST(TestCube, AddingMetricEnglish) {
	Cube* cube = new Cube();
	EXPECT_TRUE(cube->add_Metric("Temperature")) << "New metric but not added";
	EXPECT_TRUE(cube->add_Metric("Temperature 123@@@_033--")) << "New metric but not added";
	EXPECT_TRUE(cube->add_Metric(" 123@@@_033--")) << "New metric but not added";
	delete cube;
}

TEST(TestCube, AddingMetricEmpty) {
	Cube* cube = new Cube();
	EXPECT_TRUE(cube->add_Metric("")) << "New metric but not added";
	EXPECT_FALSE(cube->add_Metric("")) << "Added twice";
	delete cube;
}

TEST(TestCube, AddingFactUnique) {
	setlocale(LC_ALL, "Russian");
	Cube* cube = new Cube();
	cube->add_Dimension("Дата");
	cube->add_Dimension("Город");
	cube->add_Metric("Температура");
	ASSERT_EQ(cube->add_Fact(-21, "Температура", { "25 января","Зеленоград" }), static_cast<add_result>(1)) << "fact not added";
	EXPECT_EQ(cube->add_Fact(14, "Температура", { "25 января","Зеленоград" }), static_cast<add_result>(0)) << "fact must be not added with true metric";
	EXPECT_EQ(cube->add_Fact(0, "Температура", { "25 января","Зеленоград" }), static_cast<add_result>(0)) << "fact must be not added with true metric";
	EXPECT_EQ(cube->add_Fact(-21.0, "Температура", { "25 января","Зеленоград" }), static_cast<add_result>(0)) << "fact must be not added with true metric";
	EXPECT_EQ(cube->add_Fact(14.0, "Температура", { "25 января","Зеленоград" }), static_cast<add_result>(0)) << "fact must be not added with true metric";
	EXPECT_EQ(cube->add_Fact(0.0, "Температура", { "25 января","Зеленоград" }), static_cast<add_result>(0)) << "fact must be not added with true metric";
	delete cube;
}

TEST(TestCube, AddingFactWrongMetric) {
	Cube* cube = new Cube();
	cube->add_Dimension("Дата");
	cube->add_Dimension("Город");
	cube->add_Metric("Температура");
	EXPECT_EQ(cube->add_Fact(21, "Cкорость ветра", { "25 января","Зеленоград" }), static_cast<add_result>(-1)) << "fact must be not added with wrong metric";
	EXPECT_EQ(cube->add_Fact(21, "Температура", { "25 января","Зеленоград" }), static_cast<add_result>(1)) << "fact not added";
	EXPECT_EQ(cube->add_Fact(21, "Cкорость ветра", { "25 января","Зеленоград" }), static_cast<add_result>(-1)) << "fact must be not added with wrong metric";
	delete cube;
}

TEST(TestCube, AddingFactTwoMetric) {
	Cube* cube = new Cube();
	cube->add_Dimension("Дата");
	cube->add_Dimension("Город");
	cube->add_Metric("Температура");
	cube->add_Metric("Влажность");
	EXPECT_EQ(cube->add_Fact(21, "Температура", { "25 января","Зеленоград" }), static_cast<add_result>(1)) << "fact not added";
	EXPECT_EQ(cube->add_Fact(21, "Влажность", { "25 января","Зеленоград" }), static_cast<add_result>(1)) << "fact not added";
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
	cube->add_Dimension("Город");
	cube->add_Dimension("Дата");
	cube->add_Metric("Температура");
	cube->add_Metric("Влажность");
	for (int city_c = 0; city_c < 5; city_c++) {
		for (int date_c = 0; date_c < 20; date_c++) {
			cube->add_Fact(facts_temp[city_c][date_c], "Температура", { cities[city_c],dates[date_c] });
			cube->add_Fact(facts_hum[city_c][date_c], "Влажность", { cities[city_c],dates[date_c] });
		}
	}
	Selection* select = new Selection(cube);
	ASSERT_EQ(select->make("Дата", { "15 мая","16 мая" }), static_cast<make_result>(1)) << "Must found something";
	ASSERT_EQ(select->make("Город", { "Санкт-Петербург" }), static_cast<make_result>(1)) << "Must found something";
	delete cube;
	delete select;
}

TEST(TestSelection, NotFound) {
	Cube* cube = new Cube();
	cube->add_Dimension("Город");
	cube->add_Dimension("Дата");
	cube->add_Metric("Температура");
	cube->add_Metric("Влажность");
	for (int city_c = 0; city_c < 5; city_c++) {
		for (int date_c = 0; date_c < 20; date_c++) {
			cube->add_Fact(facts_temp[city_c][date_c], "Температура", { cities[city_c],dates[date_c] });
			cube->add_Fact(facts_hum[city_c][date_c], "Влажность", { cities[city_c],dates[date_c] });
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
	cube->add_Dimension("Дата");
	cube->add_Dimension("Город");
	cube->add_Metric("Температура");
	cube->add_Metric("Влажность");
	for (int city_c = 0; city_c < 5; city_c++) {
		for (int date_c = 0; date_c < 20; date_c++) {
			cube->add_Fact(facts_temp[city_c][date_c], "Температура", { cities[city_c],dates[date_c] });
			cube->add_Fact(facts_hum[city_c][date_c], "Влажность", { cities[city_c],dates[date_c] });
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
	cube->add_Dimension("Дата");
	cube->add_Dimension("Город");
	cube->add_Metric("Температура");
	cube->add_Metric("Влажность");
	for (int city_c = 0; city_c < 5; city_c++) {
		for (int date_c = 0; date_c < 20; date_c++) {
			cube->add_Fact(facts_temp[city_c][date_c], "Температура", { cities[city_c],dates[date_c] });
			cube->add_Fact(facts_hum[city_c][date_c], "Влажность", { cities[city_c],dates[date_c] });
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
	cube->add_Dimension("Дата");
	cube->add_Dimension("Город");
	cube->add_Metric("Температура");
	cube->add_Metric("Влажность");
	for (int city_c = 0; city_c < 5; city_c++) {
		for (int date_c = 0; date_c < 20; date_c++) {
			cube->add_Fact(facts_temp[city_c][date_c], "Температура", { cities[city_c],dates[date_c] });
			cube->add_Fact(facts_hum[city_c][date_c], "Влажность", { cities[city_c],dates[date_c] });
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
	cube->add_Dimension("Город");
	cube->add_Dimension("Дата");
	cube->add_Metric("Температура");
	cube->add_Metric("Влажность");
	for (int city_c = 0; city_c < 5; city_c++) {
		for (int date_c = 0; date_c < 20; date_c++) {
			cube->add_Fact(facts_temp[city_c][date_c], "Температура", { cities[city_c],dates[date_c] });
			cube->add_Fact(facts_hum[city_c][date_c], "Влажность", { cities[city_c],dates[date_c] });
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
	cube->add_Dimension("Город");
	cube->add_Dimension("Дата");
	cube->add_Metric("Температура");
	cube->add_Metric("Влажность");
	for (int city_c = 0; city_c < 5; city_c++) {
		for (int date_c = 0; date_c < 20; date_c++) {
			cube->add_Fact(facts_temp[city_c][date_c], "Температура", { cities[city_c],dates[date_c] });
			cube->add_Fact(facts_hum[city_c][date_c], "Влажность", { cities[city_c],dates[date_c] });
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
	cube->add_Dimension("Дата");
	cube->add_Dimension("Город");
	cube->add_Metric("Температура");
	cube->add_Metric("Влажность");
	for (int city_c = 0; city_c < 5; city_c++) {
		for (int date_c = 0; date_c < 20; date_c++) {
			cube->add_Fact(facts_temp[city_c][date_c], "Температура", { cities[city_c],dates[date_c] });
			cube->add_Fact(facts_hum[city_c][date_c], "Влажность", { cities[city_c],dates[date_c] });
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
	cube->add_Dimension("Дата");
	cube->add_Dimension("Город");
	cube->add_Metric("Температура");
	cube->add_Metric("Влажность");
	for (int city_c = 0; city_c < 5; city_c++) {
		for (int date_c = 0; date_c < 20; date_c++) {
			cube->add_Fact(facts_temp[city_c][date_c], "Температура", { cities[city_c],dates[date_c] });
			cube->add_Fact(facts_hum[city_c][date_c], "Влажность", { cities[city_c],dates[date_c] });
		}
	}
	Selection* select = new Selection(cube);
	select->make("Дата", { "15 мая","16 мая" });
	EXPECT_NO_THROW(cube->clean()) << "cleaning of full cube causes error";
	EXPECT_TRUE(cube->add_Dimension("Дата")) << "must be added after cleaning";
	EXPECT_TRUE(cube->add_Dimension("Город")) << "must be added after cleaning";
	EXPECT_TRUE(cube->add_Metric("Температура")) << "must be added after cleaning";
	EXPECT_TRUE(cube->add_Metric("Влажность")) << "must be added after cleaning";
	for (int city_c = 0; city_c < 5; city_c++) {
		for (int date_c = 0; date_c < 20; date_c++) {
			EXPECT_EQ(cube->add_Fact(facts_temp[city_c][date_c], "Температура", { cities[city_c],dates[date_c] }),static_cast<add_result>(1)) << "must be added after cleaning";;
			EXPECT_EQ(cube->add_Fact(facts_hum[city_c][date_c], "Влажность", { cities[city_c],dates[date_c] }), static_cast<add_result>(1)) << "must be added after cleaning";;
		}
	}
	delete cube;
	delete select;
}

TEST(TestSelection, Clean) {
	Cube* cube = new Cube();
	cube->add_Dimension("Дата");
	cube->add_Dimension("Город");
	cube->add_Metric("Температура");
	cube->add_Metric("Влажность");
	for (int city_c = 0; city_c < 5; city_c++) {
		for (int date_c = 0; date_c < 20; date_c++) {
			cube->add_Fact(facts_temp[city_c][date_c], "Температура", { cities[city_c],dates[date_c] });
			cube->add_Fact(facts_hum[city_c][date_c], "Влажность", { cities[city_c],dates[date_c] });
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

TEST(TestVariant, INT64) {
	Variant var((int64_t)1560);
	EXPECT_EQ(var.getvalue<int64_t>(),1560) << "Wrong value";
	EXPECT_EQ(var.getvaluetype(), var_type::INT64) << "Wrong value type";
}

TEST(TestVariant, UINT64) {
	Variant var((uint64_t)1560);
	EXPECT_EQ(var.getvalue<uint64_t>(), 1560) << "Wrong value";
	EXPECT_EQ(var.getvaluetype(), var_type::UINT64) << "Wrong value type";
}

TEST(TestVariant, Boolean) {
	Variant var(false);
	EXPECT_EQ(var.getvalue<bool>(), false) << "Wrong value";
	EXPECT_EQ(var.getvaluetype(), var_type::BOOL) << "Wrong value type";
}

TEST(TestVariant, LongDouble) {
	Variant var((long double)344.55);
	EXPECT_EQ(var.getvalue<long double>(), 344.55) << "Wrong value";
	EXPECT_EQ(var.getvaluetype(), var_type::LDOUBLE) << "Wrong value type";
}

TEST(TestVariant, Character) {
	Variant var('a');
	EXPECT_EQ(var.getvalue<char>(), 'a') << "Wrong value";
	EXPECT_EQ(var.getvaluetype(), var_type::CHAR) << "Wrong value type";
}

TEST(TestVariant, String) {
	Variant var((std::string)"String Строка");
	EXPECT_EQ(var.getvalue<std::string>(), "String Строка") << "Wrong value";
	EXPECT_EQ(var.getvaluetype(), var_type::STRING) << "Wrong value type";
}

TEST(TestVariant, WideString) {
	Variant var((std::wstring)L"String Строка");
	EXPECT_EQ(var.getvalue<std::wstring>(), L"String Строка") << "Wrong value";
	EXPECT_EQ(var.getvaluetype(), var_type::WSTRING) << "Wrong value type";
}

TEST(TestVariant, TIME) {
	Variant var(Time(23,46,00));
	EXPECT_EQ(var.getvalue<Time>(), Time(23, 46, 00)) << "Wrong value";
	EXPECT_EQ(var.getvaluetype(), var_type::TIME) << "Wrong value type";
}

TEST(TestVariant, DATE) {
	Variant var(Date(23, 2, 2000));
	EXPECT_EQ(var.getvalue<Date>(), Date(23, 2, 2000)) << "Wrong value";
	EXPECT_EQ(var.getvaluetype(), var_type::DATE) << "Wrong value type";
}

TEST(TestVariant, DATETIME) {
	Variant var(DateTime{Date(23, 2, 2000), Time(23, 46, 00) });
	DateTime v = {Date(23, 2, 2000),Time(23, 46, 00)};
	EXPECT_TRUE(var.getvalue<DateTime>() == v) << "Wrong value";
	EXPECT_EQ(var.getvaluetype(), var_type::DATETIME) << "Wrong value type";
}

TEST(StressTest, MakeTimeFor_3650000_Elements) {
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
	std::vector<std::string> metrics = { "Цена", "Масса", "Расход" };
	srand(time(NULL));
	setlocale(LC_ALL, "Russian");
	Cube* cube = new Cube();
	cube->add_Dimension(dimensions[2]);
	cube->add_Dimension(dimensions[1]);
	cube->add_Dimension(dimensions[0]);
	cube->add_Metric(metrics[2]);
	cube->add_Metric(metrics[1]);
	cube->add_Metric(metrics[0]);
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
	std::chrono::microseconds elapsed_mks = std::chrono::microseconds::zero(), elapsed_mks_offset = std::chrono::microseconds::zero();
	for (int j = 1; j < 3; j++) {
		unsigned long long tens = pow(2, j);
		unsigned long long min = 500000000000, max = 0;
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
	SUCCEED();
}