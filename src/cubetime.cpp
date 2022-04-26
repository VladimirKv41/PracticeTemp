#include "cubetime.h"

/**
 * @brief Конструктор.
 *
 * @param [in] a_hours Часы
 * @param [in] a_minutes Минуты
 * @param [in] a_seconds Секунды
 */
Time::Time(uint8_t a_hours, uint8_t a_minutes, uint8_t a_seconds) : m_hours(a_hours), m_minutes(a_minutes), m_seconds(a_seconds) {
	if (m_hours > 23)
		m_hours = 23;
	if (m_minutes > 59)
		m_minutes = 59;
	if (m_seconds > 59)
		m_seconds = 59;
}

/**
 * @brief Возращение часов из временной отметки.
 * 

 * @return Количество часов
 */
uint16_t Time::hours() const {
	return m_hours;
}

/**
 * @brief Возращение минут из временной отметки.
 *
 * @return Количество минут
 */
uint8_t Time::minutes() const {
	return m_minutes;
}

/**
 * @brief Возращение секунд из временной отметки.
 *
 * @return Количество секунд
 */
uint8_t Time::seconds() const {
	return m_seconds;
}

/**
 * @brief Перегрузка оператора сравнения == для класса Time.
 *
 */
bool Time::operator== (const Time& a_other) const
{
	return m_hours == a_other.m_hours
		&& m_minutes == a_other.m_minutes
		&& m_seconds == a_other.m_seconds;
}

/**
 * @brief Перегрузка оператора сравнения != для класса Time.
 *
 */
bool Time::operator!= (const Time& a_other) const
{
	return !(*this == a_other);
}