#ifndef VARIANT_H
#define VARIANT_H
#include <iostream>
#include <string>

class Date;
class Time;
struct DateTime;
/**
 * @brief Тип данных.
 */
enum class var_type {
	VOID,
	INT64,
	UINT64,
	BOOL,
	LDOUBLE,
	CHAR,
	STRING,
	WSTRING,
	TIME,
	DATE,
	DATETIME
};

/**
 * @brief  Класс типизации.
 *
 *  Класс для хранения значений различных типов данных Фактов.
 */
class Variant {
public:

	Variant(void* a_var);

	Variant(std::int64_t a_var);

	Variant(std::uint64_t a_var);

	Variant(bool a_var);

	Variant(long double a_var);

	Variant(char a_var);

	Variant(const std::string& a_var);

	Variant(const std::wstring& a_var);

	Variant(Time a_var);

	Variant(Date a_var);

	Variant(DateTime a_var);

	// Конструктор копирования
	Variant(const Variant& a_var);

	// Возвращение значения типа данных.
	template <typename T>
	T value() const;

	// Возвращение типа данных.
	var_type valueType() const;

	// Перегрузка оператора сравнения == для класса Variant.
	bool operator== (const Variant& a_other) const;

	// Перегрузка оператора сравнения != для класса Variant.
	bool operator!= (const Variant& a_other) const;

	// Перегрузка оператора присваивания = для класса Variant.
	Variant& operator= (const Variant& a_variant);

	~Variant();

private:

	// Идентификатор выбранного (при инициализации) типа данныха (перечислитель)
	// нужен, чтобы идентифицировать тип данных, и затем вернуть значение
	var_type m_var_type;
	// Указатель на хранимое значение типа данных
	// без указания конкретного типа
	void* m_pointer_to_value;
};

#endif
