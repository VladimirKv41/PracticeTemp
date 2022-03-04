#include "Dimension.h"

/**
 * @brief Конструктор
 * 
 * @param [in] a_name Название измерения
 */
Dimension::Dimension(const std::string &a_name) : m_name(a_name) {
	
}

/**
 * @brief Добавление связанной ТочкиДанных
 * 
 * @param [in,out] a_point ТочкаДанных
 */
void Dimension::push_DataPoint(DataPoint* a_point) {
	m_points.push_back(a_point);
}

/**
 * @brief Получение вектора связанных ТочекДанных
 * 
 * @return Вектор всех ТочекДанных связанных с этим Измерением
 */
const std::vector<DataPoint*>& Dimension::get_DataPoints() const {
	return m_points;
}

/**
 * @brief Получение индекса позиции в Измерении
 * 
 * Если позиция не существует, то сначала создаётся
 * 
 * @param [in] a_mark Позиция
 * @return Индекс позиции
 */
uint32_t Dimension::get_mark(const std::string &a_mark) {
	if (std::find(m_vector.begin(), m_vector.end(), a_mark) == m_vector.end()) {
		m_vector.push_back(a_mark);
		return m_vector.end() - m_vector.begin() - 1;
	}
	else 
		return std::find(m_vector.begin(), m_vector.end(), a_mark) - m_vector.begin();
}

/**
 * @brief Поиск позиции в Измерении
 * 
 * @param [in] a_mark Позиция
 * @return Существует позиция или нет
 */
bool Dimension::search_mark(const std::string &a_mark) const {
	if (std::find(m_vector.begin(), m_vector.end(), a_mark) == m_vector.end()) {
		return false;
	}
	else
		return true;
}

/**
 * @brief Получение названия Измерения
 * 
 * @return Название Измерения
 */
const std::string& Dimension::get_name() const {
	return m_name;
}

/**
 * @brief Получение вектора позиций Измерения
 * 
 * @return Вектор позиций Измерения
 */
const std::vector<std::string>& Dimension::get_positions() const {
	return m_vector;
}