#include "Dimension.h"
#include "DimensionPosition.h"

/**
 * @brief Конструктор.
 * 
 * @param [in] a_name Название измерения
 */
Dimension::Dimension(const std::string &a_name) : m_name(a_name) {
	
}

/**
 * @brief Добавление связанной ТочкиДанных.
 * 
 * @param [in,out] a_point ТочкаДанных
 */
void Dimension::push_DataPoint(DataPoint* a_point) {
	m_points.push_back(a_point);
}

/**
 * @brief Получение вектора связанных ТочекДанных.
 * 
 * @return Вектор всех ТочекДанных связанных с этим Измерением
 */
const std::vector<DataPoint*>& Dimension::get_DataPoints() const {
	return m_points;
}

/**
 * @brief Получение Позиции на Измерении.
 * 
 * Если Позиция не существует, то сначала создаётся.
 * 
 * @param [in] a_mark Позиция
 * @return Позиция
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
 * @brief Поиск позиции в Измерении.
 * 
 * @param [in] a_mark Позиция
 * @return Существует позиция или нет
 */
bool Dimension::search_mark(const std::string &a_mark) const {
	if (m_positions_map.find(a_mark) == m_positions_map.end()) {
		return false;
	}
	else
		return true;
}

/**
 * @brief Получение названия Измерения.
 * 
 * @return Название Измерения
 */
const std::string& Dimension::get_name() const {
	return m_name;
}

/**
 * @brief Получение ассоциативного массива Позиций на Измерении.
 * 
 * @return Ассоциативного массив Позиций на Измерении
 */
const std::map<std::string, DimensionPosition*>& Dimension::get_positions() const {
	return m_positions_map;
}

Dimension::~Dimension(){
	for (class std::map<std::string, DimensionPosition*>::iterator it = m_positions_map.begin(); it != m_positions_map.end(); it++) {
		delete it->second;
	}
}
