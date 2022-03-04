#pragma once
#include <string>

class Dimension;
class DimensionPosition;
class Fact;

/**
 * @brief Точка данных.
 * 
 * Класс связывающий измерение и факт для дальнейшего взаимодействия.
 */
class DataPoint {
public:
	DataPoint(Fact* a_fact, Dimension* a_dim, DimensionPosition* a_dim_position);

	// Получение связанного измерения
	const Dimension* const get_Dimension() const;

	// Получение связанного факта
	Fact* const get_Fact() const;

	// Получение отметки в связанном измерении
	const std::string& get_dim_position_name() const;
	
private:
	// Связанные факт/измерение/отметка в измерении
	Fact* m_fact;
	Dimension* m_dim;
	DimensionPosition* m_dim_position;
};