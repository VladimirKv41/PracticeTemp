#include "DataPoint.h"
#include "Fact.h"
#include "Dimension.h"
#include "DimensionPosition.h"

/**
 * @brief Конструктор.
 * 
 * @param [in,out] a_fact Факт
 * @param [in,out] a_dim Измерение
 * @param [in,out] a_dim Позиция на Измерении
 */
DataPoint::DataPoint(Fact* a_fact, Dimension* a_dim, DimensionPosition* a_dim_position) : m_fact(a_fact), m_dim(a_dim), m_dim_position(a_dim_position) {
	// Даём ссылку на эту ТочкуДанных Факту и Измерению, связанным с этой ТочкойДанных
	m_fact->push_DataPoint(this);
	m_dim->push_DataPoint(this);
	m_dim_position->push_DataPoint(m_fact->get_Measure_name(),this);
}	

/**
 * @brief Получение Измерения.
 * 
 * @return связанное Измерение
 */
const Dimension* const DataPoint::get_Dimension() const {
	return m_dim;
}

/**
 * @brief Получение Факта.
 * 
 * @return связанный Факт
 */
Fact* const DataPoint::get_Fact() const {
	return m_fact;
}

/**
 * @brief Получение названия Позиции в связанном Измерении.
 * 
 * @return Название Позиции на связанном Измерении
 */
const std::string& DataPoint::get_dim_position_name() const {
	return m_dim_position->get_name();
}
