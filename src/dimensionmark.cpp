#include "dimensionmark.h"

/**
 * @brief Конструктор.
 *
 * @param [in,out] a_name Название Отметки на Измерении
 */
DimensionMark::DimensionMark(const std::string& a_name) : m_name(a_name) {

}

/**
 * @brief Добавление связанного Классификатора Факта.
 *
 * @param [in,out] a_meas Название Метрики
 * @param [in,out] a_point Классификатор Факта
 */
void DimensionMark::pushFactClassifier(const std::string& a_meas, FactClassifier* a_point) {
	m_classifiers.insert({ a_meas,a_point });
}

/**
 * @brief Получение названия Отметки на Измерении.
 *
 * @return Название Отметки на Измерении
 */
const std::string& DimensionMark::name() const {
	return m_name;
}

/**
 * @brief Получение массива связанных Классификаторов Фактов.
 *
 * @return Массив связанных Классификаторо Фактов
 */
const std::unordered_multimap<std::string, FactClassifier*>& DimensionMark::FactClassifiers() const {
	return m_classifiers;
}