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
 * @brief Добавление связанного Классификатора Факта.
 *
 * @param [in,out] a_point Классификатор Факта
 */
void Fact::pushFactClassifier(FactClassifier* a_point) {
	m_classifiers.push_back(a_point);
}

/**
 * @brief Получение вектора связанных Классификаторов Факта.
 *
 * @return Вектор связанных Классификаторов Факта
 */
const std::vector<FactClassifier*>& Fact::FactClassifiers() const {
	return m_classifiers;
}

/**
 * @brief Получение значения Факта.
 *
 * @return Значения Факта
 */
double Fact::value() const {
	return m_value;
}

/**
 * @brief Получение названия Метрики Факта.
 *
 * @return Название Метрики
 */
const std::string& Fact::MetricName() const {
	return m_mes->name();
}

/**
 * @brief Получение связанной Метрики.
 *
 * @return Связанная Метрика
 */
const Metric* const Fact::metric() const {
	return m_mes;
}