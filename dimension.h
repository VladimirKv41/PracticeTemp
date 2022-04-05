#ifndef DIMENSION_H
#define DIMENSION_H
#include <string>
#include <vector>
#include <map>

class FactClassifier;
class DimensionMark;

/**
 * @brief Измерение.
 *
 * Класс для хранения и работы с одной из "осей" куба.
 */
class Dimension {
public:

	Dimension(const std::string& a_name);

	// Добавление связанной точки данных
	void push_FactClassifier(FactClassifier* a_point);

	// Получение вектора связанных точек данных
	const std::vector<FactClassifier*>& get_FactClassifiers() const;

	// Получение отметки в измерении
	DimensionMark* const get_DimensionPosition(const std::string& a_mark);

	// Поиск отметки в измерении
	bool search_mark(const std::string& a_mark) const;

	// Получение названия измерения
	const std::string& get_name() const;

	// Получение массива отметок на измерении
	const std::map<std::string, DimensionMark*>& get_marks() const;

	~Dimension();

private:
	// Массив отметок на измерении
	// Ключ: название отметок
	std::map<std::string, DimensionMark*> m_marks_map;
	// Название измерения
	std::string m_name;
	// Вектор связанных точек данных
	std::vector<FactClassifier*> m_points;
};

#endif