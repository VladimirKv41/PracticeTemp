#include "fact.h"
#include "metric.h"


/**
 * @brief Конструктор.
 *
 * @param [in] a_value Значение
 * @param [in,out] a_mes Метрика
 */
Fact::Fact(double a_value, Metric* a_mes) : m_value(a_value), m_mes(a_mes) {

}

/**
 * @brief Добавление связанной точки данных.
 *
 * @param [in,out] a_point Точка данных
 */
void Fact::push_FactClassifier(FactClassifier* a_point) {
	m_points.push_back(a_point);
}

/**
 * @brief Получение вектора связанных точек данных.
 *
 * @return Вектор связанных точек данных
 */
const std::vector<FactClassifier*>& Fact::get_FactClassifiers() const {
	return m_points;
}

/**
 * @brief Получение значения факта.
 *
 * @return Значения факта
 */
double Fact::get_value() const {
	return m_value;
}

/**
 * @brief Получение названия метрики факта.
 *
 * @return Название метрики
 */
const std::string& Fact::get_Metric_name() const {
	return m_mes->get_name();
}

/**
 * @brief Получение связанной метрики.
 *
 * @return Связанная метрика
 */
const Metric* const Fact::get_Metric() const {
	return m_mes;
}