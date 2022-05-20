#ifndef FACT_H
#define FACT_H
#include <vector>
#include <string>

class Dimension;
class Metric;
class FactClassifier;
class Variant;

/**
 * @brief Факт.
 *
 * Класс хранящий значение и его Метрику.
 */
class Fact {
public:

	Fact(Variant* a_value, Metric* a_mes);

	// Добавление связанного Классификатора Факта
	void pushFactClassifier(FactClassifier* a_point);

	// Получение вектора связанных Классификаторов Факта
	const std::vector<FactClassifier*>& FactClassifiers() const;

	// Получение значения Факта
	const Variant& value() const;

	// Получение названия Метрики Факта
	const std::string& MetricName() const;

	// Получение связанной Метрики
	const Metric* const metric() const;

	~Fact();

private:
	// Значение Факта
	Variant* m_value;
	// Связанная Метрика
	Metric* m_mes;
	// Вектор связанных Классификаторов Факта
	std::vector<FactClassifier*> m_classifiers;
};

#endif