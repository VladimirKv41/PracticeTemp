#include "Measure.h"

/**
 * @brief Конструктор.
 * 
 * @param [in,out] a_name Название метрики
 */
Measure::Measure(const std::string& a_name) : m_name(a_name) {
	
}

/**
 * @brief Получение название метрики.
 * 
 * @return Название метрики
 */
const std::string& Measure::get_name() const {
	return m_name;
}



