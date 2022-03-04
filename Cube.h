#pragma once
#include <string>
#include <vector>
#include <map>
#include <unordered_map>

/**
 * @brief Результат добавления факта.
 */
enum class add_result {
	UNKNOWN_MEASURE = -1, // неизвестная метрика
	ALREADY_EXIST, // факт уже существует
	ADDED // факт добавлен
};

class Dimension;
class Fact;
class Measure;
class DataPoint;
class Selection;

/**
 * @brief Куб.
 * 
 * Класс для добавления и хранения данных.
 */
class Cube{
public:

	friend Selection;

	Cube();

	// Добавление измерения
	bool add_Dimension(const std::string& a_dim_name);

	// Добавление метрики
	bool add_Measure(const std::string& a_measure_name);

	// Добавление факта
	add_result add_Fact(double a_value, const std::string& a_measure_name, const std::vector<std::string>& a_positions_list);

    // Очистка куба
	void clean();
	
	~Cube();

private:

	// Поиск метрики
	std::vector<Measure*>::const_iterator find_measure(const std::string& a_measure_name);

	// Очистка вектора указателей
	template <class T>
	void clean_vector(std::vector<T*>& a_vector);
	// Контейнеры фактов/измерений/метрик/точек данных куба
	std::multimap<std::pair<std::string, std::vector<std::string>>, Fact*> m_facts;
	std::vector<Dimension*> m_dims;
	std::vector<Measure*> m_measures;
	std::vector<DataPoint*> m_points;
	// Связанная выборка
	Selection* m_selection;
};