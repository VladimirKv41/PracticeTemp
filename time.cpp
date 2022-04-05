#include "time.h"

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
uint16_t Time::gethours() const {
	return m_hours;
}

/**
 * @brief Возращение минут из временной отметки.
 *
 * @return Количество минут
 */
uint8_t Time::getminutes() const {
	return m_minutes;
}

/**
 * @brief Возращение секунд из временной отметки.
 *
 * @return Количество секунд
 */
uint8_t Time::getseconds() const {
	return m_seconds;
}