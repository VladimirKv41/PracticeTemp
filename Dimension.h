#pragma once
#include <string>
#include <vector>
#include <map>

class DataPoint;
class DimensionPosition;

/**
 * @brief Измерение.
 * 
 * Класс для хранения и работы с одной из "осей" Куба.
 */
class Dimension {
public:

	Dimension(const std::string &a_name);

	// Добавление связанной ТочкиДанных
	void push_DataPoint(DataPoint* a_point);

	// Получение вектора связанных ТочекДанных
	const std::vector<DataPoint*>& get_DataPoints() const;

	// Получение Позиции в Измерении
	DimensionPosition* const get_DimensionPosition(const std::string &a_mark);

	// Поиск Позиции в Измерении
	bool search_mark(const std::string &a_mark) const;

	// Получение названия Измерения
	const std::string& get_name() const;

	// Получение ассоциативного массива Позиций на Измерении
	const std::map<std::string, DimensionPosition*>& get_positions() const;

	~Dimension();
	
private:
	// Ассоциативного массив Позиций на Измерении
	// Ключ: название Позиции
	std::map<std::string,DimensionPosition*> m_positions_map;
	// Название Измерения
	std::string m_name;
	// Вектор связанных ТочекДанных
	std::vector<DataPoint*> m_points;
};