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
 * @brief Результат составления Выборки.
 */
enum class make_result {
	CUBE_NOT_FOUND = -2, // Куб удален
	UNKNOWN_DIMENSION, // Неизвестное Измерение
	NO_FACT_FOUND, // Факты не найдены
	CREATED // Выборка создана
};

class DataPoint;
class Dimension;
class DimensionPosition;
class Cube;
class Fact;
class Measure;

/**
 * @brief Выборка.
 * 
 * Класс для составления и агрегирования выборок на основе данных из куба
 */
class Selection {
public:

	Selection(Cube* a_cube);

	// Создание выборки
	make_result make(const std::string& a_dim_name, const std::vector<std::string>& a_positions_list, const std::set<std::string>& a_measure_list = {});

	// Выбор агрегации
	bool aggregation(agg_type a_agg_type,const std::string& a_dimension_name, const std::vector<std::string>& a_measure_list = {});

	// Вывод Выборки
	void print() const;

	// Очистка Выборки
	void clean(bool cube_cleaning = false);

	~Selection();

private:
	struct hash_pair {
		template <class T1, class T2>
		size_t operator()(const std::pair<T1, T2>& p) const
		{
			auto hash1 = std::hash<T1>{}(p.first);
			auto hash2 = std::hash<T2>{}(p.second);
			return hash1 ^ hash2;
		}
	};
	// Ассоциативный массив, ключ: название Позиции в Измерении, значение: указатель на Позицию
	typedef std::map<std::string, DimensionPosition*> dim_positions_map;

	// Тип данных m_selection_points
	// Ассоциативный массив, ключ: пара <Название Измерения, Название Позиции на Измерении>,
	// значение: несортированный ассоциативный массив с повторяющимися ключами, ключ: название Метрики, 
	// значение: указатель на Точку Данных
	typedef std::unordered_map<std::pair<std::string,std::string>, std::unordered_multimap<std::string, DataPoint*>,hash_pair> dpoint_ummaps_map;

	// Поиск Измерения
	std::vector<Dimension*>::const_iterator find_dimension(const std::string& a_dimension_name) const;

	// Заполнение Выборки (для изначально пустой Выборки)
	void making(const dim_positions_map& a_map_from,
		const std::string& a_dim_name, const std::vector<std::string>& a_positions_list, const std::set<std::string>& a_measure_list);

	// Заполнение Выборки (для уже заполненной Выборки)
	void making(const dpoint_ummaps_map& a_map_from,
		const std::string& a_dim_name, const std::vector<std::string>& a_positions_list, const std::set<std::string>& a_measure_list);

	// Получение ТочекДанных с указанными метриками
	void get_DataPoints_by_measure(std::unordered_multimap<std::string, DataPoint*>& a_umap_to,
		const std::unordered_multimap<std::string, DataPoint*>& a_umap_from, const std::set<std::string>& a_measure_list);

	// Добавление оставшихся ТочекДанных
	void add_rest_fact_points(dpoint_ummaps_map& a_map, const std::string& a_dim_name, const std::string& a_dim_position);

	// Агрегация - суммирование
	void count(const std::string& a_dimension_name, const std::vector<std::string>& a_measure_list);

	// Агрегация - количество
	void sum(const std::string& a_dimension_name, const std::vector<std::string>& a_measure_list);

	// Агрегация - среднее значение
	void average(const std::string& a_dimension_name, const std::vector<std::string>& a_measure_list);
	
	// Удаление сведений о Кубе
	

	// Те ТочкиДанных Куба, из которых состоит выборка
	dpoint_ummaps_map m_selection_points;	
	// ТочкиДанных/Факты/Метрики, созданные на основе агрегации
	//std::vector<DataPoint*> m_aggregation_points;
	//std::vector<Fact*> m_aggregation_facts;
	std::vector<Measure*> m_aggregation_measures;
	// Измерение для агрегаций
	Dimension* m_aggregation_dim;
	// Связанный Куб для создания Выборки
	Cube* m_cube;
};