#pragma once
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
		const std::string& get_name() const;

	private:
		// Название метрики
		std::string m_name;
};