#pragma once
#include <string>
#include <unordered_map>

class Dimension;
class Measure;
class DataPoint;

/**
 * @brief Отметка (на измерении).
 *
 * Класс хранящий точки данных отметок на измерении и их метрику.
 */
class DimensionPosition {
public:

	DimensionPosition(const std::string& a_name);

	// Добавление связанной точки данных
	void push_DataPoint(const std::string& a_meas, DataPoint* a_point);

	// Получение названия отметки на измерении
	const std::string& get_name() const;

	// Получение массива связанных точек данных
	const std::unordered_multimap<std::string, DataPoint*>& get_DataPoints() const;

private:

	// Название отметки
	std::string m_name;
	// Массив связанных точек данных
	// Ключ: название метрики
	std::unordered_multimap<std::string, DataPoint*> m_points;
};