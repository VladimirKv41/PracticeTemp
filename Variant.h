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
class Variant {
public:

	Variant(std::int64_t* a_var);

	Variant(std::uint64_t* a_var);

	Variant(bool* a_var);

	Variant(long double* a_var);

	Variant(char* a_var);

	Variant(const std::string& a_var);

	Variant(Time* a_var);

	Variant(Date* a_var);

	Variant(DateTime* a_var);

	const std::string& to_String() const;
	
	~Variant();

private:
	uint8_t m_type_id;
	void* m_value;
};