#ifndef CUBE_H
#define CUBE_H
#include <string>
#include <vector>
#include <unordered_set>
#include <map>
#include <unordered_map>


/**
 * @brief Результат добавления Факта.
 */
enum class add_result {
	UNKNOWN_MEASURE = -1, // неизвестная Метрика
	ALREADY_EXIST, // Факт уже существует
	ADDED // Факт добавлен
};

class Dimension;
class Fact;
class Metric;
class FactClassifier;
class Selection;
class Variant;

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
	const add_result& addFact(const Variant& a_value, const std::string& a_metric, std::initializer_list<std::string> a__marks);


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
	std::map<std::pair<std::string, std::vector<std::string>>, Fact*> m_facts;
	std::vector<Dimension*> m_dims;
	std::vector<Metric*> m_metrics;
	std::vector<FactClassifier*> m_classifiers;
	// Связанная Выборка
	Selection* m_selection = nullptr;
};

#endif