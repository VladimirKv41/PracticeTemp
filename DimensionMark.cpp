#include "DimensionMark.h"

/**
 * @brief �����������.
 *
 * @param [in,out] a_name �������� ������� �� ���������
 */
DimensionMark::DimensionMark(const std::string& a_name) : m_name(a_name) {

}

/**
 * @brief ���������� ��������� ����� ������.
 *
 * @param [in,out] a_meas �������� �������
 * @param [in,out] a_point ����� ������
 */
void DimensionMark::push_FactClassifier(const std::string& a_meas, FactClassifier* a_point) {
	m_points.insert({ a_meas,a_point });
}

/**
 * @brief ��������� �������� ������� �� ���������.
 *
 * @return �������� ������� �� ���������
 */
const std::string& DimensionMark::get_name() const {
	return m_name;
}

/**
 * @brief ��������� ������� ��������� ����� ������.
 *
 * @return ������ ��������� ����� ������
 */
const std::unordered_multimap<std::string, FactClassifier*>& DimensionMark::get_FactClassifiers() const {
	return m_points;
}