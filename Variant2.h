#pragma once
#include <iostream>

class Time;
class Date;
struct DateTime;

/**
 * @brief Класс типизации.
 *
 * Класс для хранения значений различных типов данных Фактов.
 */
template <typename T>
class Variant {
public:

	Variant(T a_var);

	const std::string& to_String() const;

	~Variant();

private:
	T m_value;
};