#pragma once
#include <string>

class Dimension;
class Fact;

// ТочкаДанных
/**
 * @brief ТочкаДанных
 * 
 * Класс связывающий Измерение и Факт для дальнейшего взаимодействия
 */
class DataPoint {
public:
	DataPoint(Fact* a_fact, Dimension* a_dim, uint32_t a_position);

	// Получение связанного Измерения
	const Dimension* const get_Dimension() const;

	// Получение связанного Факта
	const Fact* const get_Fact() const;

	// Получение позиции в связанном Измерении
	const std::string& get_dim_position_name() const;

	// Получение индекса позиции в связанном Измерении
	uint32_t get_dim_position_index() const;
	
private:
	// Связанные Факт/Измерение
	Fact* m_fact;
	Dimension* m_dim;
	// Индекс позиции в связанном Измерении
	uint32_t m_position_index;
};