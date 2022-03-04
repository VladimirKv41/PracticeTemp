#pragma once
#include <string>
#include <vector>

/**
 * @brief Результат добавления Факта
 */
enum class add_result {
	UNKNOWN_MEASURE = -1, // Неизвестная Метрика
	ALREADY_EXIST, // Факт уже существует
	ADDED // Факт добавлен
};

class Dimension;
class Fact;
class Measure;
class DataPoint;
class Selection;

/**
 * @brief Куб
 * 
 * Класс для добавления и хранения данных
 */
class Cube{
public:

	friend Selection;

	Cube();

	// добавление Измерения
	bool add_Dimension(const std::string& a_dim_name);

	// добавление Метрика
	bool add_Measure(const std::string& a_measure_name);

	// Добавление Факта
	add_result add_Fact(double a_value, const std::string& a_measure, const std::vector<std::string>& a_positions_list);

    // Очистка Куба
	void clean();
	
	~Cube();

private:

	// Очистка вектора указателей
	template <class T>
	void clean_vector(std::vector<T*>& a_vector);

	// Вектора Фактов/Измерений/Метрик/ТочекДанных Куба
	std::vector<Fact*> m_facts;
	std::vector<Dimension*> m_dims;
	std::vector<Measure*> m_measures;
	std::vector<DataPoint*> m_points;
	// Связанная Выборка
	Selection* m_selection;
};