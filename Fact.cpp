#include "Fact.h"
#include "Metric.h"


/**
 * @brief �����������.
 *
 * @param [in] a_value ��������
 * @param [in,out] a_mes �������
 */
Fact::Fact(double a_value, Metric* a_mes) : m_value(a_value), m_mes(a_mes) {

}

/**
 * @brief ���������� ��������� ����� ������.
 *
 * @param [in,out] a_point ����� ������
 */
void Fact::push_FactClassifier(FactClassifier* a_point) {
	m_points.push_back(a_point);
}

/**
 * @brief ��������� ������� ��������� ����� ������.
 *
 * @return ������ ��������� ����� ������
 */
const std::vector<FactClassifier*>& Fact::get_FactClassifiers() const {
	return m_points;
}

/**
 * @brief ��������� �������� �����.
 *
 * @return �������� �����
 */
double Fact::get_value() const {
	return m_value;
}

/**
 * @brief ��������� �������� ������� �����.
 *
 * @return �������� �������
 */
const std::string& Fact::get_Metric_name() const {
	return m_mes->get_name();
}

/**
 * @brief ��������� ��������� �������.
 *
 * @return ��������� �������
 */
const Metric* const Fact::get_Metric() const {
	return m_mes;
}