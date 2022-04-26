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
 * Класс хранящий значение и его Метрику.
 */
class Fact {
public:

	Fact(double a_value, Metric* a_mes);

	// Добавление связанного Классификатора Факта
	void pushFactClassifier(FactClassifier* a_point);

	// Получение вектора связанных Классификаторов Факта
	const std::vector<FactClassifier*>& FactClassifiers() const;

	// Получение значения Факта
	double value() const;

	// Получение названия Метрики Факта
	const std::string& MetricName() const;

	// Получение связанной Метрики
	const Metric* const metric() const;

private:
	// Значение Факта
	double m_value;
	// Связанная Метрика
	Metric* m_mes;
	// Вектор связанных Классификаторов Факта
	std::vector<FactClassifier*> m_classifiers;
};

#endif