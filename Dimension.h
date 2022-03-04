#pragma once
#include <string>
#include <vector>
#include <map>

class DataPoint;
class DimensionPosition;

/**
 * @brief Измерение.
 * 
 * Класс для хранения и работы с одной из "осей" куба.
 */
class Dimension {
public:

	Dimension(const std::string &a_name);

	// Добавление связанной точки данных
	void push_DataPoint(DataPoint* a_point);

	// Получение вектора связанных точек данных
	const std::vector<DataPoint*>& get_DataPoints() const;

	// Получение отметки в измерении
	DimensionPosition* const get_DimensionPosition(const std::string &a_mark);

	// Поиск отметки в измерении
	bool search_mark(const std::string &a_mark) const;

	// Получение названия измерения
	const std::string& get_name() const;

	// Получение массива отметок на измерении
	const std::map<std::string, DimensionPosition*>& get_positions() const;

	~Dimension();
	
private:
	// Массив отметок на измерении
	// Ключ: название отметок
	std::map<std::string,DimensionPosition*> m_positions_map;
	// Название измерения
	std::string m_name;
	// Вектор связанных точек данных
	std::vector<DataPoint*> m_points;
};