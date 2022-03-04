#include "DataPoint.h"
#include "Fact.h"
#include "Dimension.h"
#include "DimensionPosition.h"

/**
 * @brief Конструктор.
 * 
 * @param [in,out] a_fact Связанный факт
 * @param [in,out] a_dim Связанный измерение
 * @param [in,out] a_dim Связанная отметка на измерении
 */
DataPoint::DataPoint(Fact* a_fact, Dimension* a_dim, DimensionPosition* a_dim_position) : m_fact(a_fact), m_dim(a_dim), m_dim_position(a_dim_position) {
	// Даём указатель на эту точку данных Факту и Измерению, связанным с этой точкой данных
	m_fact->push_DataPoint(this);
	m_dim->push_DataPoint(this);
	m_dim_position->push_DataPoint(m_fact->get_Measure_name(),this);
}	

/**
 * @brief Получение измерения.
 * 
 * @return Связанное измерение
 */
const Dimension* const DataPoint::get_Dimension() const {
	return m_dim;
}

/**
 * @brief Получение факта.
 * 
 * @return Связанный факт
 */
Fact* const DataPoint::get_Fact() const {
	return m_fact;
}

/**
 * @brief Получение названия отметки в связанном измерении.
 * 
 * @return Название отметки на связанном измерении
 */
const std::string& DataPoint::get_dim_position_name() const {
	return m_dim_position->get_name();
}
