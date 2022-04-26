#include "factclassifier.h"
#include "fact.h"
#include "dimension.h"
#include "dimensionmark.h"

/**
 * @brief Конструктор.
 *
 * @param [in,out] a_fact Связанный Факт
 * @param [in,out] a_dim Связанное Измерение
 * @param [in,out] a_dim Связанная Отметка на Измерении
 */
FactClassifier::FactClassifier(Fact* a_fact, Dimension* a_dim, DimensionMark* a_dim_mark) : m_fact(a_fact), m_dim(a_dim), m_dim_mark(a_dim_mark) {
	// Даём указатель на эту Классификатор Факту и Измерению, связанным с этим Классификатор
	m_fact->pushFactClassifier(this);
	m_dim->pushFactClassifier(this);
	m_dim_mark->pushFactClassifier(m_fact->MetricName(), this);
}

/**
 * @brief Получение связанного Измерения.
 *
 * @return Связанное Измерение
 */
const Dimension* const FactClassifier::getDimension() const {
	return m_dim;
}

/**
 * @brief Получение связанного Факта.
 *
 * @return Связанный Факт
 */
Fact* const FactClassifier::getFact() const {
	return m_fact;
}

/**
 * @brief Получение названия Отметки на Измерении.
 *
 * @return Название Отметки на связанном Измерении
 */
const std::string& FactClassifier::getDimMarkName() const {
	return m_dim_mark->name();
}
