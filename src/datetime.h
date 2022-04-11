#ifndef DATETIME_H
#define DATETIME_H

// структура, объедин€юща€ дату и врем€
struct DateTime {
	Date date;
	Time time;
	friend bool operator== (const DateTime& a_datetime_1, const DateTime& a_datetime_2) {
		return (a_datetime_1.date == a_datetime_2.date && a_datetime_1.time == a_datetime_2.time);
	}
	friend bool operator!= (const DateTime& a_datetime_1, const DateTime& a_datetime_2) {
		return !(a_datetime_1 == a_datetime_2);
	}
};

#endif
