#include "selection.h"
#include "factclassifier.h"
#include "cube.h"
#include "dimension.h"
#include "dimensionmark.h"
#include "fact.h"
#include "metric.h"

/**
 * @brief Конструктор.
 *
 * 1. Инициализация куба и измерения для агрегации.
 * 2. Передача ссылки на данную выборку кубу.
 *
 * @param [in] a_cube куб для создания выборки
 */
Selection::Selection(Cube* a_cube) : m_cube(a_cube), m_aggregation_dim(new Dimension("Агрегации")) {
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
 * @param [in] a_marks_list Список названий отметок в измерении
 * (Добавляются точки данных только с указанными названиями отметок)
 * @param [in] a_metric_list Список названий метрик
 * (Добавляются точки данных только с этими названиями метрик)
 * @return enum сlass make_result Результат составления выборки:
 * куб не найден/неизвестное измерение/создана/факты не найдены
 */
make_result Selection::make(const std::string& a_dim_name, const std::vector<std::string>& a_marks_list, const std::set<std::string>& a_metric_list)
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
		making((*it_dim)->getMarks(), a_dim_name, a_marks_list, a_metric_list);
	}
	else {
		making(m_selection_points, a_dim_name, a_marks_list, a_metric_list);
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
void Selection::clean(bool cube_cleaning) {
	if (cube_cleaning) {
		m_cube = nullptr;
	}
	if (!m_selection_points.empty()) {
		m_selection_points.clear();
		m_cube->cleanVector(m_aggregation_metrics);
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
	if (m_cube != nullptr)
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
 * @param [in] a_marks_list Список названий отметок,
 * добавляются точки данных только с указанными названиями отметок
 * @param [in] a_metric_list Список названий метрик, добавляются точки данных только с этими названиями метрик
 */
void Selection::making
(
	const std::map<std::string, DimensionMark*>& a_map_from,
	const std::string& a_dim_name,
	const std::vector<std::string>& a_marks_list,
	const std::set<std::string>& a_metric_list
)
{
	for (auto it_dim_mark = a_marks_list.begin(); it_dim_mark != a_marks_list.end(); it_dim_mark++) {
		// Cуществует ли такая отметка
		if (a_map_from.find(*it_dim_mark) == a_map_from.end()) \
			continue;
		// Добавление точек данных
		get_FactClassifiers_by_metric(m_selection_points[{a_dim_name, * it_dim_mark}], a_map_from.at(*it_dim_mark)->getFactClassifiers(), a_metric_list);
		// Была ли найдена хоть одна точка данных
		if (m_selection_points[{a_dim_name, * it_dim_mark}].empty()) {
			m_selection_points.erase({ a_dim_name, *it_dim_mark });
			continue;
		}
		// Добавление оставшихся точек данных
		add_rest_fact_points(m_selection_points, a_dim_name, *it_dim_mark);
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
 * @param [in] a_marks_list Список названий отметок, добавляются точки данных только с указанными названиями отметок
 * @param [in] a_metric_list Список названий метрик, добавляются точки данных только с этими названиями метрик
 */
void Selection::making
(
	const dpoint_ummaps_map& a_map_from,
	const std::string& a_dim_name,
	const std::vector<std::string>& a_marks_list,
	const std::set<std::string>& a_metric_list
)
{
	// Объявление временной выборки
	dpoint_ummaps_map temp_selection_points;
	for (auto it_dim_mark = a_marks_list.begin(); it_dim_mark != a_marks_list.end(); it_dim_mark++) {
		// Cуществует ли такая отметка
		if (a_map_from.find({ a_dim_name, *it_dim_mark }) == a_map_from.end())
			continue;
		// Добавление точек данных
		get_FactClassifiers_by_metric(temp_selection_points[{a_dim_name, * it_dim_mark}], a_map_from.at({ a_dim_name, *it_dim_mark }), a_metric_list);
		// Была ли найдена хоть одна точка данных
		if (temp_selection_points[{a_dim_name, * it_dim_mark}].empty()) {
			temp_selection_points.erase({ a_dim_name, *it_dim_mark });
			continue;
		}
		// Добавление оставшихся точек данных
		add_rest_fact_points(temp_selection_points, a_dim_name, *it_dim_mark);
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
 * @param [in] a_metric_list Список названий метрик, добавляются точки данных только с этими названиями метрик
 */
void Selection::get_FactClassifiers_by_metric
(
	std::unordered_multimap<std::string, FactClassifier*>& a_ummap_to,
	const std::unordered_multimap<std::string, FactClassifier*>& a_ummap_from,
	const std::set<std::string>& a_metric_list
)
{
	// Пустой ли список метрик
	if (a_metric_list.empty()) {
		a_ummap_to = a_ummap_from;
		return;
	}
	for (auto it = a_metric_list.begin(); it != a_metric_list.end(); it++) {
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
 * @param [in] a_dim_mark Название отметки в измерении
 * (Добавляем оставшиеся точки данных через точку данных с позицией a_dim_mark и измерением a_dim_name)
 */
void Selection::add_rest_fact_points(dpoint_ummaps_map& a_map, const std::string& a_dim_name, const std::string& a_dim_mark) {
	auto& a_map_value = a_map[{a_dim_name, a_dim_mark}];
	for (auto it_dpoint = a_map_value.begin(); it_dpoint != a_map_value.end(); it_dpoint++) {
		// Итерация по точкам данных факта
		auto fact_datapoints = (*it_dpoint).second->getFact()->getFactClassifiers();
		for (auto it_dpoint2 = fact_datapoints.begin(); it_dpoint2 != fact_datapoints.end(); it_dpoint2++) {
			auto& dim_name = (*it_dpoint2)->getDimension()->getName();
			// Добавление всех точек данных
			if (dim_name != a_dim_name) {
				a_map[{dim_name, (*it_dpoint2)->getDimMarkName()}].emplace((*it_dpoint).second->getFact()->getMetricName(), *it_dpoint2);
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
		if ((*it_dim)->getName() == a_dimension_name) {
			return it_dim;
		}
	}
	return m_cube->m_dims.end();
}