#ifndef FACT_CLASSIFIER_H
#define FACT_CLASSIFIER_H
#include <string>

class Dimension;
class DimensionMark;
class Fact;

/**
 * @brief Классификатор Факта.
 *
 * Класс, определяющий для Факта Отметки на Измерениях.
 * Организует взаимодействие Факта и Измерений,
 * служит для организации Выборок.
 */
class FactClassifier {
public:
	FactClassifier(Fact* a_fact, Dimension* a_dim, DimensionMark* a_dim_mark);

	// Получение связанного Измерения
	const Dimension* const getDimension() const;

	// Получение связанного Факта
	Fact* const getFact() const;

	// Получение названия Отметки на Измерении
	const std::string& getDimMarkName() const;

private:
	// Связанные Факт/Измерение/Отметка в Измерении
	Fact* m_fact;
	Dimension* m_dim;
	DimensionMark* m_dim_mark;
};

#endif