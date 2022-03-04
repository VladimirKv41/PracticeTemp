#include "Cube.h"
#include "Dimension.h"
#include "Measure.h"
#include "Fact.h"
#include "DataPoint.h"
#include "Selection.h"
#include <iostream>
#include <algorithm>

/**
 * @brief Конструктор
 * 
 * По умолчанию ссылка на Выборку обнулена
 */
Cube::Cube() : m_selection(nullptr){

}

/**
 * @brief Добавление Измерения
 * 
 * @param [in] a_dim_name Название Измерения
 * @return Добавлено Измерение или нет
 */
bool Cube::add_Dimension(const std::string& a_dim_name) {
	for (std::vector<Dimension*>::iterator it = m_dims.begin(); it != m_dims.end(); it++) {
		if ((*it)->get_name() == a_dim_name) {
			return false;
		}
	}
	m_dims.push_back(new Dimension(a_dim_name));
	return true;
}

/**
 * @brief Добавление Метрики
 * 
 * @param [in] a_measure_name Название Метрики
 * @return Добавлена Метрика или нет
 */
bool Cube::add_Measure(const std::string& a_measure_name) {
	for (std::vector<Measure*>::iterator it = m_measures.begin(); it != m_measures.end(); it++) {
		if ((*it)->get_name() == a_measure_name) {
			return false;
		}
	}
	m_measures.push_back(new Measure(a_measure_name));
	return true;
}

/**
 * @brief Добавление Факта
 * 
 * 1. Итерация по Метрикам, Факт добавляется только с указанной Метрикой.
 * 2. Поиск Факта в уже добавленных ранее.
 * 3. Добавление Факта и ТочекДанных.
 * 
 * @param [in] a_value Значение Факта
 * @param [in] a_measure Название Метрики
 * @param [in] a_positions_list Список позиций в Измерениях
 * @return enum class add_result Результат добавления Факта
 */
add_result Cube::add_Fact(double a_value, const std::string& a_measure, const std::vector<std::string>& a_positions_list) {
	for (std::vector<Measure*>::iterator it_measure = m_measures.begin(); it_measure != m_measures.end(); it_measure++) {
		// Проверка, соответсвует ли Метрика указанной
		if ((*it_measure)->get_name() == a_measure) {
			// Поиск в уже добавленных ранее Фактах, очень медленно на больших массивах
			std::vector<std::string>::const_iterator it_list = a_positions_list.begin();
			// Блок 1 : Проверка уникальности позиций в Измерениях, если есть уникальная, второй, более медленный, блок пропускается
			for (std::vector<Dimension*>::iterator it_dim = m_dims.begin(); it_dim != m_dims.end(); it_dim++) {
				if (!(*it_dim)->search_mark(*it_list)) {
					it_list = a_positions_list.end();
					break;
				}
				it_list++;
				if (it_dim == m_dims.end()-1)
					it_list = a_positions_list.begin();
			}
			// Блок 2 : Поиск среди всех Фактов
			if (it_list == a_positions_list.begin()) {
				for (std::vector<Fact*>::iterator it_fact = m_facts.begin(); it_fact != m_facts.end(); it_fact++) {
					it_list = a_positions_list.begin();
					if ((*it_fact)->get_Measure_name() != a_measure)
						continue;
					for (uint32_t i = 0; i < m_dims.size(); i++) {
						if ((*it_fact)->get_DataPoints().at(i)->get_dim_position_name() != *it_list) {
							break;
						}
						if (i == m_dims.size() - 1) {
							return add_result::ALREADY_EXIST;
						}
						it_list++;
					}
				}
			}
			// Добавление Факта и ТочекДанных
			m_facts.push_back(new Fact(a_value, *it_measure));
			it_list = a_positions_list.begin();
			for (std::vector<Dimension*>::iterator it_dim = m_dims.begin(); it_dim != m_dims.end(); it_dim++) {
				m_points.push_back(new DataPoint(m_facts.back(), *it_dim, (*it_dim)->get_mark(*it_list)));
				it_list++;
			}
			return add_result::ADDED;
		}
	}
	return add_result::UNKNOWN_MEASURE;
}

/**
 * @brief Очистка Куба
 */
void Cube::clean() {
	// Если Выборка удалена/не существует
	if(m_selection != nullptr)
		m_selection->clean();
	clean_vector(m_facts);
	clean_vector(m_measures);
	clean_vector(m_dims);
	clean_vector(m_points);
}

/**
 * @brief Деструктор
 */
Cube::~Cube() {
	clean();
}

/**
 * @brief Очистка вектора указателей
 * 
 * @param [in,out] a_vector Вектор из экземпляров классов
 */
template <class T>
void Cube::clean_vector(std::vector<T*>& a_vector) {
	for (class std::vector<T*>::iterator it = a_vector.begin(); it != a_vector.end(); it++) {
		delete* it;
	}
	// Уменьшение размера вектора для нуля, т.к. после очистки остаются висячие указатели
	a_vector.resize(0);
}