#include "factclassifier.h"
#include "fact.h"
#include "dimension.h"
#include "dimensionmark.h"

/**
 * @brief Конструктор.
 *
 * @param [in,out] a_fact Связанный факт
 * @param [in,out] a_dim Связанный измерение
 * @param [in,out] a_dim Связанная отметка на измерении
 */
FactClassifier::FactClassifier(Fact* a_fact, Dimension* a_dim, DimensionMark* a_dim_mark) : m_fact(a_fact), m_dim(a_dim), m_dim_mark(a_dim_mark) {
	// Даём указатель на эту точку данных Факту и Измерению, связанным с этой точкой данных
	m_fact->push_FactClassifier(this);
	m_dim->push_FactClassifier(this);
	m_dim_mark->push_FactClassifier(m_fact->get_Metric_name(), this);
}

/**
 * @brief Получение измерения.
 *
 * @return Связанное измерение
 */
const Dimension* const FactClassifier::get_Dimension() const {
	return m_dim;
}

/**
 * @brief Получение факта.
 *
 * @return Связанный факт
 */
Fact* const FactClassifier::get_Fact() const {
	return m_fact;
}

/**
 * @brief Получение названия отметки в связанном измерении.
 *
 * @return Название отметки на связанном измерении
 */
const std::string& FactClassifier::get_dim_mark_name() const {
	return m_dim_mark->get_name();
}
