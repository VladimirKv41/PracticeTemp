#ifndef TIME_H
#define TIME_H
#include <string>

/**
 * @brief  Класс Время.
 *
 *  Класс хранящий временную отметку (часы-минуты-секунды).
 */
class Time {
public:

	Time(uint8_t a_hours, uint8_t a_minutes, uint8_t a_seconds);

	// Возращение часов из временной отметки
	uint16_t hours() const;

	// Возращение минут из временной отметки
	uint8_t minutes() const;

	// Возращение секунд из временной отметки
	uint8_t seconds() const;

	// Перегрузка оператора сравнения == для класса Time.
	friend bool operator== (const Time& a_time_1, const Time& a_time_2);

	// Перегрузка оператора сравнения == для класса Time.
	friend bool operator!= (const Time& a_time_1, const Time& a_time_2);

private:
	// Количество часов, минут, секунд
	uint8_t m_hours;
	uint8_t m_minutes;
	uint8_t m_seconds;
};

#endif