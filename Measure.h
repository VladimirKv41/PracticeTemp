#pragma once
#include <string>

/**
 * @brief Метрика
 * 
 * Класс хранящий название Метрики некоторых Фактов
 */
class Measure {
public:

	Measure(const std::string &a_name);

	// Получение название Метрики
	const std::string& get_name() const;

private:
	// Название Метрики
	std::string m_name;
};