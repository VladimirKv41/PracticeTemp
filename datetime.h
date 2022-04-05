#ifndef DATETIME_H
#define DATETIME_H

// структура, объедин€юща€ дату и врем€
struct DateTime {
	Date date;
	Time time;
	bool operator== (const DateTime& a_datetime) {
		return (date == a_datetime.date && time == a_datetime.time);
	}
};

#endif
