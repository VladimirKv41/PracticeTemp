#include "cube.h"
#include "dimension.h"
#include "dimensionmark.h"
#include "metric.h"
#include "fact.h"
#include "factclassifier.h"
#include "selection.h"

/**
 * @brief Конструктор.
 *
 */
Cube::Cube() {

}

/**
 * @brief Добавление Измерения.
 *
 * @param [in] a_dim_name Название Измерения, для поиска, а потом добавления
 * @return Добавлено Измерение или нет
 */
bool Cube::addDimension(const std::string& a_dim_name) {
	for (std::vector<Dimension*>::iterator it = m_dims.begin(); it != m_dims.end(); it++) {
		if ((*it)->getName() == a_dim_name) {
			return false;
		}
	}
	m_dims.push_back(new Dimension(a_dim_name));
	return true;
}

/**
 * @brief Добавление Метрики.
 *
 * @param [in] a_metric_name Название Метрики, для поиска, а потом добавления
 * @return Добавлена Метрика или нет
 */
bool Cube::addMetric(const std::string& a_metric_name) {
	for (std::vector<Metric*>::iterator it = m_metrics.begin(); it != m_metrics.end(); it++) {
		if ((*it)->getName() == a_metric_name) {
			return false;
		}
	}
	m_metrics.push_back(new Metric(a_metric_name));
	return true;
}

/**
 * @brief Добавление Факта.
 *
 * 1. Поиск указанной Метрики среди ранее добавленных.
 * 2. Поиск Факта в уже добавленных ранее.
 * 3. Добавление Факта и Классификаторов Факта.
 *
 * @param [in] a_value Значение добавляемого Факта
 * @param [in] a_metric Название Метрики добавляемого Факта
 * @param [in] a_marks_list Список Отметок на Измерениях добавляемого Факта
 * @return enum class add_result Результат добавления Факта : неизвестная Метрика/уже существует/добавлен
 */
add_result Cube::addFact(double a_value, const std::string& a_metric_name, const std::vector<std::string>& a_marks_list) {
	// Поиск указанной Метрики
	std::vector<Metric*>::const_iterator it_metric = findMetric(a_metric_name);
	if (it_metric == m_metrics.end()) {
		return add_result::UNKNOWN_MEASURE;
	}
	// Поиск в уже добавленных ранее Фактах
	if (m_facts.find({ a_metric_name, a_marks_list }) != m_facts.end())
		return add_result::ALREADY_EXIST;
	auto it_list = a_marks_list.begin();
	auto iter_new_fact = m_facts.insert({ { a_metric_name, a_marks_list }, new Fact(a_value, *it_metric) });
	// Добавление Классификаторов Факта
	for (auto it_dim = m_dims.begin(); it_dim != m_dims.end(); it_dim++, it_list++) {
		m_classifiers.push_back(new FactClassifier(iter_new_fact->second, *it_dim, (*it_dim)->getDimensionMark(*it_list)));
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
	cleanVector(m_metrics);
	cleanVector(m_dims);
	cleanVector(m_classifiers);
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
std::vector<Metric*>::const_iterator Cube::findMetric(const std::string& a_metric_name) {
	for (auto it_meas = m_metrics.begin(); it_meas != m_metrics.end(); it_meas++) {
		if ((*it_meas)->getName() == a_metric_name) {
			return it_meas;
		}
	}
	return m_metrics.end();
}

/**
 * @brief Очистка вектора указателей.
 *
 * @param [in,out] a_vector Вектор из экземпляров классов
 */
template <class T>
void Cube::cleanVector(std::vector<T*>& a_vector) {
	for (class std::vector<T*>::iterator it = a_vector.begin(); it != a_vector.end(); it++) {
		delete* it;
	}
	// Уменьшение размера вектора для нуля, т.к. после очистки остаются висячие указатели
	a_vector.resize(0);
}
