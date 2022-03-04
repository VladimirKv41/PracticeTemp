﻿#include "Dimension.h"
#include "DimensionPosition.h"

/**
 * @brief Конструктор.
 * 
 * @param [in] a_name Название измерения
 */
Dimension::Dimension(const std::string &a_name) : m_name(a_name) {
	
}

/**
 * @brief Добавление связанной точки данных.
 * 
 * @param [in,out] a_point Точка данных
 */
void Dimension::push_DataPoint(DataPoint* a_point) {
	m_points.push_back(a_point);
}

/**
 * @brief Получение вектора связанных точек данных.
 * 
 * @return Вектор всех точек данных связанных с этим измерением
 */
const std::vector<DataPoint*>& Dimension::get_DataPoints() const {
	return m_points;
}

/**
 * @brief Получение отметок на измерении.
 * 
 * Если отметка не существует, то сначала создаётся.
 * 
 * @param [in] a_mark Название отметки
 * @return Отметка
 */
DimensionPosition* const Dimension::get_DimensionPosition(const std::string &a_mark) {
	if (m_positions_map.find(a_mark) == m_positions_map.end()) {
		m_positions_map[a_mark] = new DimensionPosition(a_mark);
		return m_positions_map[a_mark];
	}
	else 
		return m_positions_map[a_mark];
}

/**
 * @brief Поиск отметки в измерении.
 * 
 * @param [in] a_mark Название отметка
 * @return Существует отметка или нет
 */
bool Dimension::search_mark(const std::string &a_mark) const {
	if (m_positions_map.find(a_mark) == m_positions_map.end()) {
		return false;
	}
	else
		return true;
}

/**
 * @brief Получение названия измерения.
 * 
 * @return Название измерения
 */
const std::string& Dimension::get_name() const {
	return m_name;
}

/**
 * @brief Получение массива отметок на измерении.
 * 
 * @return Массив отметок на измерении
 */
const std::map<std::string, DimensionPosition*>& Dimension::get_positions() const {
	return m_positions_map;
}

Dimension::~Dimension(){
	for (auto it = m_positions_map.begin(); it != m_positions_map.end(); it++) {
		delete it->second;
	}
}
