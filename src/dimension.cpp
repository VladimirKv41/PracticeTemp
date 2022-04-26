#include "dimension.h"
#include "dimensionmark.h"

/**
 * @brief Конструктор.
 *
 * @param [in] a_name Название Измерения
 */
Dimension::Dimension(const std::string& a_name) : m_name(a_name) {

}

/**
 * @brief Добавление связанного Классификатора Факта.
 *
 * @param [in,out] a_point Классификатор Факта
 */
void Dimension::pushFactClassifier(FactClassifier* a_point) {
	m_classifiers.push_back(a_point);
}

/**
 * @brief Получение вектора связанных Классификаторов Фактов.
 *
 * @return Вектор всех Классификаторов Фактов связанных с этим Измерением
 */
const std::vector<FactClassifier*>& Dimension::FactClassifiers() const {
	return m_classifiers;
}

/**
 * @brief Получение Отметки на Измерении.
 *
 * Если Отметка не существует, то сначала создаётся.
 *
 * @param [in] a_mark Название Отметки
 * @return Отметка
 */
DimensionMark* const Dimension::dimensionMark(const std::string& a_mark) {
	if (m_marks_map.find(a_mark) == m_marks_map.end()) {
		m_marks_map[a_mark] = new DimensionMark(a_mark);
		return m_marks_map[a_mark];
	}
	else
		return m_marks_map[a_mark];
}

/**
 * @brief Поиск Отметки на Измерении.
 *
 * @param [in] a_mark Название Отметки
 * @return Существует Отметка или нет
 */
bool Dimension::searchMark(const std::string& a_mark) const {
	if (m_marks_map.find(a_mark) == m_marks_map.end()) {
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
const std::string& Dimension::name() const {
	return m_name;
}

/**
 * @brief Получение массива Отметок на Измерении.
 *
 * @return Массив Отметок на Измерении
 */
const std::map<std::string, DimensionMark*>& Dimension::Marks() const {
	return m_marks_map;
}

Dimension::~Dimension() {
	for (auto it = m_marks_map.begin(); it != m_marks_map.end(); it++) {
		delete it->second;
	}
}
