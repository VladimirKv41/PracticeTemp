#pragma once
#include <iostream>

class Time;
class Date;
struct DateTime;

/**
 * @brief ����� ���������.
 *
 * ����� ��� �������� �������� ��������� ����� ������ ������.
 */
template <typename T>
class Variant {
public:

	Variant(T a_var);

	const std::string& to_String() const;

	~Variant();

private:
	T m_value;
};