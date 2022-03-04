#pragma once
#include <vector>
#include <string>

class Dimension;
class Measure;
class DataPoint;

/**
 * @brief Факт
 * 
 * Класс хранящий значение и его Метрику
 */
class Fact {
public:

	Fact(double a_value,Measure* a_mes);

	// Добавление связанной ТочкиДанных
	void push_DataPoint(DataPoint* a_point);

	// Получение вектора связанных ТочекДанных
	const std::vector<DataPoint*>& get_DataPoints() const;

	// Получение значения Факта
	double get_value() const;

	// Получение названия Метрики Факта
	const std::string& get_Measure_name() const;

	// Получение связанной Метрики
	const Measure* const get_Measure() const;

private:
	// Значение Факта
	double m_value;
	// Связанная Метрика
	Measure* m_mes;
	// Вектор связанных ТочекДанных
	std::vector<DataPoint*> m_points;
};