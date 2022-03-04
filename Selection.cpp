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
 * 1. Инициализация куба и измерения для агрегации.
 * 2. Передача ссылки на данную выборку кубу.
 * 
 * @param [in] a_cube куб для создания выборки
 */
Selection::Selection(Cube* a_cube): m_cube(a_cube), m_aggregation_dim(new Dimension("Агрегации")) {
	m_cube->m_selection = this;
}


/**
 * @brief Создание выборки.
 * 
 * 1. Проверка, существует ли куб.
 * 2. Поиск указанного измерения среди ранее добавленных.
 * 3. Заполнение выборки, зависящее от того, пустая ли изначально выборка.
 * 4. Проверка, найден ли хоть один факт.
 * 
 * @param [in] a_dim_name Название измерения (Добавляются точки данных только с этим названием измерения,
 * а также все точки данных связанные с этими через факт)
 * @param [in] a_positions_list Список названий отметок в измерении 
 * (Добавляются точки данных только с указанными названиями отметок)
 * @param [in] a_measure_list Список названий метрик 
 * (Добавляются точки данных только с этими названиями метрик)
 * @return enum сlass make_result Результат составления выборки: 
 * куб не найден/неизвестное измерение/создана/факты не найдены
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
	// Заполнение выборки
	if (m_selection_points.empty()) {
		making((*it_dim)->get_positions(), a_dim_name, a_positions_list, a_measure_list);
	}
	else {
		making(m_selection_points, a_dim_name, a_positions_list, a_measure_list);
	}
	// Найдены ли факты
	if (m_selection_points.empty())
		return make_result::NO_FACT_FOUND;
	else
		return make_result::CREATED;
}

/**
 * @brief Очистка выборки.
 * 
 * 1. Проверяется, нужно ли очищать куб (по умолчанию - нет).
 * 2. Проверяется, есть ли добавленные точки данных, если да,
 * то выборка очищается.
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
	// Сообщение кубу, что выборка удалена
	if(m_cube!= nullptr)
		m_cube->m_selection = nullptr;
}

/**
 * @brief Заполнение выборки (для изначально пустой выборки).
 *
 * Итерация по списку отметок, каждая отметка ищется в измерении:
 *	1. Проверка перед добавлением, существует ли такая отметка в измерении.
 *	2. Добавление точек данных отметки в массив точек данных выборки 
 *	(добавляются отметки с указанными метриками).
 *	3. Проверка, была ли найдена хоть одна точка данных с указанной метрикой,
 *	если нет, то соответствующая отметка в выборке удаляется.
 *	4. Добавление оставшихся точек данных, для "сборки" полноценных фактов.
 *
 * @param [in] a_map_from Массив с позициями измерения, из этого массива будет составляться выборка
 * @param [in] a_dim_name Название измерения, добавляются точки данных только с этим названием измерения
 * @param [in] a_positions_list Список названий отметок, 
 * добавляются точки данных только с указанными названиями отметок
 * @param [in] a_measure_list Список названий метрик, добавляются точки данных только с этими названиями метрик
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
		// Cуществует ли такая отметка
		if (a_map_from.find(*it_dimposition) == a_map_from.end()) \
			continue;
		// Добавление точек данных
		get_DataPoints_by_measure(m_selection_points[{a_dim_name, * it_dimposition}], a_map_from.at(*it_dimposition)->get_DataPoints(), a_measure_list);
		// Была ли найдена хоть одна точка данных
		if (m_selection_points[{a_dim_name, * it_dimposition}].empty()) {
			m_selection_points.erase({ a_dim_name, *it_dimposition });
			continue;
		}
		// Добавление оставшихся точек данных
		add_rest_fact_points(m_selection_points, a_dim_name, *it_dimposition);
	}
}

/**
 * @brief Заполнение выборки (для уже заполненной выборки).
 *
 * 1. Объявление временной выборки для заполнения точками данных из основной,
 * в конце основная будет приравнена ко временной.
 * 2. Итерация по списку отметок, каждая отметка ищется в выборке:
 *	2.1. Проверка перед добавлением, существует ли такая отметка в выборке.
 *	2.2. Добавление точек данных отметок во временный массив точек данных выборки (добавляются отметок с указанными метриками).
 *	2.3. Проверка, была ли найдена хоть одна точка данных с указанной метрикой,
 *		если нет, то соответствующая отметка в временной выборке удаляется.
 *	2.4. Добавление оставшихся точек данных, для "сборки" полноценных фактов.
 * 3. Основная выборка приравнивается к временной.
 *
 * @param [in] a_map_from Массив с позициями измерения, из этого массива будет составляться выборка
 * @param [in] a_dim_name Название измерения, добавляются точки данных только с этим названием измерения
 * @param [in] a_positions_list Список названий отметок, добавляются точки данных только с указанными названиями отметок
 * @param [in] a_measure_list Список названий метрик, добавляются точки данных только с этими названиями метрик
 */
void Selection::making
(
	const dpoint_ummaps_map& a_map_from,
	const std::string& a_dim_name, 
	const std::vector<std::string>& a_positions_list, 
	const std::set<std::string>& a_measure_list
)
{
	// Объявление временной выборки
	dpoint_ummaps_map temp_selection_points;
	for (auto it_dimposition = a_positions_list.begin(); it_dimposition != a_positions_list.end(); it_dimposition++) {
		// Cуществует ли такая отметка
		if (a_map_from.find({ a_dim_name, *it_dimposition }) == a_map_from.end())
			continue;
		// Добавление точек данных
		get_DataPoints_by_measure(temp_selection_points[{a_dim_name, * it_dimposition}], a_map_from.at({ a_dim_name, *it_dimposition }), a_measure_list);
		// Была ли найдена хоть одна точка данных
		if (temp_selection_points[{a_dim_name, * it_dimposition}].empty()) {
			temp_selection_points.erase({ a_dim_name, *it_dimposition });
			continue;
		}
		// Добавление оставшихся точек данных
		add_rest_fact_points(temp_selection_points, a_dim_name, *it_dimposition);	
	}
	m_selection_points = temp_selection_points;
}

/**
 * @brief Получение точек данных с указанными метриками.
 *
 * 1. Проверка, пустой ли список метрик, если да, берутся все точки данных.
 * 2. Итерация по списку метрик, каждая ищется и добавляется:
 *	2.2. Объявление переменной для хранения пары итераторов.
 *	2.2. Добавление точек данных.
 * 
 * @param [out] a_map_to Массив, в который будут добавляться точки данных
 * @param [in] a_map_from  Массив, из которого будут браться точки данных
 * @param [in] a_measure_list Список названий метрик, добавляются точки данных только с этими названиями метрик
 */
void Selection::get_DataPoints_by_measure
(
	std::unordered_multimap<std::string, DataPoint*>& a_ummap_to,
	const std::unordered_multimap<std::string,DataPoint*>& a_ummap_from,
	const std::set<std::string>& a_measure_list
)
{
	// Пустой ли список метрик
	if (a_measure_list.empty()) {
		a_ummap_to = a_ummap_from;
		return;
	}
	for (auto it = a_measure_list.begin(); it != a_measure_list.end(); it++) {
		auto range_iter = a_ummap_from.equal_range(*it);
		// Добавление точек данных
		a_ummap_to.insert(range_iter.first, range_iter.second);
	}
}

/**
 * @brief Добавление оставшихся точек данных.
 *
 * 1. Итерация по точкам данных(1) в выборке:
 *		2. Итерация по точкам данных(2) факта, связанного с точкой данных(1),
 *		для "cборки" полноценного факта:
 *			3. Добавление всех точек данных оставшихся по соответствующим ключам в выборке,
 *			кроме точки данных(1).
 *
 * @param [in] a_map Массив, в который будут добавляться точки данных
 * @param [in] a_dim_name Название измерения, добавляются все точки данных с НЕ таким названием измерения
 * @param [in] a_dim_position Название отметки в измерении 
 * (Добавляем оставшиеся точки данных через точку данных с позицией a_dim_position и измерением a_dim_name)
 */
void Selection::add_rest_fact_points(dpoint_ummaps_map& a_map, const std::string& a_dim_name, const std::string& a_dim_position) {
	auto& aab = a_map[{a_dim_name, a_dim_position}];
	for (auto it_dpoint = aab.begin(); it_dpoint != aab.end(); it_dpoint++) {
		// Итерация по точкам данных факта
		auto fact_datapoints = (*it_dpoint).second->get_Fact()->get_DataPoints();
		for (auto it_dpoint2 = fact_datapoints.begin(); it_dpoint2 != fact_datapoints.end(); it_dpoint2++) {
			auto& dim_name = (*it_dpoint2)->get_Dimension()->get_name();
			// Добавление всех точек данных
			if (dim_name != a_dim_name) {
				a_map[{dim_name, (*it_dpoint2)->get_dim_position_name()}].emplace((*it_dpoint).second->get_Fact()->get_Measure_name() ,*it_dpoint2);
			}
		}
	}
}

/**
 * @brief Поиск измерения.
 *
 * @param [in] a_dimension_name Название измерения
 * @return Итератор на измерение или end()
 */
std::vector<Dimension*>::const_iterator Selection::find_dimension(const std::string& a_dimension_name) const {
	for (auto it_dim = m_cube->m_dims.begin(); it_dim != m_cube->m_dims.end(); it_dim++) {
		if ((*it_dim)->get_name() == a_dimension_name) {
			return it_dim;
		}
	}
	return m_cube->m_dims.end();
}