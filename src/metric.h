#ifndef METRIC_H
#define METRIC_H
#include <string>

/**
 * @brief Метрика.
 *
 * Класс хранящий название метрики некоторых фактов.
 */
	class Metric {
	public:

		Metric(const std::string& a_name);

		// Получение название метрики
		const std::string& name() const;

	private:
		// Название метрики
		std::string m_name;
};

#endif