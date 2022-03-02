#include "Selection.h"
#include "DataPoint.h"
#include "Cube.h"
#include "Dimension.h"
#include "DimensionPosition.h"
#include "Fact.h"
#include "Measure.h"

/**
 * @brief Конструктор.
 * 
 * 1. Инициализация Куба и Измерения для агрегации.
 * 2. Передача ссылки на данную Выборку Кубу.
 * 
 * @param [in] a_cube Куб для создания выборки
 */
Selection::Selection(Cube* a_cube): m_cube(a_cube), m_aggregation_dim(new Dimension("Агрегации")) {
	m_cube->m_selection = this;
}

// Макросы
// Получение Позиций Измерения
#define dim_positions (*it_dim)->get_positions()

// Проверка существует ли ключ в map'е
#define is_exist(a_map, a_key_part, a_key_part_opt) \
	if (a_map.find({a_key_part, a_key_part_opt}) == a_map.end()) \
		continue;

// Получение ТочекДанных связанных с Фактом
#define fact_datapoints (*it_dpoint).second->get_Fact()->get_DataPoints()


/**
 * @brief Создание выборки.
 * 
 * 1. Проверка, существует ли Куб.
 * 2. Поиск указанного Измерения среди ранее добавленных.
 * 3. Заполнение Выборки, зависящее от того, пустая ли изначально Выборка.
 * 4. Проверка, найден ли хоть один Факт.
 * 
 * @param [in] a_dim_name Название Измерения
 * @param [in] a_positions_list Список Позиций в Измерении
 * @param [in] a_measure_list Список Метрик
 * @return enum сlass make_result Результат составления Выборки: Куб не найден/Неизвестное Измерение/Cоздана/Факты не найдены
 */
make_result Selection::make(const std::string& a_dim_name, const std::vector<std::string>& a_positions_list, const std::set<std::string>& a_measure_list) 
{
	// Проверка, существует ли куб
	if (m_cube == nullptr)
		return make_result::CUBE_NOT_FOUND;
	// Поиск указанного измерения среди ранее добавленных
	std::vector<Dimension*>::const_iterator it_dim = find_dimension(a_dim_name);
	if (it_dim == m_cube->m_dims.end()) {
		return make_result::UNKNOWN_DIMENSION;
	}
	// Заполнение Выборки
	if (m_selection_points.empty()) {
		making(dim_positions, a_dim_name, a_positions_list, a_measure_list);
	}
	else {
		making(m_selection_points, a_dim_name, a_positions_list, a_measure_list);
	}
	// Найдены ли Факты
	if (m_selection_points.empty())
		return make_result::NO_FACT_FOUND;
	else
		return make_result::CREATED;
}

/**
 * @brief Выбор агрегации.
 * 
 * Вызывает приватную функцию агрегации в зависимости от параметров.
 * 
 * @param [in] a_agg_type Тип агрегации
 * @param [in] a_dimension_name Название измерения
 * @param [in] a_measure_list Список Метрик
 * @return Найдена/вызвана ли функция или нет
 */
bool Selection::aggregation(agg_type a_agg_type, const std::string& a_dimension_name, const std::vector<std::string>& a_measure_list) {
	switch (a_agg_type) {
		case agg_type::COUNT:
			count(a_dimension_name, a_measure_list);
			return true;
		case agg_type::SUM:
			sum(a_dimension_name, a_measure_list);
			return true;
		case agg_type::AVERAGE:
			average(a_dimension_name, a_measure_list);
			return true;
		default:
			return false;
	}
}

/**
 * @brief Вывод Выборки
 */
void Selection::print() const {
	//uint32_t counter = 0;
	//// Вывод ТочекДанных на основе Куба
	//auto temp = m_selection_points.begin()->first.first;
	//m_selection_points.begin()->second;
	//for (auto it_map = m_selection_points.begin(); it_map != m_selection_points.end(); it_map++) {
	//	if (it_map->first.first == temp)
	//	{
	//		for (auto it_dpoint = it_map->second.begin(); it_dpoint != it_map->second.end(); it_dpoint++) {
	//			for (auto it_dpoint2 = (*it_dpoint).second->get_Fact()->get_DataPoints().begin(); it_dpoint2 != (*it_dpoint).second->get_Fact()->get_DataPoints().end(); it_dpoint2++) {
	//				std::cout << (*it_dpoint2)->get_dim_position_name() << " | ";
	//				counter++;
	//				if (counter == m_cube->m_dims.size()) {
	//					counter = 0;
	//					std::cout << (*it_dpoint2)->get_Fact()->get_Measure_name() << " " << (*it_dpoint2)->get_Fact()->get_value() << std::endl;
	//				}
	//			}
	//		}
	//	}
	//	else
	//		break;
	//}
	//counter = 0;
	//// Вывод ТочекДанных на основе Аггрегации
	//for (auto it_dpoint = m_aggregation_points.begin(); it_dpoint != m_aggregation_points.end(); it_dpoint++) {
	//	std::cout << (*it_dpoint)->get_dim_position_name() << " | ";
	//	if (counter == m_cube->m_dims.size()-1) {
	//		counter = 0;
	//		std::cout << (*it_dpoint)->get_Fact()->get_Measure_name() << " " << (*it_dpoint)->get_Fact()->get_value() << std::endl;
	//		continue;
	//	}
	//	counter++;
	//}
	//std::cout << std::endl;
}

/**
 * @brief Очистка Выборки.
 * 
 * 1. Проверяется, нужно ли очищать куб (по умолчанию - нет).
 * 2. Проверяется, есть ли добавленные ТочкиДанных, если да,
 * то Выборка очищается.
 * 
 * @param [in] cube_cleaning Очищать ли информацию о кубе (по умолчанию - нет)
 */
void Selection::clean(bool cube_cleaning){
	if (cube_cleaning) {
		m_cube = nullptr;
	}
	if (!m_selection_points.empty()) {
		m_selection_points.clear();
		m_cube->clean_vector(m_aggregation_measures);
		//m_cube->clean_vector(m_aggregation_points);
	}
}

/**
 * @brief Деструктор.
 */
Selection::~Selection() {
	clean();
	delete m_aggregation_dim;
	m_aggregation_dim = nullptr;
	// Сообщение кубу, что Выборка удалена
	if(m_cube!= nullptr)
		m_cube->m_selection = nullptr;
}

/**
 * @brief Заполнение Выборки (для изначально пустой Выборки).
 *
 * 1. Итерация по списку Позиций, каждая Позиция ищется в Измерении.
 * 2. Проверка перед добавлением, существует ли такая Позиция в Измерении.
 * 3. Добавление ТочекДанных Позиции в массив ТочекДанных Выборки (добавляются Позиции с указанными Метриками).
 * 4. Проверка, была ли найдена хоть одна ТочкаДанных с указанной Метрикой,
 * если нет, то соответствующая Позиция в Выборке удаляется.
 * 5. Добавление оставшихся ТочекДанных, для "сборки" полноценных Фактов.
 *
 * @param [in] a_map_from Ассоциативный массив с Позициями Измерения
 * @param [in] a_dim_name Название измерения
 * @param [in] a_positions_list Список Позиций
 * @param [in] a_measure_list Список Метрик
 */
void Selection::making(const dim_positions_map& a_map_from,
	const std::string& a_dim_name, const std::vector<std::string>& a_positions_list, const std::set<std::string>& a_measure_list)
{
	for (auto it_dimposition = a_positions_list.begin(); it_dimposition != a_positions_list.end(); it_dimposition++) {
		// Cуществует ли такая Позиция
		is_exist(a_map_from, *it_dimposition);
		// Добавление ТочекДанных
		get_DataPoints_by_measure(m_selection_points[{a_dim_name, * it_dimposition}], a_map_from.at(*it_dimposition)->get_DataPoints(), a_measure_list);
		// Была ли найдена хоть одна ТочкаДанных
		if (m_selection_points[{a_dim_name, * it_dimposition}].empty()) {
			m_selection_points.erase({ a_dim_name, *it_dimposition });
			continue;
		}
		// Добавление оставшихся ТочекДанных
		add_rest_fact_points(m_selection_points, a_dim_name, *it_dimposition);
	}
}

/**
 * @brief Заполнение Выборки (для уже заполненной Выборки).
 *
 * 1. Объявление временной Выборки для заполнения ТочкамиДанных из основной,
 * в конце основная будет приравнена ко временной.
 * 2. Итерация по списку Позиций, каждая Позиция ищется в Выборке.
 * 3. Проверка перед добавлением, существует ли такая Позиция в Выборке.
 * 4. Добавление ТочекДанных Позиции во временный массив ТочекДанных Выборки (добавляются Позиции с указанными Метриками).
 * 5. Проверка, была ли найдена хоть одна ТочкаДанных с указанной Метрикой,
 * если нет, то соответствующая Позиция в временной Выборке удаляется.
 * 6. Добавление оставшихся ТочекДанных, для "сборки" полноценных Фактов.
 *
 * @param [in] a_map_from Ассоциативный массив с Позициями Измерения
 * @param [in] a_dim_name Название измерения
 * @param [in] a_positions_list Список Позиций
 * @param [in] a_measure_list Список Метрик
 */
void Selection::making
(
	const dpoint_ummaps_map& a_map_from,
	const std::string& a_dim_name, 
	const std::vector<std::string>& a_positions_list, 
	const std::set<std::string>& a_measure_list
)
{
	// Объявление временной Выборки
	dpoint_ummaps_map temp_selection_points;
	for (auto it_dimposition = a_positions_list.begin(); it_dimposition != a_positions_list.end(); it_dimposition++) {
		// Cуществует ли такая Позиция
		is_exist(a_map_from, a_dim_name, *it_dimposition);
		// Добавление ТочекДанных
		get_DataPoints_by_measure(temp_selection_points[{a_dim_name, * it_dimposition}], a_map_from.at({ a_dim_name, *it_dimposition }), a_measure_list);
		// Была ли найдена хоть одна ТочкаДанных
		if (temp_selection_points[{a_dim_name, * it_dimposition}].empty()) {
			temp_selection_points.erase({ a_dim_name, *it_dimposition });
			continue;
		}
		// Добавление оставшихся ТочекДанных
		add_rest_fact_points(temp_selection_points, a_dim_name, *it_dimposition);	
	}
	m_selection_points = temp_selection_points;
}

/**
 * @brief Получение ТочекДанных с указанными метриками.
 *
 * 1. Проверка, пустой ли список Метрик, если да, берутся все ТочкиДанных.
 * 2. Итерация по списку Метрик, каждая ищется и добавляется.
 * 3. Объявление переменной для хранения пары итераторов.
 * 4. Добавление ТочекДанных.
 * 
 * @param [out] a_map_to Ассоциативный массив, в который будут добавляться ТочкиДанных
 * @param [in] a_map_from  Ассоциативный массив, из которого будут браться ТочкиДанных
 * @param [in] a_measure_list Список Метрик
 */
void Selection::get_DataPoints_by_measure(std::unordered_multimap<std::string, DataPoint*>& a_ummap_to, const std::unordered_multimap<std::string,
	DataPoint*>& a_ummap_from, const std::set<std::string>& a_measure_list)
{
	// Пустой ли список Метрик
	if (a_measure_list.empty()) {
		a_ummap_to = a_ummap_from;
		return;
	}
	for (auto it = a_measure_list.begin(); it != a_measure_list.end(); it++) {
		auto range_iter = a_ummap_from.equal_range(*it);
		// Добавление ТочекДанных
		a_ummap_to.insert(range_iter.first, range_iter.second);
	}
}

/**
 * @brief Добавление оставшихся ТочекДанных.
 *
 * 1. Итерация по ТочкамДанных(1) Позиции в Выборке.
 * 2. Итерация по ТочкамДанных(2) Факта, связанного с ТочкойДанных(1),
 *  для "cборки" полноценного Факта.
 * 3. Объявление переменной для хранения названия Измерения ТочкиДанных(2).
 * 4. Добавление всех ТочекДанных оставщихся в соответствующие Позиции в Выборке, кроме ТочкиДанных(1).
 * 5. Ассоциативному массиву ТочекДанных Выборки присваивается значение "собранного" Ассоциативному массива.
 *
 * @param [in] a_map Ассоциативный массив, в который будут добавляться ТочкиДанных
 * @param [in] a_dim_name Название Измерения
 * @param [in] a_dim_position Название Позиции в Измерении
 */
void Selection::add_rest_fact_points(dpoint_ummaps_map& a_map, const std::string& a_dim_name, const std::string& a_dim_position) {
	auto& aab = a_map[{a_dim_name, a_dim_position}];
	for (auto it_dpoint = aab.begin(); it_dpoint != aab.end(); it_dpoint++) {
		// Итерация по ТочкамДанных Факта
		for (auto it_dpoint2 = fact_datapoints.begin(); it_dpoint2 != fact_datapoints.end(); it_dpoint2++) {
			auto& dim_name = (*it_dpoint2)->get_Dimension()->get_name();
			// Добавление всех ТочекДанных
			if (dim_name != a_dim_name) {
				a_map[{dim_name, (*it_dpoint2)->get_dim_position_name()}].emplace((*it_dpoint).second->get_Fact()->get_Measure_name() ,*it_dpoint2);
			}
		}
	}
}

/**
 * @brief Агрегация - количество.
 *
 * 1. Итерация по Метрикам, агрегируются только указанные, если не указаны, то по всем.
 * 2. Создание ассоциативного контейнера(map): ключ - вектор из чисел, где число - индекс позиции в Измерении; значение - количество.
 * 3. Итерация по ТочкамДанных выборки и заполнение map-контейнера.
 * 4. Итерация по полученному map-контейнеру и создание новых ТочекДанных и Фактов к ним.
 *
 * @param [in] a_dimension_name Название Измерения
 * @param [in] a_measure_list Список Метрик
 */
void Selection::count(const std::string& a_dimension_name, const std::vector<std::string>& a_measure_list) {
	//for (std::vector<Measure*>::const_iterator it_measure = m_cube->m_measures.begin(); it_measure != m_cube->m_measures.end(); it_measure++) {
	//	// Проверка, указана ли данная Метрика
	//	if (!a_measure_list.size() || std::find(a_measure_list.begin(), a_measure_list.end(), (*it_measure)->get_name()) != a_measure_list.end()) {
	//uint8_t counter = 0;
	//// Размер вектора на 1 меньше количества измерений, т.к. к агрегируемому Измерению будут создаваться новые ТочкиДанных
	//std::vector<std::string> temp_vector(m_cube->m_dims.size() - 1);
	//std::map<std::vector<std::string>, uint32_t> temp_map;
	//// Заполнение map-контейнера
	////for (auto it_map = m_selection_points.lower_bound(std::make_pair(a_dimension_name, "")); it_map->first.first == a_dimension_name; it_map++) {
	//	//for (auto it_dpoint = it_map->second.begin(); it_dpoint != it_map->second.end(); it_dpoint++) {
	//		//for (auto it_dpoint2 = (*it_dpoint)->get_Fact()->get_DataPoints().begin(); it_dpoint2 != (*it_dpoint)->get_Fact()->get_DataPoints().end(); it_dpoint2++) {
	//		//	// Заполнение ключа-вектора
	//		//	if ((*it_dpoint2)->get_Dimension()->get_name() != a_dimension_name) {
	//		//		temp_vector.at(counter) = (*it_dpoint2)->get_dim_position_name();
	//		//		counter++;
	//		//	}
	//		//	// Подсчет количества
	//		//	if (counter == m_cube->m_dims.size() - 1) {
	//		//		temp_map[temp_vector]++;
	//		//		counter = 0;
	//		//	}
	//		//}
	//	//}
	////}
	//// Получение индекса Метрики Количество
	//uint8_t meas_index = std::find_if(m_aggregation_measures.begin(), m_aggregation_measures.end(), [](const Measure* m) { return (m->get_name() == "Количество"); }) - m_aggregation_measures.begin();
	//if (meas_index == m_aggregation_measures.size()) {
	//	m_aggregation_measures.push_back(new Measure("Количество"));
	//}
	//// Cоздание Фактов и ТочекДанных
	//m_aggregation_facts.reserve(temp_map.size());
	//for (auto it_map = temp_map.begin(); it_map != temp_map.end(); it_map++) {
	//	m_aggregation_facts.push_back(new Fact(it_map->second, m_aggregation_measures.at(meas_index)));
	//	counter = 0;
	//	for (auto it_dim = m_cube->m_dims.begin(); it_dim != m_cube->m_dims.end(); it_dim++) {
	//		if ((*it_dim)->get_name() != a_dimension_name) {
	//			m_aggregation_points.push_back(new DataPoint(m_aggregation_facts.back(), *it_dim, (*it_dim)->get_DimensionPosition(it_map->first.at(counter))));
	//			counter++;
	//		}
	//		else {
	//
	//			m_aggregation_points.push_back(new DataPoint(m_aggregation_facts.back(), m_aggregation_dim,
	//				m_aggregation_dim->get_DimensionPosition("COUNT(" + m_cube->m_dims.at(it_dim - m_cube->m_dims.begin())->get_name() + ")")));
	//		}
	//
	//	}
	//}
	//}
//}
};

/**
 * @brief Агрегация - суммирование.
 *
 * 1. Итерация по Метрикам, агрегируются только указанные, если не указаны, то по всем.
 * 2. Создание ассоциативного контейнера(map): ключ - вектор из чисел, где число - индекс позиции в Измерении; значение - сумма.
 * 3. Итерация по ТочкамДанных выборки и заполнение map-контейнера.
 * 4. Итерация по полученному map-контейнеру и создание новых ТочекДанных и Фактов к ним.
 *
 * @param [in] a_dimension_name Название Измерения
 * @param [in] a_measure_list Список Метрик
 */
void Selection::sum(const std::string& a_dimension_name, const std::vector<std::string>& a_measure_list) {
	//for (std::vector<Measure*>::const_iterator it_measure = m_cube->m_measures.begin(); it_measure != m_cube->m_measures.end(); it_measure++) {
	//	// Проверка, указана ли данная Метрика
	//	if (!a_measure_list.size() || std::find(a_measure_list.begin(), a_measure_list.end(), (*it_measure)->get_name()) != a_measure_list.end()) {
	//		uint32_t counter = 0;
	//		// Размер вектора на 1 меньше количества измерений, т.к. к агрегируемому Измерению будут создаваться новые ТочкиДанных
	//		std::vector<uint32_t> temp_vector(m_cube->m_dims.size() - 1);
	//		std::map<std::vector<uint32_t>, double> temp_map;
	//		// Заполнение map-контейнера
	//		for (std::vector<DataPoint*>::iterator it_dpoint = m_selection_points.begin(); it_dpoint != m_selection_points.end(); it_dpoint++) {
	//			if ((*it_dpoint)->get_Fact()->get_Measure() == *it_measure) {
	//				// Заполнение ключа-вектора
	//				if ((*it_dpoint)->get_Dimension()->get_name() != a_dimension_name) {
	//					temp_vector.at(counter) = (*it_dpoint)->get_dim_position_index();
	//					counter++;
	//				}
	//				// Суммирование
	//				if (counter == m_cube->m_dims.size() - 1) {
	//					temp_map[temp_vector] += (*it_dpoint)->get_Fact()->get_value();
	//					counter = 0;
	//				}
	//			}
	//		}
	//		// Cоздание Фактов и ТочекДанных
	//		for (std::map<std::vector<uint32_t>, double>::iterator it_map = temp_map.begin(); it_map != temp_map.end(); it_map++) {
	//			m_aggregation_facts.push_back(new Fact(it_map->second, *it_measure));
	//			counter = 0;
	//			for (std::vector<Dimension*>::const_iterator it_dim = m_cube->m_dims.begin(); it_dim != m_cube->m_dims.end(); it_dim++) {
	//				if ((*it_dim)->get_name() != a_dimension_name) {
	//					m_aggregation_points.push_back(new DataPoint(m_aggregation_facts.back(), *it_dim, it_map->first.at(counter)));
	//					counter++;
	//				}
	//				else
	//				{
	//					m_aggregation_points.push_back(new DataPoint(m_aggregation_facts.back(), m_aggregation_dim,
	//						m_aggregation_dim->get_mark("SUM(" + m_cube->m_dims.at(it_dim - m_cube->m_dims.begin())->get_name() + ")")));
	//				}
	//			}
	//		}
	//	}
	//}
};

/**
 * @brief Агрегация - среднее значение.
 *
 * 1. Итерация по Метрикам, агрегируются только указанные, если не указаны, то по всем.
 * 2. Создание ассоциативного контейнера(map): ключ - вектор из чисел, где число - индекс позиции в Измерении; значение - пара, где
 * первое число - сумма, второе - количество.
 * 3. Итерация по ТочкамДанных выборки и заполнение map-контейнера.
 * 4. Итерация по полученному map-контейнеру и создание новых ТочекДанных и Фактов к ним.
 *
 * @param [in] a_dimension_name Название Измерения
 * @param [in] a_measure_list Список Метрик
 */
void Selection::average(const std::string& a_dimension_name, const std::vector<std::string>& a_measure_list) {
	//for (std::vector<Measure*>::const_iterator it_measure = m_cube->m_measures.begin(); it_measure != m_cube->m_measures.end(); it_measure++) {
	//	// Проверка, указана ли данная Метрика
	//	if (!a_measure_list.size() || std::find(a_measure_list.begin(), a_measure_list.end(), (*it_measure)->get_name()) != a_measure_list.end()) {
	//		uint32_t counter = 0;
	//		// Размер вектора на 1 меньше количества измерений, т.к. к агрегируемому Измерению будут создаваться новые ТочкиДанных
	//		std::vector<uint32_t> temp_vector(m_cube->m_dims.size() - 1);
	//		std::map<std::vector<uint32_t>, std::pair<double, uint32_t>> temp_map;
	//		// Заполнение map-контейнера
	//		for (std::vector<DataPoint*>::iterator it_dpoint = m_selection_points.begin(); it_dpoint != m_selection_points.end(); it_dpoint++) {
	//			if ((*it_dpoint)->get_Fact()->get_Measure() == *it_measure) {
	//				// Заполнение ключа-вектора
	//				if ((*it_dpoint)->get_Dimension()->get_name() != a_dimension_name) {
	//					temp_vector.at(counter) = (*it_dpoint)->get_dim_position_index();
	//					counter++;
	//				}
	//				// Суммирование и подсчет количества
	//				if (counter == m_cube->m_dims.size() - 1) {
	//					temp_map[temp_vector].first += (*it_dpoint)->get_Fact()->get_value();
	//					temp_map[temp_vector].second++;
	//					counter = 0;
	//				}
	//			}
	//		}
	//		// Cоздание Фактов и ТочекДанных
	//		for (std::map<std::vector<uint32_t>, std::pair<double, uint32_t>>::iterator it_map = temp_map.begin(); it_map != temp_map.end(); it_map++) {
	//			m_aggregation_facts.push_back(new Fact(it_map->second.first / it_map->second.second, *it_measure));
	//			counter = 0;
	//			for (std::vector<Dimension*>::const_iterator it_dim = m_cube->m_dims.begin(); it_dim != m_cube->m_dims.end(); it_dim++) {
	//				if ((*it_dim)->get_name() != a_dimension_name) {
	//					m_aggregation_points.push_back(new DataPoint(m_aggregation_facts.back(), *it_dim, it_map->first.at(counter)));
	//					counter++;
	//				}
	//				else {
	//					m_aggregation_points.push_back(new DataPoint(m_aggregation_facts.back(), m_aggregation_dim,
	//						m_aggregation_dim->get_mark("AVERAGE(" + m_cube->m_dims.at(it_dim - m_cube->m_dims.begin())->get_name() + ")")));
	//				}
	//			}
	//		}
	//	}
	//
	//}
};

/**
 * @brief Поиск Измерения.
 *
 * @param [in] a_dimension_name Название Измерения
 * @return Итератор на Измерение или end()
 */
std::vector<Dimension*>::const_iterator Selection::find_dimension(const std::string& a_dimension_name) const {
	for (auto it_dim = m_cube->m_dims.begin(); it_dim != m_cube->m_dims.end(); it_dim++) {
		if ((*it_dim)->get_name() == a_dimension_name) {
			return it_dim;
		}
	}
	return m_cube->m_dims.end();
}