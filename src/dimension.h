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

	// Добавление связанного Классификатора Факта
	void pushFactClassifier(FactClassifier* a_point);

	// Получение вектора связанных Классификатор Фактов
	const std::vector<FactClassifier*>& FactClassifiers() const;

	// Получение Отметки на Измерении
	DimensionMark* const dimensionMark(const std::string& a_mark);

	// Поиск Отметки на Измерении
	bool searchMark(const std::string& a_mark) const;

	// Получение названия Измерения
	const std::string& name() const;

	// Получение массива Отметок на Измерении
	const std::map<std::string, DimensionMark*>& Marks() const;

	~Dimension();

private:
	// Массив Отметок на Измерении
	// Ключ: название Отметок
	std::map<std::string, DimensionMark*> m_marks_map;
	// Название Измерения
	std::string m_name;
	// Вектор связанных Классификаторов Фактов
	std::vector<FactClassifier*> m_classifiers;
};

#endif