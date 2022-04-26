#include "date.h"

/**
 * @brief Конструктор.
 *
 * @param [in] a_day День
 * @param [in] a_month Месяц
 * @param [in] a_year Год
 */
Date::Date(uint8_t a_day, uint8_t a_month, uint16_t a_year): m_day(a_day), m_month(a_month), m_year(a_year) {
	uint8_t leap_year = (1 - (m_year % 4 + 2 / m_year % 4 + 1))
		* (m_year % 100 + 2 / m_year % 100 + 1)
		* (1 - (m_year % 400 + 2 / m_year % 400 + 1));
	uint8_t days_in_month = 28 + (m_month + m_month / 8) % 2 + 2 % m_month
		+ 2 * (1 / m_month) + (1 + leap_year) / m_month + 1 / m_month - leap_year / m_month;
	if (m_day > days_in_month)
		m_day = days_in_month;
	if (m_day == 0)
		m_day = 1;
	if (m_month > 12 || m_month == 0)
		m_month = 1;
	if (m_year == 0 || m_year > 2100)
		m_year = 1950;
}

/**
 * @brief Возращение года из даты.
 *
 * @return Год
 */
uint16_t Date::year() const {
	return m_year;
}

/**
 * @brief Возращение месяца из даты.
 *
 * @return Месяц
 */
uint8_t Date::month() const {
	return m_month;
}

/**
 * @brief Возращение дня из даты.
 *
 * @return День
 */
uint8_t Date::day() const {
	return m_day;
}

/**
 * @brief Перегрузка оператора сравнения == для класса Date.
 *
 */
bool Date::operator== (const Date& a_other) const
{
	return m_year == a_other.m_year
		&& m_month == a_other.m_month
		&& m_day == a_other.m_day;
}

/**
 * @brief Перегрузка оператора сравнения != для класса Date.
 *
 */
bool Date::operator!= (const Date& a_other) const
{
	return !(*this == a_other);
}