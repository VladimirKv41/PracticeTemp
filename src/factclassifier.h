#ifndef FACT_CLASSIFIER_H
#define FACT_CLASSIFIER_H
#include <string>

class Dimension;
class DimensionMark;
class Fact;

/**
 * @brief Точка данных.
 *
 * Класс, определяющий для Факта Отметки на Измерениях.
 * Организует взаимодействие Факта и Измерений,
 * служит для организации выборок.
 */
class FactClassifier {
public:
	FactClassifier(Fact* a_fact, Dimension* a_dim, DimensionMark* a_dim_mark);

	// Получение связанного измерения
	const Dimension* const get_Dimension() const;

	// Получение связанного факта
	Fact* const get_Fact() const;

	// Получение отметки в связанном измерении
	const std::string& get_dim_mark_name() const;

private:
	// Связанные факт/измерение/отметка в измерении
	Fact* m_fact;
	Dimension* m_dim;
	DimensionMark* m_dim_mark;
};

#endif