#include "Metric.h"

/**
 * @brief �����������.
 *
 * @param [in,out] a_name �������� �������
 */
Metric::Metric(const std::string& a_name) : m_name(a_name) {

}

/**
 * @brief ��������� �������� �������.
 *
 * @return �������� �������
 */
const std::string& Metric::get_name() const {
	return m_name;
}