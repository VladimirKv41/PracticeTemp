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
 * 1. Инициализация Куба для агрегации.
 * 2. Передача ссылки на данную Выборку Кубу.
 *
 * @param [in] a_cube Куб для создания Выборки
 */
Selection::Selection(Cube* a_cube) : m_cube(a_cube), m_aggregation_dim(new Dimension("Агрегации")) {
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
 * @param [in] a_dim_name Название Измерения (Добавляются Классификаторы только с этим названием Измерения,
 * а также все Классификаторы связанные с этими через Факт)
 * @param [in] a_marks_list Список названий Отметок на Измерении
 * (Добавляются Классификаторы только с указанными названиями Отметок)
 * @param [in] a_metric_list Список названий Метрик
 * (Добавляются Классификаторы только с этими названиями Метрик)
 * @return enum сlass make_result Результат составления Выборки:
 * Куб не найден/неизвестное Измерение/создана/Факты не найдены
 */
make_result Selection::make(const std::string& a_dim_name, const std::vector<std::string>& a_marks_list, const std::set<std::string>& a_metric_list)
{
	// Проверка, существует ли Куб
	if (m_cube == nullptr)
		return make_result::CUBE_NOT_FOUND;
	// Поиск указанного Измерения среди ранее добавленных
	std::vector<Dimension*>::const_iterator it_dim = findDimension(a_dim_name);
	if (it_dim == m_cube->m_dims.end()) {
		return make_result::UNKNOWN_DIMENSION;
	}
	// Заполнение Выборки
	if (m_selection_points.empty()) {
		making((*it_dim)->Marks(), a_dim_name, a_marks_list, a_metric_list);
	}
	else {
		making(m_selection_points, a_dim_name, a_marks_list, a_metric_list);
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
 * 1. Проверяется, нужно ли очищать Куб (по умолчанию - нет).
 * 2. Проверяется, есть ли добавленные Классификаторы, если да,
 * то Выборка очищается.
 *
 * @param [in] cube_cleaning Очищать ли информацию о Кубе (по умолчанию - нет)
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
	// Сообщение Кубу, что Выборка удалена
	if (m_cube != nullptr)
		m_cube->m_selection = nullptr;
}

/**
 * @brief Заполнение Выборки (для изначально пустой Выборки).
 *
 * Итерация по списку Отметок, каждая Отметка ищется на Измерении:
 *	1. Проверка перед добавлением, существует ли такая Отметка на Измерении.
 *	2. Добавление Классификаторов Отметки в массив Классификаторов Выборки
 *	(добавляются Отметки с указанными Метриками).
 *	3. Проверка, был ли найден хоть один Классификатор с указанной Метрикой,
 *	если нет, то соответствующая Отметка в Выборке удаляется.
 *	4. Добавление оставшихся Классификаторов, для "сборки" полноценных Фактов.
 *
 * @param [in] a_map_from Массив с Отметками на Измерении, из этого массива будет составляться Выборка
 * @param [in] a_dim_name Название Измерения, добавляются Классификаторы только с этим названием Измерения
 * @param [in] a_marks_list Список названий Отметок,
 * добавляются Классификаторы только с указанными названиями Отметок
 * @param [in] a_metric_list Список названий Метрик, добавляются Классификаторы только с этими названиями Метрик
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
		// Cуществует ли такая Отметка
		if (a_map_from.find(*it_dim_mark) == a_map_from.end()) \
			continue;
		// Добавление Классификаторов
		FactClassifiersByMetric(m_selection_points[{a_dim_name, * it_dim_mark}], a_map_from.at(*it_dim_mark)->FactClassifiers(), a_metric_list);
		// Был ли найден хоть один Классификатор
		if (m_selection_points[{a_dim_name, * it_dim_mark}].empty()) {
			m_selection_points.erase({ a_dim_name, *it_dim_mark });
			continue;
		}
		// Добавление оставшихся Классификаторов
		addRestFactClassifiers(m_selection_points, a_dim_name, *it_dim_mark);
	}
}

/**
 * @brief Заполнение Выборки (для уже заполненной Выборки).
 *
 * 1. Объявление временной Выборки для заполнения Классификаторами из основной,
 * в конце основная будет приравнена ко временной.
 * 2. Итерация по списку Отметок, каждая Отметка ищется в Выборке:
 *	2.1. Проверка перед добавлением, существует ли такая Отметка в Выборке.
 *	2.2. Добавление Классификатор Фактов Отметок во временный массив Классификаторов Выборки 
 *		(добавляются Отметок с указанными Метриками).
 *	2.3. Проверка, была ли найдена хоть одна Классификатор с указанной Метрикой,
 *		если нет, то соответствующая Отметка в временной Выборке удаляется.
 *	2.4. Добавление оставшихся Классификаторов, для "сборки" полноценных Фактов.
 * 3. Основная Выборка приравнивается к временной.
 *
 * @param [in] a_map_from Массив с позициями Измерения, из этого массива будет составляться Выборка
 * @param [in] a_dim_name Название Измерения, добавляются Классификаторы только с этим названием Измерения
 * @param [in] a_marks_list Список названий Отметок, добавляются Классификаторы только с указанными названиями Отметок
 * @param [in] a_metric_list Список названий Метрик, добавляются Классификаторы только с этими названиями Метрик
 */
void Selection::making
(
	const dpoint_ummaps_map& a_map_from,
	const std::string& a_dim_name,
	const std::vector<std::string>& a_marks_list,
	const std::set<std::string>& a_metric_list
)
{
	// Объявление временной Выборки
	dpoint_ummaps_map temp_selection_points;
	for (auto it_dim_mark = a_marks_list.begin(); it_dim_mark != a_marks_list.end(); it_dim_mark++) {
		// Cуществует ли такая Отметка
		if (a_map_from.find({ a_dim_name, *it_dim_mark }) == a_map_from.end())
			continue;
		// Добавление Классификаторов
		FactClassifiersByMetric(temp_selection_points[{a_dim_name, * it_dim_mark}], a_map_from.at({ a_dim_name, *it_dim_mark }), a_metric_list);
		// Был ли найден хоть один Классификатор
		if (temp_selection_points[{a_dim_name, * it_dim_mark}].empty()) {
			temp_selection_points.erase({ a_dim_name, *it_dim_mark });
			continue;
		}
		// Добавление оставшихся Классификаторов
		addRestFactClassifiers(temp_selection_points, a_dim_name, *it_dim_mark);
	}
	m_selection_points = temp_selection_points;
}

/**
 * @brief Получение Классификаторов с указанными Метриками.
 *
 * 1. Проверка, пустой ли список Метрик, если да, берутся все Классификаторы.
 * 2. Итерация по списку Метрик, каждая ищется и добавляется:
 *	2.2. Объявление переменной для хранения пары итераторов.
 *	2.2. Добавление Классификаторов.
 *
 * @param [out] a_map_to Массив, в который будут добавляться Классификаторы
 * @param [in] a_map_from  Массив, из которого будут браться Классификаторы
 * @param [in] a_metric_list Список названий Метрик, добавляются Классификаторы только с этими названиями Метрик
 */
void Selection::FactClassifiersByMetric
(
	std::unordered_multimap<std::string, FactClassifier*>& a_ummap_to,
	const std::unordered_multimap<std::string, FactClassifier*>& a_ummap_from,
	const std::set<std::string>& a_metric_list
)
{
	// Пустой ли список Метрик
	if (a_metric_list.empty()) {
		a_ummap_to = a_ummap_from;
		return;
	}
	for (auto it = a_metric_list.begin(); it != a_metric_list.end(); it++) {
		auto range_iter = a_ummap_from.equal_range(*it);
		// Добавление Классификаторов
		a_ummap_to.insert(range_iter.first, range_iter.second);
	}
}

/**
 * @brief Добавление оставшихся Классификаторов.
 *
 * 1. Итерация по Классификаторам(1) в Выборке:
 *		2. Итерация по Классификаторам(2) Факта, связанного с Классификатором(1),
 *		для "cборки" полноценного Факта:
 *			3. Добавление всех Классификатор оставшихся по соответствующим ключам в Выборке,
 *			кроме Классификатора(1).
 *
 * @param [in] a_map Массив, в который будут добавляться Классификаторы
 * @param [in] a_dim_name Название Измерения, добавляются все Классификаторы с НЕ таким названием Измерения
 * @param [in] a_dim_mark Название Отметки на Измерении
 * (Добавляем оставшиеся Классификаторы через Классификатор с Отметкой a_dim_mark и Измерением a_dim_name)
 */
void Selection::addRestFactClassifiers(dpoint_ummaps_map& a_map, const std::string& a_dim_name, const std::string& a_dim_mark) {
	auto& a_map_value = a_map[{a_dim_name, a_dim_mark}];
	for (auto it_dpoint = a_map_value.begin(); it_dpoint != a_map_value.end(); it_dpoint++) {
		// Итерация по точкам данных факта
		auto fact_datapoints = (*it_dpoint).second->getFact()->FactClassifiers();
		for (auto it_dpoint2 = fact_datapoints.begin(); it_dpoint2 != fact_datapoints.end(); it_dpoint2++) {
			auto& dim_name = (*it_dpoint2)->getDimension()->name();
			// Добавление всех точек данных
			if (dim_name != a_dim_name) {
				a_map[{dim_name, (*it_dpoint2)->getDimMarkName()}].emplace((*it_dpoint).second->getFact()->MetricName(), *it_dpoint2);
			}
		}
	}
}

/**
 * @brief Поиск Измерения.
 *
 * @param [in] a_dimension_name Название Bзмерения
 * @return Итератор на Измерение или end()
 */
std::vector<Dimension*>::const_iterator Selection::findDimension(const std::string& a_dimension_name) const {
	for (auto it_dim = m_cube->m_dims.begin(); it_dim != m_cube->m_dims.end(); it_dim++) {
		if ((*it_dim)->name() == a_dimension_name) {
			return it_dim;
		}
	}
	return m_cube->m_dims.end();
}