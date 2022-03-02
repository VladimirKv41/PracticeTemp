#pragma once
#include <string>

class Dimension;
class DimensionPosition;
class Fact;

/**
 * @brief ТочкаДанных.
 * 
 * Класс связывающий Измерение и Факт для дальнейшего взаимодействия.
 */
class DataPoint {
public:
	DataPoint(Fact* a_fact, Dimension* a_dim, DimensionPosition* a_dim_position);

	// Получение связанного Измерения
	const Dimension* const get_Dimension() const;

	// Получение связанного Факта
	Fact* const get_Fact() const;

	// Получение позиции в связанном Измерении
	const std::string& get_dim_position_name() const;
	
private:
	// Связанные Факт/Измерение/Позиция в Измерении
	Fact* m_fact;
	Dimension* m_dim;
	DimensionPosition* m_dim_position;
};