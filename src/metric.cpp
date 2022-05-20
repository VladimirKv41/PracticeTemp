#include "metric.h"

/**
 * @brief Конструктор.
 *
 * @param [in,out] a_name Название Метрики
 */
Metric::Metric(const std::string& a_name) : m_name(a_name) {

}

/**
 * @brief Получение название Метрики.
 *
 * @return Название Метрики
 */
const std::string& Metric::name() const {
	return m_name;
}