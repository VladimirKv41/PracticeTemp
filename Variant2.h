#pragma once
#include <iostream>

class Time;
class Date;
struct DateTime;

/**
 * @brief  ласс типизации.
 *
 *  ласс дл¤ хранени¤ значений различных типов данных ‘актов.
 */
template <typename T>
class Variant {
public:

	Variant(T a_var);

	const std::string& to_String() const;

	const T& get_value() const;

	~Variant();

private:
	T m_value;
};