#pragma once
#include <vector>
#include <string>

/**
 * @brief Тип аггрегации
 */
enum class agg_type {
	COUNT, // Количество
	SUM, // Сумма
	AVERAGE // Среднее
};

/**
 * @brief Результат составления Выборки
 */
enum class make_result {
	CUBE_DELETED = -2, // Куб удален
	UNKNOWN_DIMENSION, // Неизвестное Измерение
	NO_FACT_FOUND, // Факты не найдены
	CREATED // Выборка создана
};

class DataPoint;
class Dimension;
class Cube;
class Fact;
class Measure;

/**
 * @brief Выборка
 * 
 * Класс для составления и агрегирования выборок на основе данных из куба
 */
class Selection {
public:

	Selection(Cube* a_cube);

	// Создание выборки
	make_result make(const std::string& a_dim_name, const std::vector<std::string>& a_positions_list, const std::vector<std::string>& a_measure_list = {});

	// Выбор агрегации
	bool aggregation(agg_type a_agg_type,const std::string& a_dimension_name, const std::vector<std::string>& a_measure_list = {});

	// Вывод Выборки
	void print() const;

	// Очистка Выборки
	void clean();

	~Selection();

private:

	// Агрегация - суммирование
	void count(const std::string& a_dimension_name, const std::vector<std::string>& a_measure_list);
	// Агрегация - количество
	void sum(const std::string& a_dimension_name, const std::vector<std::string>& a_measure_list);
	// Агрегация - среднее значение
	void average(const std::string& a_dimension_name, const std::vector<std::string>& a_measure_list);

	// Те ТочкиДанных Куба, из которых состоит выборка
	std::vector<DataPoint*> m_selection_points;
	// ТочкиДанных/Факты/Метрики, созданные на основе агрегации
	std::vector<DataPoint*> m_aggregation_points;
	std::vector<Fact*> m_aggregation_facts;
	std::vector<Measure*> m_aggregation_measures;
	// Измерение для агрегаций
	Dimension* m_aggregation_dim;
	// Связанный Куб для создания Выборки
	Cube* m_cube;
};