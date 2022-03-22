#pragma once
#include <vector>
#include <string>
#include <map>
#include <unordered_map>
#include <set>

/**
 * @brief Тип аггрегации.
 */
enum class agg_type {
	COUNT, // Количество
	SUM, // Сумма
	AVERAGE // Среднее
};

/**
 * @brief Результат составления выборки.
 */
enum class make_result {
	CUBE_NOT_FOUND = -2, // куб удален
	UNKNOWN_DIMENSION, // Неизвестное измерение
	NO_FACT_FOUND, // факты не найдены
	CREATED // выборка создана
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
// Тип данных выборки точек данных
// Ключ: пара <Название измерения, Название отметок на измерении>,
// Значение: Ключ: название метрики, Значение: указатель на точку данных
typedef std::unordered_map<std::pair<std::string, std::string>, std::unordered_multimap<std::string, FactClassifier*>, hash_pair> dpoint_ummaps_map;

/**
 * @brief Выборка.
 *
 * Класс для составления и агрегирования выборок на основе данных из куба.
 */
class Selection {
public:

	Selection(Cube* a_cube);

	// Создание выборки
	make_result make(const std::string& a_dim_name, const std::vector<std::string>& a_marks_list, const std::set<std::string>& a_metric_list = {});

	// Очистка выборки
	void clean(bool cube_cleaning = false);

	~Selection();

private:

	// Поиск измерения
	std::vector<Dimension*>::const_iterator find_dimension(const std::string& a_dimension_name) const;

	// Заполнение выборки (для изначально пустой выборки)
	void making
	(
		const std::map<std::string, DimensionMark*>& a_map_from,
		const std::string& a_dim_name, const std::vector<std::string>& a_marks_list,
		const std::set<std::string>& a_metric_list
	);

	// Заполнение выборки (для уже заполненной выборки)
	void making
	(
		const dpoint_ummaps_map& a_map_from,
		const std::string& a_dim_name,
		const std::vector<std::string>& a_marks_list,
		const std::set<std::string>& a_metric_list
	);

	// Получение точек данных с указанными метриками
	void get_FactClassifiers_by_metric
	(
		std::unordered_multimap<std::string, FactClassifier*>& a_umap_to,
		const std::unordered_multimap<std::string, FactClassifier*>& a_umap_from,
		const std::set<std::string>& a_metric_list
	);

	// Добавление оставшихся точек данных
	void add_rest_fact_points(dpoint_ummaps_map& a_map, const std::string& a_dim_name, const std::string& a_dim_mark);


	// Те точки данных куба, из которых состоит выборка
	dpoint_ummaps_map m_selection_points;
	// Метрики, созданные на основе агрегации
	std::vector<Metric*> m_aggregation_metrics;
	// Измерение для агрегаций
	Dimension* m_aggregation_dim;
	// Связанный куб для создания выборки
	Cube* m_cube;
};