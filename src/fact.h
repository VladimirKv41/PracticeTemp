#ifndef FACT_H
#define FACT_H
#include <vector>
#include <string>

class Dimension;
class Metric;
class FactClassifier;

/**
 * @brief Факт.
 *
 * Класс хранящий значение и его метрику.
 */
class Fact {
public:

	Fact(double a_value, Metric* a_mes);

	// Добавление связанной точки данных
	void push_FactClassifier(FactClassifier* a_point);

	// Получение вектора связанных точек данных
	const std::vector<FactClassifier*>& get_FactClassifiers() const;

	// Получение значения факта
	double get_value() const;

	// Получение названия метрики факта
	const std::string& get_Metric_name() const;

	// Получение связанной метрики
	const Metric* const get_Metric() const;

private:
	// Значение факта
	double m_value;
	// Связанная метрика
	Metric* m_mes;
	// Вектор связанных точек данных
	std::vector<FactClassifier*> m_points;
};

#endif