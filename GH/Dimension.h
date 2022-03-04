#pragma once
#include <string>
#include <vector>

class DataPoint;

// Измерение
/**
 * @brief Измерение
 * 
 * Класс для хранения и работы с одной из "осей" Куба
 */
class Dimension {
public:

	Dimension(const std::string &a_name);

	// Добавление связанной ТочкиДанных
	void push_DataPoint(DataPoint* a_point);

	// Получение вектора связанных ТочекДанных
	const std::vector<DataPoint*>& get_DataPoints() const;

	// Получение индекса позиции в Измерении
	uint32_t get_mark(const std::string &a_mark);

	// Поиск позиции в Измерении
	bool search_mark(const std::string &a_mark) const;

	// Получение названия Измерения
	const std::string& get_name() const;

	// Получение вектора позиций Измерения
	const std::vector<std::string>& get_positions() const;
	
private:
	// Вектор позиций Измерения
	std::vector<std::string> m_vector;
	// Название Измерения
	std::string m_name;
	// Вектор связанных ТочекДанных
	std::vector<DataPoint*> m_points;
};