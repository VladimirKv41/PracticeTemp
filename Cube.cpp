#include "Cube.h"
#include "Dimension.h"
#include "DimensionPosition.h"
#include "Measure.h"
#include "Fact.h"
#include "DataPoint.h"
#include "Selection.h"
#include <unordered_map>
/**
 * @brief Конструктор.
 * 
 * По умолчанию ссылка на Выборку обнулена.
 */
Cube::Cube() : m_selection(nullptr){

}

/**
 * @brief Добавление Измерения.
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
 * @brief Добавление Метрики.
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
 * @brief Добавление Факта.
 * 
 * 1. Поиск указанной Метрики среди ранее добавленных.
 * 2. Поиск Факта в уже добавленных ранее.
 * 3. Добавление Факта и ТочекДанных.
 * 
 * @param [in] a_value Значение Факта
 * @param [in] a_measure Название Метрики
 * @param [in] a_positions_list Список позиций в Измерениях
 * @return enum class add_result Результат добавления Факта : Неизвестная Метрика/Уже существует/Добавлен
 */
add_result Cube::add_Fact(double a_value, const std::string& a_measure_name, const std::vector<std::string>& a_positions_list) {
	// Поиск указанной Метрики
	std::vector<Measure*>::const_iterator it_measure = find_measure(a_measure_name);
	if (it_measure == m_measures.end()) {
		return add_result::UNKNOWN_MEASURE;
	}
	// Поиск в уже добавленных ранее Фактах
	if(m_facts.find({ a_measure_name, a_positions_list }) != m_facts.end())
		return add_result::ALREADY_EXIST;
	std::vector<std::string>::const_iterator it_list = a_positions_list.begin();
	auto iter_new_fact = m_facts.insert({ { a_measure_name, a_positions_list }, new Fact(a_value, *it_measure) });
	// Добавление ТочекДанных
	for (auto it_dim = m_dims.begin(); it_dim != m_dims.end(); it_dim++, it_list++) {
		m_points.push_back(new DataPoint(iter_new_fact->second, *it_dim, (*it_dim)->get_DimensionPosition(*it_list)));
	}
	return add_result::ADDED;
}

/**
 * @brief Очистка Куба.
 */
void Cube::clean() {
	// Если Выборка не удалена/существует
	if (m_selection != nullptr) {
		m_selection->clean(true);
	}
	for (auto it = m_facts.begin(); it != m_facts.end(); it++) {
		delete it->second;
	}
	m_facts.clear();
	clean_vector(m_measures);
	clean_vector(m_dims);
	clean_vector(m_points);
}

/**
 * @brief Деструктор.
 */
Cube::~Cube() {
	clean();
}

/**
 * @brief Поиск Метрики.
 *
 * @param [in] a_dimension_name Название Метрики
 * @return Итератор на Метрику или end()
 */
std::vector<Measure*>::const_iterator Cube::find_measure(const std::string& a_measure_name) {
	for (auto it_meas = m_measures.begin(); it_meas != m_measures.end(); it_meas++) {
		if ((*it_meas)->get_name() == a_measure_name) {
			return it_meas;
		}
	}
	return m_measures.end();
}

/**
 * @brief Очистка вектора указателей.
 * 
 * @param [in,out] a_vector Вектор из экземпляров классов
 */
template <class T>
void Cube::clean_vector(std::vector<T*>& a_vector) {
	for (class std::vector<T*>::iterator it = a_vector.begin(); it != a_vector.end(); it++) {
		delete *it;
	}
	// Уменьшение размера вектора для нуля, т.к. после очистки остаются висячие указатели
	a_vector.resize(0);
}
