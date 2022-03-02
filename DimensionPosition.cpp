#include "DimensionPosition.h"

/**
 * @brief Конструктор.
 *
 * @param [in,out] a_name Название Позиции на Измерении
 */
DimensionPosition::DimensionPosition(const std::string& a_name) : m_name(a_name){
	
}

/**
 * @brief Добавление связанной ТочкиДанных.
 * 
 * @param [in,out] a_meas Название Метрики
 * @param [in,out] a_point ТочкаДанных
 */
void DimensionPosition::push_DataPoint(const std::string& a_meas, DataPoint* a_point){
	m_points.insert({ a_meas,a_point });
}

/**
 * @brief Получение названия Позиции на Измерении.
 * 
 * @return название Позиции на Измерении
 */
const std::string& DimensionPosition::get_name() const {
	return m_name;
}

/**
 * @brief Получение ассоциативного массива связанных ТочекДанных.
 *
 * @return Ассоциативный массив связанных ТочекДанных
 */
const std::unordered_multimap<std::string, DataPoint*>& DimensionPosition::get_DataPoints() const{
	return m_points;
}