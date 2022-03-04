#include "DataPoint.h"
#include "Fact.h"
#include "Dimension.h"

/**
 * @brief Конструктор
 * 
 * @param [in,out] a_fact Факт
 * @param [in,out] a_dim Измерение
 * @param [in] a_position_index Индекс позиции в Измерении
 */
DataPoint::DataPoint(Fact* a_fact, Dimension* a_dim, uint32_t a_position_index) : m_fact(a_fact), m_dim(a_dim), m_position_index(a_position_index) {
	// Даём ссылку на эту ТочкуДанных Факту и Измерению, связанным с этой ТочкойДанных
	m_fact->push_DataPoint(this);
	m_dim->push_DataPoint(this);
}	

/**
 * @brief Получение Измерения
 * 
 * @return связанное Измерение
 */
const Dimension* const DataPoint::get_Dimension() const {
	return m_dim;
}

/**
 * @brief Получение Факта
 * 
 * @return связанный Факт
 */
const Fact* const DataPoint::get_Fact() const {
	return m_fact;
}

/**
 * @brief Получение позиции в связанном Измерении
 * 
 * @return 
 */
const std::string& DataPoint::get_dim_position_name() const {
	return m_dim->get_positions().at(m_position_index);
}

/**
 * @brief Получение индекса позиции в связанном Измерении
 * 
 * @return Индекса позиции в связанном Измерении
 */
uint32_t DataPoint::get_dim_position_index() const {
	return m_position_index;
}
