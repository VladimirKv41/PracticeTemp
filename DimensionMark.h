#pragma once
#include <string>
#include <unordered_map>

class Dimension;
class Metric;
class FactClassifier;

/**
 * @brief Отметка (на измерении).
 *
 * Класс хранящий точки данных отметок на измерении и их метрику.
 */
class DimensionMark {
public:

	DimensionMark(const std::string& a_name);

	// Добавление связанной точки данных
	void push_FactClassifier(const std::string& a_meas, FactClassifier* a_point);

	// Получение названия отметки на измерении
	const std::string& get_name() const;

	// Получение массива связанных точек данных
	const std::unordered_multimap<std::string, FactClassifier*>& get_FactClassifiers() const;

private:

	// Название отметки
	std::string m_name;
	// Массив связанных точек данных
	// Ключ: название метрики
	std::unordered_multimap<std::string, FactClassifier*> m_points;
};