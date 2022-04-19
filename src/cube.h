#ifndef CUBE_H
#define CUBE_H
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
class Metric;
class FactClassifier;
class Selection;

/**
 * @brief Куб.
 *
 * Класс для добавления и хранения данных.
 */
class Cube {
public:

	friend Selection;

	Cube();

	// Добавление Измерения
	bool addDimension(const std::string& a_dim_name);

	// Добавление Метрики
	bool addMetric(const std::string& a_metric_name);

	// Добавление Факта
	add_result addFact(double a_value, const std::string& a_metric_name, const std::vector<std::string>& a_marks_list);

	// Очистка Куба
	void clean();

	~Cube();

private:

	// Поиск Метрики
	std::vector<Metric*>::const_iterator findMetric(const std::string& a_metric_name);

	// Очистка вектора указателей
	template <class T>
	void cleanVector(std::vector<T*>& a_vector);

	// Контейнеры Фактов/Измерений/Метрик/Классификаторов Фактов Куба
	std::multimap<std::pair<std::string, std::vector<std::string>>, Fact*> m_facts;
	std::vector<Dimension*> m_dims;
	std::vector<Metric*> m_metrics;
	std::vector<FactClassifier*> m_classifiers;
	// Связанная Выборка
	Selection* m_selection = nullptr;
};

#endif