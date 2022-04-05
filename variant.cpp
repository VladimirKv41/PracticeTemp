#include "variant.h"
#include "time.h"
#include "date.h"

// структура, объединяющая дату и время
struct DateTime {
	Date date;
	Time time;
};

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
Variant::Variant(std::string& a_var) : m_var_type(var_type::STRING), m_pointer_to_value(new std::string(a_var)) {

}

/**
 * @brief Конструктор для широких(Unicode) строк.
 *
 * @param [in] a_var Значение типа данных
 */
Variant::Variant(std::wstring& a_var) : m_var_type(var_type::WSTRING), m_pointer_to_value(new std::wstring(a_var)) {

}

/**
 * @brief Конструктор для класса Time.
 *
 * @param [in] a_var Значение типа данных
 */
Variant::Variant(Time* a_var) : m_var_type(var_type::TIME), m_pointer_to_value(a_var) {

}

/**
 * @brief Конструктор для класса Date.
 *
 * @param [in] a_var Значение типа данных
 */
Variant::Variant(Date* a_var) : m_var_type(var_type::DATE), m_pointer_to_value(a_var) {

}

/**
 * @brief Конструктор для структуры DateTime.
 *
 * @param [in] a_var Значение типа данных
 */
Variant::Variant(DateTime* a_var) : m_var_type(var_type::DATETIME), m_pointer_to_value(a_var) {

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

		return false;
}