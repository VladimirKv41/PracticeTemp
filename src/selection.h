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

struct hash_pair {
	template <class T1, class T2>
	size_t operator()(const std::pair<T1, T2>& p) const
	{
		auto hash1 = std::hash<T1>{}(p.first);
		auto hash2 = std::hash<T2>{}(p.second);
		return hash1 ^ hash2;
	}
};
// Тип данных Выборки из Классификаторов
// Ключ: пара <Название Измерения, Название Отметок на Измерении>,
// Значение: Ключ: название Метрики, Значение: указатель на Классификатор Факта
typedef std::unordered_map<std::pair<std::string, std::string>, std::unordered_multimap<std::string, FactClassifier*>, hash_pair> dpoint_ummaps_map;

/**
 * @brief Выборка.
 *
 * Класс для составления и агрегирования Выборок на основе данных из Куба.
 */
class Selection {
public:

	Selection(Cube* a_cube);

	// Создание Выборки
	make_result make(const std::string& a_dim_name, const std::vector<std::string>& a_marks_list, const std::set<std::string>& a_metric_list = {});

	// Очистка Выборки
	void clean(bool cube_cleaning = false);

	~Selection();

private:

	// Поиск Измерения
	std::vector<Dimension*>::const_iterator findDimension(const std::string& a_dimension_name) const;

	// Заполнение Выборки (для изначально пустой Выборки)
	void making
	(
		const std::map<std::string, DimensionMark*>& a_map_from,
		const std::string& a_dim_name, const std::vector<std::string>& a_marks_list,
		const std::set<std::string>& a_metric_list
	);

	// Заполнение Выборки (для уже заполненной Выборки)
	void making
	(
		const dpoint_ummaps_map& a_map_from,
		const std::string& a_dim_name,
		const std::vector<std::string>& a_marks_list,
		const std::set<std::string>& a_metric_list
	);

	// Получение Классификаторов Фактов с указанными Метриками
	void FactClassifiersByMetric
	(
		std::unordered_multimap<std::string, FactClassifier*>& a_umap_to,
		const std::unordered_multimap<std::string, FactClassifier*>& a_umap_from,
		const std::set<std::string>& a_metric_list
	);

	// Добавление оставшихся Классификаторов Фактов
	void addRestFactClassifiers(dpoint_ummaps_map& a_map, const std::string& a_dim_name, const std::string& a_dim_mark);


	// Те Классификаторы Фактов Куба, из которых состоит Выборка
	dpoint_ummaps_map m_selection_points;
	// Метрики, созданные на основе агрегации
	std::vector<Metric*> m_aggregation_metrics;
	// Измерение для агрегаций
	Dimension* m_aggregation_dim;
	// Связанный Куб для создания Выборки
	Cube* m_cube;
};

#endif