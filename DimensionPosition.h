#pragma once
#include <string>
#include <unordered_map>

class Dimension;
class Measure;
class DataPoint;

/**
 * @brief Позиция (на Измерении).
 *
 * Класс хранящий ТочкиДанных Позиции на Измерении и их Метрику.
 */
class DimensionPosition {
public:

	DimensionPosition(const std::string& a_name);

	// Добавление связанной ТочкиДанных
	void push_DataPoint(const std::string& a_meas, DataPoint* a_point);

	// Получение названия Позиции на Измерении
	const std::string& get_name() const;

	// Получение ассоциативного массива связанных ТочекДанных
	const std::unordered_multimap<std::string, DataPoint*>& get_DataPoints() const;

private:

	// Название Позиции
	std::string m_name;
	// Ассоциативный массив связанных ТочекДанных
	// Ключ: название Метрики
	std::unordered_multimap<std::string, DataPoint*> m_points;
};