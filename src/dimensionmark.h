#ifndef DIMENSION_MARK_H
#define DIMENSION_MARK_H
#include <string>
#include <map>

class Dimension;
class Metric;
class FactClassifier;

/**
 * @brief Отметка на Измерении.
 *
 * Класс хранящий Классификаторы Фактов Отметок на Измерении.
 */
class DimensionMark {
public:

	DimensionMark(const std::string& a_name);

	// Добавление связанного Классификатора Факта
	void pushFactClassifier(const std::string& a_meas, FactClassifier* a_point);

	// Получение названия Отметки на Измерении
	const std::string& name() const;

	// Получение массива связанных Классификаторов Фактов
	const std::multimap<std::string, FactClassifier*>& FactClassifiers() const;

private:

	// Название Отметки
	std::string m_name;
	// Массив связанных Классификаторов Фактов
	// Ключ: название Метрики
	std::multimap<std::string, FactClassifier*> m_classifiers;
};

#endif