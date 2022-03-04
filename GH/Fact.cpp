#include "Fact.h"
#include "Measure.h"


/**
 * @brief Конструктор
 * 
 * @param [in] a_value Значение
 * @param [in,out] a_mes Метрика
 */
Fact::Fact(double a_value,Measure* a_mes) : m_value(a_value), m_mes(a_mes) {
	
}

/**
 * @brief Добавление связанной ТочкиДанных
 * 
 * @param [in,out] a_point ТочкаДанных
 */
void Fact::push_DataPoint(DataPoint* a_point) {
	m_points.push_back(a_point);
}

/**
 * @brief Получение вектора связанных ТочекДанных
 * 
 * @return Вектор связанных ТочекДанных
 */
const std::vector<DataPoint*>& Fact::get_DataPoints() const {
	return m_points;
}

/**
 * @brief Получение значения Факта
 * 
 * @return значения Факта
 */
double Fact::get_value() const {
	return m_value;
}

/**
 * @brief Получение названия Метрики Факта
 * 
 * @return название Метрики
 */
const std::string& Fact::get_Measure_name() const {
	return m_mes->get_name();
}

/**
 * @brief Получение связанной Метрики
 * 
 * @return связанная Метрика
 */
const Measure* const Fact::get_Measure() const {
	return m_mes;
}