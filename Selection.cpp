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


/**
 * @brief Создание Выборки.
 * 
 * 1. Проверка, существует ли Куб.
 * 2. Поиск указанного Измерения среди ранее добавленных.
 * 3. Заполнение Выборки, зависящее от того, пустая ли изначально Выборка.
 * 4. Проверка, найден ли хоть один Факт.
 * 
 * @param [in] a_dim_name Название Измерения (Добавляются ТочкиДанных только с этим названием Измерения,
 *  а также все ТочкиДанных связанные с этими через Факт)
 * @param [in] a_positions_list Список названий Позиций в Измерении (Добавляются ТочкиДанных только с указанными названиями Позиции)
 * @param [in] a_measure_list Список названий Метрик (Добавляются ТочкиДанных только с этими названиями Метрик)
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
		making((*it_dim)->get_positions(), a_dim_name, a_positions_list, a_measure_list);
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
 * @param [in] a_map_from Ассоциативный массив с Позициями Измерения,
 *  из этого массива будет составляться Выборка
 * @param [in] a_dim_name Название измерения, добавляются ТочкиДанных только с этим названием Измерения
 * @param [in] a_positions_list Список названий Позиций, добавляются ТочкиДанных только с указанными названиями Позиции
 * @param [in] a_measure_list Список названий Метрик, добавляются ТочкиДанных только с этими названиями Метрик
 */
void Selection::making
(
	const std::map<std::string, DimensionPosition*>& a_map_from,
	const std::string& a_dim_name,
	const std::vector<std::string>& a_positions_list,
	const std::set<std::string>& a_measure_list
)
{
	for (auto it_dimposition = a_positions_list.begin(); it_dimposition != a_positions_list.end(); it_dimposition++) {
		// Cуществует ли такая Позиция
		if (a_map_from.find(*it_dimposition) == a_map_from.end()) \
			continue;
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
 * @param [in] a_map_from Ассоциативный массив с Позициями Измерения,
 *  из этого массива будет составляться Выборка
 * @param [in] a_dim_name Название измерения, добавляются ТочкиДанных только с этим названием Измерения
 * @param [in] a_positions_list Список названий Позиций, добавляются ТочкиДанных только с указанными названиями Позиции
 * @param [in] a_measure_list Список названий Метрик, добавляются ТочкиДанных только с этими названиями Метрик
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
		if (a_map_from.find({ a_dim_name, *it_dimposition }) == a_map_from.end())
			continue;
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
 * @param [in] a_measure_list Список названий Метрик, добавляются ТочкиДанных только с этими названиями Метрик
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
 * 1. Итерация по ТочкамДанных(1) в Выборке.
 * 2. Итерация по ТочкамДанных(2) Факта, связанного с ТочкойДанных(1),
 *  для "cборки" полноценного Факта.
 * 3. Добавление всех ТочекДанных оставшихся по соответствующим ключам в Выборке, кроме ТочкиДанных(1).
 * 4. Ассоциативному массиву ТочекДанных Выборки присваивается значение "собранного" Ассоциативному массива.
 *
 * @param [in] a_map Ассоциативный массив, в который будут добавляться ТочкиДанных
 * @param [in] a_dim_name Название измерения, добавляются ТочкиДанных только НЕ с этим названием Измерения
 * @param [in] a_dim_position Название Позиции в Измерении (Добавляем ТочкиДанных через ТочкуДанных с такой позицией и
 *  Измерением с названием a_dim_name)
 */
void Selection::add_rest_fact_points(dpoint_ummaps_map& a_map, const std::string& a_dim_name, const std::string& a_dim_position) {
	auto& aab = a_map[{a_dim_name, a_dim_position}];
	for (auto it_dpoint = aab.begin(); it_dpoint != aab.end(); it_dpoint++) {
		// Итерация по ТочкамДанных Факта
		auto fact_datapoints = (*it_dpoint).second->get_Fact()->get_DataPoints();
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
