#include "DimensionPosition.h"

/**
 * @brief Конструктор.
 *
 * @param [in,out] a_name Название отметки на измерении
 */
DimensionPosition::DimensionPosition(const std::string& a_name) : m_name(a_name){
	
}

/**
 * @brief Добавление связанной точки данных.
 * 
 * @param [in,out] a_meas Название метрики
 * @param [in,out] a_point Точка данных
 */
void DimensionPosition::push_DataPoint(const std::string& a_meas, DataPoint* a_point){
	m_points.insert({ a_meas,a_point });
}

/**
 * @brief Получение названия отметки на измерении.
 * 
 * @return Название отметки на измерении
 */
const std::string& DimensionPosition::get_name() const {
	return m_name;
}

/**
 * @brief Получение массива связанных точек данных.
 *
 * @return Массив связанных точек данных
 */
const std::unordered_multimap<std::string, DataPoint*>& DimensionPosition::get_DataPoints() const{
	return m_points;
}