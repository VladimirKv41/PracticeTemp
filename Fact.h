#pragma once
#include <vector>
#include <string>

class Dimension;
class Measure;
class DataPoint;

/**
 * @brief Факт.
 * 
 * Класс хранящий значение и его метрику.
 */
class Fact {
public:

	Fact(double a_value,Measure* a_mes);

	// Добавление связанной точки данных
	void push_DataPoint(DataPoint* a_point);

	// Получение вектора связанных точек данных
	const std::vector<DataPoint*>& get_DataPoints() const;

	// Получение значения факта
	double get_value() const;

	// Получение названия метрики факта
	const std::string& get_Measure_name() const;

	// Получение связанной метрики
	const Measure* const get_Measure() const;

private:
	// Значение факта
	double m_value;
	// Связанная метрика
	Measure* m_mes;
	// Вектор связанных точек данных
	std::vector<DataPoint*> m_points;
};