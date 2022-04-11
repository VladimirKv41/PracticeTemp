#ifndef DATE_H
#define DATE_H
#include <cstdint>

/**
 * @brief  Класс Дата.
 *
 *  Класс хранящий дату (дни-месяцы-годы).
 */
class Date {
public:

	Date(uint8_t a_day, uint8_t a_month, uint16_t a_year);

	// Возращение года из даты.
	uint16_t getyear() const;

	// Возращение месяца из даты.
	uint8_t getmonth() const;

	// Возращение дня из даты.
	uint8_t getday() const;

	// Перегрузка оператора сравнения == для класса Date.
	friend bool operator== (const Date& a_date_1, const Date& a_date_2);

	// Перегрузка оператора сравнения != для класса Date.
	friend bool operator!= (const Date& a_date_1, const Date& a_date_2);

private:
	// Год, месяц, день
	uint16_t m_year;
	uint8_t m_month;
	uint8_t m_day;
};

#endif