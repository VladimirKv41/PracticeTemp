#include "Measure.h"

/**
 * @brief �����������.
 * 
 * @param [in,out] a_name �������� �������
 */
Measure::Measure(const std::string& a_name) : m_name(a_name) {
	
}

/**
 * @brief ��������� �������� �������.
 * 
 * @return �������� �������
 */
const std::string& Measure::get_name() const {
	return m_name;
}



