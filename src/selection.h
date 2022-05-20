#ifndef SELECTION_H
#define SELECTION_H
#include <vector>
#include <string>
#include <map>
#include <unordered_map>
#include <set>

/**
 * @brief Тип агрегации.
 */
enum class agg_type {
	COUNT, // Количество
	SUM, // Сумма
	AVERAGE // Среднее
};

/**
 * @brief Результат составления Выборки.
 */
enum class make_result {
	CUBE_NOT_FOUND = -2, // Куб удален
	UNKNOWN_DIMENSION, // Неизвестное Измерение
	NO_FACT_FOUND, // Факты не найдены
	CREATED // Выборка создана
};

class FactClassifier;
class Dimension;
class DimensionMark;
class Cube;
class Fact;
class Metric;

/**
 * @brief Выборка.
 *
 * Класс для составления и агрегирования Выборок на основе данных из Куба.
 */
class Selection {
public:

	Selection(Cube* a_cube);

	// Создание Выборки
	make_result make(const std::string& a_dim_name, const std::initializer_list<std::string>& a_marks_list, const std::set<std::string>& a_metric_list = {});
	
	// Выбор агрегации
	bool aggregation(agg_type a_agg_type, const std::string& a_dimension_name, const std::vector<std::string>& a_measure_list = {});
	
	// Вывод Выборки
	void print() const;

	// Очистка Выборки
	void clean(bool cube_cleaning = false);

	~Selection();

private:

	// Поиск Измерения
	std::vector<Dimension*>::const_iterator findDimension(const std::string& a_dimension_name) const;

	// Заполнение Выборки (для изначально пустой Выборки)
	void making
	(
		const Dimension* a_dimension,
		const std::initializer_list<std::string>& a_marks_list,
		const std::set<std::string>& a_metric_list
	);
	
	// Заполнение Выборки (для уже заполненной Выборки)
	void making
	(
		const std::string& a_dim_name,
		const std::initializer_list<std::string>& a_marks_list,
		const std::set<std::string>& a_metric_list
	);

	// Добавление Классификаторов c указанными Отметками
	void addFactClassifiers
	(
		const FactClassifier* a_dimension,
		const std::initializer_list<std::string>& a_marks_list
	);

	// Агрегация - суммирование
	void count(const std::string& a_dimension_name, const std::vector<std::string>& a_measure_list);
	// Агрегация - количество
	void sum(const std::string& a_dimension_name, const std::vector<std::string>& a_measure_list);
	// Агрегация - среднее значение
	void average(const std::string& a_dimension_name, const std::vector<std::string>& a_measure_list);

	// Те Классификаторы Фактов Куба, из которых состоит Выборка
	std::vector<FactClassifier*> m_selection_classifiers;
	// Метрики, Факты и Классификаторы, созданные на основе агрегации
	std::vector<FactClassifier*> m_aggregation_classifiers;
	std::vector<Metric*> m_aggregation_metrics;
	std::vector<Fact*> m_aggregation_facts;
	// Измерение для агрегаций
	Dimension* m_aggregation_dim;
	// Связанный Куб для создания Выборки
	Cube* m_cube;
};

#endif