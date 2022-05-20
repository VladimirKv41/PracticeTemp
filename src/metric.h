#ifndef METRIC_H
#define METRIC_H
#include <string>

/**
 * @brief Метрика.
 *
 * Класс хранящий название Метрики некоторых Фактов.
 */
	class Metric {
	public:

		Metric(const std::string& a_name);

		// Получение название Метрики
		const std::string& name() const;

	private:
		// Название Метрики
		std::string m_name;
};

#endif