#include "Selection.h"
#include "DataPoint.h"
#include "Cube.h"
#include "Dimension.h"
#include "Fact.h"
#include "Measure.h"
#include <iostream>
#include <map>

/**
 * @brief Конструктор
 * 
 * 1. Инициализация Куба и Измерения для агрегации
 * 2. Передача ссылки на данную Выборку Кубу
 * 
 * @param [in] a_cube Куб для создания выборки
 */
Selection::Selection(Cube* a_cube): m_cube(a_cube), m_aggregation_dim(new Dimension("Агрегации")) {
	m_cube->m_selection = this;
}

/**
 * @brief Создание выборки
 * 
 * 1. Итерация по Измерениям, Выборка создается только на основе указанного измерения
 * 2. Создается вектор из ТочекДанных, на основе которых составляется Выборка.
 * 3. Итерация по ТочкамДанных из вектора, если позиция в Измерении и Метрика совпадают с указанными,
 * то в Выборку добавляются все ТочкиДанных, связанные с тем же Фактом.
 * 4. Возврат значения, в зависимости от результата.
 * 
 * @param [in] a_dim_name Название Измерения
 * @param [in] a_positions_list Список позиций в Измерении
 * @param [in] a_measure_list Список Метрик
 * @return enum сlass make_result Результат составления выборки: Куб удален/Неизвестное Измерение/Cоздана/Факты не найдены
 */
make_result Selection::make(const std::string& a_dim_name, const std::vector<std::string>& a_positions_list, const std::vector<std::string>& a_measure_list) {
	// Проверка, существует ли куб
	if (m_aggregation_dim == nullptr)
		return make_result::CUBE_DELETED;
	for (std::vector<Dimension*>::const_iterator it_dim = m_cube->m_dims.begin(); it_dim != m_cube->m_dims.end(); it_dim++) {
		// Проверка, совпадает ли измерение с указанным
		if ((*it_dim)->get_name() == a_dim_name) {
			std::vector<DataPoint*> dpoint_vector;
			// Если Выборка уже заполнена, то новая Выборка составляется из уже имеющихся ТочекДанных
			if (m_selection_points.size()) {
				dpoint_vector = m_selection_points;
				m_selection_points.clear();
			}
			// Если не заполнена, то берутся все ТочкиДанных указанного измерения
			else
				dpoint_vector = (*it_dim)->get_DataPoints();
			// Итерация по ТочкамДанных из вектора
			for (std::vector<DataPoint*>::const_iterator it_dpoint = dpoint_vector.begin(); it_dpoint != dpoint_vector.end(); it_dpoint++) {
				for (std::vector<std::string>::const_iterator it_list = a_positions_list.begin(); it_list != a_positions_list.end(); it_list++) {
					if ((*it_dpoint)->get_dim_position_name() == (*it_list)) {
						// Если Метрики не указаны, то добавляются все ТочкиДанных
						if (!a_measure_list.size() || std::find(a_measure_list.begin(), a_measure_list.end(), (*it_dpoint)->get_Fact()->get_Measure_name()) != a_measure_list.end()) {
							m_selection_points.insert(std::end(m_selection_points), std::begin((*it_dpoint)->get_Fact()->get_DataPoints()), std::end((*it_dpoint)->get_Fact()->get_DataPoints()));
							break;
						}
					}
				}
			}
			if (m_selection_points.size())
				return make_result::CREATED;
			else
				return make_result::NO_FACT_FOUND;
		}
	}
	return make_result::UNKNOWN_DIMENSION;
}

/**
 * @brief Выбор агрегации
 * 
 * Вызывает приватную функцию агрегации в зависимости от параметров
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
	uint32_t counter = 0;
	// Вывод ТочекДанных на основе Куба
	for (std::vector<DataPoint*>::const_iterator it_dpoint = m_selection_points.begin(); it_dpoint != m_selection_points.end(); it_dpoint++) {
		std::cout << (*it_dpoint)->get_dim_position_name() << " | ";
		counter++;
		if (counter == m_cube->m_dims.size() ) {
			counter = 0;
			std::cout << (*it_dpoint)->get_Fact()->get_Measure_name() << " " << (*it_dpoint)->get_Fact()->get_value() << std::endl;
		}
	}
	counter = 0;
	// Вывод ТочекДанных на основе Аггрегации
	for (std::vector<DataPoint*>::const_iterator it_dpoint = m_aggregation_points.begin(); it_dpoint != m_aggregation_points.end(); it_dpoint++) {
		std::cout << (*it_dpoint)->get_dim_position_name() << " | ";
		if (counter == m_cube->m_dims.size()-1) {
			counter = 0;
			std::cout << (*it_dpoint)->get_Fact()->get_Measure_name() << " " << (*it_dpoint)->get_Fact()->get_value() << std::endl;
			continue;
		}
		counter++;
	}
	std::cout << std::endl;
}

/**
 * @brief Очистка Выборки
 * 
 * Сначала проверяется указатель на Измерение для аггрегаций, он служит флагом,
 * Выборка уже очищена или нет
 */
void Selection::clean() {
	if (m_aggregation_dim != nullptr) {
		delete m_aggregation_dim;
		m_aggregation_dim = nullptr;
		m_selection_points.resize(0);
		m_cube->clean_vector(m_aggregation_facts);
		m_cube->clean_vector(m_aggregation_measures);
		m_cube->clean_vector(m_aggregation_points);
	}
}

/**
 * @brief Деструктор
 */
Selection::~Selection() {
	clean();
	// Сообщение кубу, что Выборка удалена
	m_cube->m_selection = nullptr;
}

/**
 * @brief Агрегация - количество
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
	for (std::vector<Measure*>::const_iterator it_measure = m_cube->m_measures.begin(); it_measure != m_cube->m_measures.end(); it_measure++) {
		// Проверка, указана ли данная Метрика
		if (!a_measure_list.size() || std::find(a_measure_list.begin(), a_measure_list.end(), (*it_measure)->get_name()) != a_measure_list.end()) {
			uint8_t counter = 0;
			// Размер вектора на 1 меньше количества измерений, т.к. к агрегируемому Измерению будут создаваться новые ТочкиДанных
			std::vector<uint32_t> temp_vector(m_cube->m_dims.size() - 1);
			std::map<std::vector<uint32_t>, uint32_t> temp_map;
			// Заполнение map-контейнера
			for (std::vector<DataPoint*>::iterator it_dpoint = m_selection_points.begin(); it_dpoint != m_selection_points.end(); it_dpoint++) {
				if ((*it_dpoint)->get_Fact()->get_Measure() == *it_measure) {
					// Заполнение ключа-вектора
					if ((*it_dpoint)->get_Dimension()->get_name() != a_dimension_name) {
						temp_vector.at(counter) = (*it_dpoint)->get_dim_position_index();
						counter++;
					}
					// Подсчет количества
					if (counter == m_cube->m_dims.size() - 1) {
						temp_map[temp_vector]++;
						counter = 0;
					}
				}
			}
			// Получение индекса Метрики Количество
			uint8_t meas_index = std::find_if(m_aggregation_measures.begin(), m_aggregation_measures.end(), [](const Measure* m) { return (m->get_name() == "Количество"); }) - m_aggregation_measures.begin();
			if (meas_index == m_aggregation_measures.size()) {
				m_aggregation_measures.push_back(new Measure("Количество"));
			}
			// Cоздание Фактов и ТочекДанных
			for (std::map<std::vector<uint32_t>, uint32_t>::iterator it_map = temp_map.begin(); it_map != temp_map.end(); it_map++) {
				m_aggregation_facts.push_back(new Fact(it_map->second, m_aggregation_measures.at(meas_index)));
				counter = 0;
				for (std::vector<Dimension*>::const_iterator it_dim = m_cube->m_dims.begin(); it_dim != m_cube->m_dims.end(); it_dim++) {
					if ((*it_dim)->get_name() != a_dimension_name) {
						m_aggregation_points.push_back(new DataPoint(m_aggregation_facts.back(), *it_dim, it_map->first.at(counter)));
						counter++;
					}
					else {
						m_aggregation_points.push_back(new DataPoint(m_aggregation_facts.back(), m_aggregation_dim,
							m_aggregation_dim->get_mark("COUNT(" + m_cube->m_dims.at(it_dim - m_cube->m_dims.begin())->get_name() + ")")));
					}

				}
			}
		}
	}
};

/**
 * @brief Агрегация - суммирование
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
	for (std::vector<Measure*>::const_iterator it_measure = m_cube->m_measures.begin(); it_measure != m_cube->m_measures.end(); it_measure++) {
		// Проверка, указана ли данная Метрика
		if (!a_measure_list.size() || std::find(a_measure_list.begin(), a_measure_list.end(), (*it_measure)->get_name()) != a_measure_list.end()) {
			uint32_t counter = 0;
			// Размер вектора на 1 меньше количества измерений, т.к. к агрегируемому Измерению будут создаваться новые ТочкиДанных
			std::vector<uint32_t> temp_vector(m_cube->m_dims.size() - 1);
			std::map<std::vector<uint32_t>, double> temp_map;
			// Заполнение map-контейнера
			for (std::vector<DataPoint*>::iterator it_dpoint = m_selection_points.begin(); it_dpoint != m_selection_points.end(); it_dpoint++) {
				if ((*it_dpoint)->get_Fact()->get_Measure() == *it_measure) {
					// Заполнение ключа-вектора
					if ((*it_dpoint)->get_Dimension()->get_name() != a_dimension_name) {
						temp_vector.at(counter) = (*it_dpoint)->get_dim_position_index();
						counter++;
					}
					// Суммирование
					if (counter == m_cube->m_dims.size() - 1) {
						temp_map[temp_vector] += (*it_dpoint)->get_Fact()->get_value();
						counter = 0;
					}
				}
			}
			// Cоздание Фактов и ТочекДанных
			for (std::map<std::vector<uint32_t>, double>::iterator it_map = temp_map.begin(); it_map != temp_map.end(); it_map++) {
				m_aggregation_facts.push_back(new Fact(it_map->second, *it_measure));
				counter = 0;
				for (std::vector<Dimension*>::const_iterator it_dim = m_cube->m_dims.begin(); it_dim != m_cube->m_dims.end(); it_dim++) {
					if ((*it_dim)->get_name() != a_dimension_name) {
						m_aggregation_points.push_back(new DataPoint(m_aggregation_facts.back(), *it_dim, it_map->first.at(counter)));
						counter++;
					}
					else
					{
						m_aggregation_points.push_back(new DataPoint(m_aggregation_facts.back(), m_aggregation_dim,
							m_aggregation_dim->get_mark("SUM(" + m_cube->m_dims.at(it_dim - m_cube->m_dims.begin())->get_name() + ")")));
					}
				}
			}
		}
	}
};

/**
 * @brief Агрегация - среднее значение
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
	for (std::vector<Measure*>::const_iterator it_measure = m_cube->m_measures.begin(); it_measure != m_cube->m_measures.end(); it_measure++) {
		// Проверка, указана ли данная Метрика
		if (!a_measure_list.size() || std::find(a_measure_list.begin(), a_measure_list.end(), (*it_measure)->get_name()) != a_measure_list.end()) {
			uint32_t counter = 0;
			// Размер вектора на 1 меньше количества измерений, т.к. к агрегируемому Измерению будут создаваться новые ТочкиДанных
			std::vector<uint32_t> temp_vector(m_cube->m_dims.size() - 1);
			std::map<std::vector<uint32_t>, std::pair<double, uint32_t>> temp_map;
			// Заполнение map-контейнера
			for (std::vector<DataPoint*>::iterator it_dpoint = m_selection_points.begin(); it_dpoint != m_selection_points.end(); it_dpoint++) {
				if ((*it_dpoint)->get_Fact()->get_Measure() == *it_measure) {
					// Заполнение ключа-вектора
					if ((*it_dpoint)->get_Dimension()->get_name() != a_dimension_name) {
						temp_vector.at(counter) = (*it_dpoint)->get_dim_position_index();
						counter++;
					}
					// Суммирование и подсчет количества
					if (counter == m_cube->m_dims.size() - 1) {
						temp_map[temp_vector].first += (*it_dpoint)->get_Fact()->get_value();
						temp_map[temp_vector].second++;
						counter = 0;
					}
				}
			}
			// Cоздание Фактов и ТочекДанных
			for (std::map<std::vector<uint32_t>, std::pair<double, uint32_t>>::iterator it_map = temp_map.begin(); it_map != temp_map.end(); it_map++) {
				m_aggregation_facts.push_back(new Fact(it_map->second.first / it_map->second.second, *it_measure));
				counter = 0;
				for (std::vector<Dimension*>::const_iterator it_dim = m_cube->m_dims.begin(); it_dim != m_cube->m_dims.end(); it_dim++) {
					if ((*it_dim)->get_name() != a_dimension_name) {
						m_aggregation_points.push_back(new DataPoint(m_aggregation_facts.back(), *it_dim, it_map->first.at(counter)));
						counter++;
					}
					else {
						m_aggregation_points.push_back(new DataPoint(m_aggregation_facts.back(), m_aggregation_dim,
							m_aggregation_dim->get_mark("AVERAGE(" + m_cube->m_dims.at(it_dim - m_cube->m_dims.begin())->get_name() + ")")));
					}
				}
			}
		}

	}
};