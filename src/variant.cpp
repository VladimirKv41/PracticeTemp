#include "variant.h"
#include "cubetime.h"
#include "date.h"
#include "datetime.h"



/**
 * @brief Конструктор для пустого значения.
 *
 * @param [in] a_var Значение типа данных
 */
Variant::Variant(void* a_var) : m_var_type(var_type::VOID), m_pointer_to_value(new void*(a_var)) {

}

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
T Variant::value() const {
	return *static_cast<T*>(m_pointer_to_value);
}

template<>
void* Variant::value<void*>() const {
	return m_pointer_to_value;
}

template<>
uint64_t Variant::value<uint64_t>() const {
	return *static_cast<uint64_t*>(m_pointer_to_value);
}

template<>
int64_t Variant::value<int64_t>() const {
	return *static_cast<int64_t*>(m_pointer_to_value);
}

template<>
bool Variant::value<bool>() const {
	return *static_cast<bool*>(m_pointer_to_value);
}

template<>
long double Variant::value<long double>() const {
	return *static_cast<long double*>(m_pointer_to_value);
}

template<>
char Variant::value<char>() const {
	return *static_cast<char*>(m_pointer_to_value);
}

template<>
const std::string& Variant::value<const std::string&>() const {
	return *static_cast<std::string*>(m_pointer_to_value);
}

template<>
const std::wstring& Variant::value<const std::wstring&>() const {
	return *static_cast<std::wstring*>(m_pointer_to_value);
}

template<>
const Date& Variant::value<const Date&>() const {
	return *static_cast<Date*>(m_pointer_to_value);
}

template<>
const Time& Variant::value<const Time&>() const {
	return *static_cast<Time*>(m_pointer_to_value);
}

template<>
const DateTime& Variant::value<const DateTime&>() const {
	return *static_cast<DateTime*>(m_pointer_to_value);
}

/**
 * @brief Конструктор копирования.
 *
 * @param [in] a_var Объект для копирования
 */
Variant::Variant(const Variant& a_var) : m_var_type(a_var.m_var_type) {
	if (m_var_type == var_type::VOID)
		m_pointer_to_value = new void* (a_var.m_pointer_to_value);
	if (m_var_type == var_type::INT64)
		m_pointer_to_value = new int64_t(a_var.value<int64_t>());
	if (m_var_type == var_type::UINT64)
		m_pointer_to_value = new uint64_t(a_var.value<uint64_t>());
	if (m_var_type == var_type::BOOL)
		m_pointer_to_value = new bool(a_var.value<bool>());
	if (m_var_type == var_type::LDOUBLE)
		m_pointer_to_value = new long double(a_var.value<long double>());
	if (m_var_type == var_type::CHAR)
		m_pointer_to_value = new char(a_var.value<char>());
	if (m_var_type == var_type::STRING)
		m_pointer_to_value = new std::string(a_var.value<std::string>());
	if (m_var_type == var_type::WSTRING)
		m_pointer_to_value = new std::wstring(a_var.value<std::wstring>());
	if (m_var_type == var_type::DATE)
		m_pointer_to_value = new Date(a_var.value<Date>());
	if (m_var_type == var_type::TIME)
		m_pointer_to_value = new Time(a_var.value<Time>());
	if (m_var_type == var_type::DATETIME)
		m_pointer_to_value = new DateTime(a_var.value<DateTime>());;
}

/**
 * @brief Возвращение типа данных.
 * 
 * @return enum class var_type Тип данных
 */
var_type Variant::valueType() const {
	return m_var_type;
}

/**
 * @brief Перегрузка оператора сравнения == для класса Variant.
 *
 */
bool Variant::operator== (const Variant& a_other) const
{
	if (m_var_type == a_other.valueType()) {
		if (m_var_type == var_type::VOID)
			return false;
		if (m_var_type == var_type::INT64)
			return this->value<int64_t>() == a_other.value<int64_t>();
		if (m_var_type == var_type::UINT64)
			return this->value<uint64_t>() == a_other.value<uint64_t>();
		if (m_var_type == var_type::BOOL)
			return this->value<bool>() == a_other.value<bool>();
		if (m_var_type == var_type::LDOUBLE)
			return this->value<long double>() == a_other.value<long double>();
		if (m_var_type == var_type::CHAR)
			return this->value<char>() == a_other.value<char>();
		if (m_var_type == var_type::STRING)
			return this->value<std::string>() == a_other.value<std::string>();
		if (m_var_type == var_type::WSTRING)
			return this->value<std::wstring>() == a_other.value<std::wstring>();
		if (m_var_type == var_type::DATE)
			return this->value<Date>() == a_other.value<Date>();
		if (m_var_type == var_type::TIME)
			return this->value<Time>() == a_other.value<Time>();
		if (m_var_type == var_type::DATETIME)
			return this->value<DateTime>() == a_other.value<DateTime>();
	}
	else
		return false;
}

/**
 * @brief Перегрузка оператора сравнения != для класса Variant.
 *
 */
bool Variant::operator!= (const Variant& a_other) const
{
	return !(*this == a_other);
}

/**
 * @brief Перегрузка оператора присваивания = для класса Variant.
 *
 */
Variant& Variant::operator= (const Variant& a_variant)
{
	if (this == &a_variant || m_var_type == var_type::VOID)
		return *this;
	m_var_type = a_variant.m_var_type;
	delete m_pointer_to_value;
	if (m_var_type == var_type::INT64)
		m_pointer_to_value = new int64_t(a_variant.value<int64_t>());
	else if (m_var_type == var_type::UINT64)
		m_pointer_to_value = new uint64_t(a_variant.value<uint64_t>());
	else if (m_var_type == var_type::BOOL)
		m_pointer_to_value = new bool(a_variant.value<bool>());
	else if (m_var_type == var_type::LDOUBLE)
		m_pointer_to_value = new long double(a_variant.value<long double>());
	else if (m_var_type == var_type::CHAR)
		m_pointer_to_value = new char(a_variant.value<char>());
	else if (m_var_type == var_type::STRING)
		m_pointer_to_value = new std::string(a_variant.value<std::string>());
	else if (m_var_type == var_type::WSTRING)
		m_pointer_to_value = new std::wstring(a_variant.value<std::wstring>());
	else if (m_var_type == var_type::TIME)
		m_pointer_to_value = new Time(a_variant.value<Time>());
	else if (m_var_type == var_type::DATE)
		m_pointer_to_value = new Date(a_variant.value<Date>());
	else if (m_var_type == var_type::DATETIME)
		m_pointer_to_value = new DateTime(a_variant.value<DateTime>());
	return *this;
}

/**
 * @brief Деструктор.
 */
Variant::~Variant() {
	delete m_pointer_to_value;
}
