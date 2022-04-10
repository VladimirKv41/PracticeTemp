#include "variant.h"
#include "time.h"
#include "date.h"
#include "datetime.h"




/**
 * @brief Конструктор для знакового 64-битного целого.
 *
 * @param [in] a_var Значение типа данных
 */
Variant::Variant(std::int64_t a_var) : m_var_type(var_type::INT64), m_pointer_to_value(new int64_t(a_var)) {

}

/**
 * @brief Конструктор для беззнакового 64-битного целого.
 *
 * @param [in] a_var Значение типа данных
 */
Variant::Variant(std::uint64_t a_var) : m_var_type(var_type::UINT64), m_pointer_to_value(new uint64_t(a_var)) {

}

/**
 * @brief Конструктор для логического выражения.
 *
 * @param [in] a_var Значение типа данных
 */
Variant::Variant(bool a_var) : m_var_type(var_type::BOOL), m_pointer_to_value(new bool(a_var)) {

}

/**
 * @brief Конструктор для длинного вещественного.
 *
 * @param [in] a_var Значение типа данных
 */
Variant::Variant(long double a_var) : m_var_type(var_type::LDOUBLE), m_pointer_to_value(new long double(a_var)) {

}

/**
 * @brief Конструктор для символов.
 *
 * @param [in] a_var Значение типа данных
 */
Variant::Variant(char a_var) : m_var_type(var_type::CHAR), m_pointer_to_value(new char(a_var)) {

}

/**
 * @brief Конструктор для строк.
 *
 * @param [in] a_var Значение типа данных
 */
Variant::Variant(const std::string& a_var) : m_var_type(var_type::STRING), m_pointer_to_value(new std::string(a_var)) {
	
}

/**
 * @brief Конструктор для широких(Unicode) строк.
 *
 * @param [in] a_var Значение типа данных
 */
Variant::Variant(const std::wstring& a_var) : m_var_type(var_type::WSTRING), m_pointer_to_value(new std::wstring(a_var)) {

}

/**
 * @brief Конструктор для класса Time.
 *
 * @param [in] a_var Значение типа данных
 */
Variant::Variant(Time a_var) : m_var_type(var_type::TIME), m_pointer_to_value(new Time(a_var)) {

}

/**
 * @brief Конструктор для класса Date.
 *
 * @param [in] a_var Значение типа данных
 */
Variant::Variant(Date a_var) : m_var_type(var_type::DATE), m_pointer_to_value(new Date(a_var)) {

}

/**
 * @brief Конструктор для структуры DateTime.
 *
 * @param [in] a_var Значение типа данных
 */
Variant::Variant(DateTime a_var) : m_var_type(var_type::DATETIME), m_pointer_to_value(new DateTime(a_var)) {

}

/**
 * @brief Возвращение значения типа данных.
 *
 * @return Значения типа данных
 */
template <typename T>
T Variant::getvalue() const {
	return *static_cast<T*>(m_pointer_to_value);
}

template<>
uint64_t Variant::getvalue<uint64_t>() const {
	return *static_cast<uint64_t*>(m_pointer_to_value);
}

template<>
int64_t Variant::getvalue<int64_t>() const {
	return *static_cast<int64_t*>(m_pointer_to_value);
}

template<>
bool Variant::getvalue<bool>() const {
	return *static_cast<bool*>(m_pointer_to_value);
}

template<>
long double Variant::getvalue<long double>() const {
	return *static_cast<long double*>(m_pointer_to_value);
}

template<>
char Variant::getvalue<char>() const {
	return *static_cast<char*>(m_pointer_to_value);
}

template<>
const std::string& Variant::getvalue<const std::string&>() const {
	return *static_cast<std::string*>(m_pointer_to_value);
}

template<>
const std::wstring& Variant::getvalue<const std::wstring&>() const {
	return *static_cast<std::wstring*>(m_pointer_to_value);
}

template<>
const Date& Variant::getvalue<const Date&>() const {
	return *static_cast<Date*>(m_pointer_to_value);
}

template<>
const Time& Variant::getvalue<const Time&>() const {
	return *static_cast<Time*>(m_pointer_to_value);
}

template<>
const DateTime& Variant::getvalue<const DateTime&>() const {
	return *static_cast<DateTime*>(m_pointer_to_value);
}

/**
 * @brief Возвращение типа данных.
 * 
 * @return enum class var_type Тип данных
 */
var_type Variant::getvaluetype() const {
	return m_var_type;
}

/**
 * @brief Перегрузка оператора сравнения == для класса Variant.
 *
 */
bool operator== (const Variant& a_variant_1, const Variant& a_variant_2)
{
	if (a_variant_1.getvaluetype() == a_variant_2.getvaluetype()) {
		if(a_variant_1.getvaluetype() == var_type::INT64)
			return a_variant_1.getvalue<int64_t>() == a_variant_2.getvalue<int64_t>();
		if (a_variant_1.getvaluetype() == var_type::UINT64)
			return a_variant_1.getvalue<uint64_t>() == a_variant_2.getvalue<uint64_t>();
		if (a_variant_1.getvaluetype() == var_type::BOOL)
			return a_variant_1.getvalue<bool>() == a_variant_2.getvalue<bool>();
		if (a_variant_1.getvaluetype() == var_type::LDOUBLE)
			return a_variant_1.getvalue<long double>() == a_variant_2.getvalue<long double>();
		if (a_variant_1.getvaluetype() == var_type::CHAR)
			return a_variant_1.getvalue<char>() == a_variant_2.getvalue<char>();
		if (a_variant_1.getvaluetype() == var_type::STRING)
			return a_variant_1.getvalue<std::string>() == a_variant_2.getvalue<std::string>();
		if (a_variant_1.getvaluetype() == var_type::WSTRING)
			return a_variant_1.getvalue<std::wstring>() == a_variant_2.getvalue<std::wstring>();
		if (a_variant_1.getvaluetype() == var_type::DATE)
			return a_variant_1.getvalue<Date>() == a_variant_2.getvalue<Date>();
		if (a_variant_1.getvaluetype() == var_type::TIME)
			return a_variant_1.getvalue<Time>() == a_variant_2.getvalue<Time>();
		if (a_variant_1.getvaluetype() == var_type::DATETIME)
			return a_variant_1.getvalue<DateTime>() == a_variant_2.getvalue<DateTime>();

	}
}

/**
 * @brief Перегрузка оператора сравнения == для класса Variant.
 *
 */
Variant& Variant::operator= (const Variant& a_variant)
{
	if (this == &a_variant)
		return *this;
	m_var_type = a_variant.m_var_type;
	m_pointer_to_value = a_variant.m_pointer_to_value;
	return *this;
}

/**
 * @brief Деструктор.
 */
Variant::~Variant() {
	delete m_pointer_to_value;
}
