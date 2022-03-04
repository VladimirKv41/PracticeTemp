#include "Fact.h"
#include "Measure.h"


/**
 * @brief �����������.
 * 
 * @param [in] a_value ��������
 * @param [in,out] a_mes �������
 */
Fact::Fact(double a_value,Measure* a_mes) : m_value(a_value), m_mes(a_mes) {
	
}

/**
 * @brief ���������� ��������� ����� ������.
 * 
 * @param [in,out] a_point ����� ������
 */
void Fact::push_DataPoint(DataPoint* a_point) {
	m_points.push_back(a_point);
}

/**
 * @brief ��������� ������� ��������� ����� ������.
 * 
 * @return ������ ��������� ����� ������
 */
const std::vector<DataPoint*>& Fact::get_DataPoints() const {
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
const std::string& Fact::get_Measure_name() const {
	return m_mes->get_name();
}

/**
 * @brief ��������� ��������� �������.
 * 
 * @return ��������� �������
 */
const Measure* const Fact::get_Measure() const {
	return m_mes;
}