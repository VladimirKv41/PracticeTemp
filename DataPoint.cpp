#include "DataPoint.h"
#include "Fact.h"
#include "Dimension.h"
#include "DimensionPosition.h"

/**
 * @brief �����������.
 * 
 * @param [in,out] a_fact ��������� ����
 * @param [in,out] a_dim ��������� ���������
 * @param [in,out] a_dim ��������� ������� �� ���������
 */
DataPoint::DataPoint(Fact* a_fact, Dimension* a_dim, DimensionPosition* a_dim_position) : m_fact(a_fact), m_dim(a_dim), m_dim_position(a_dim_position) {
	// ��� ��������� �� ��� ����� ������ ����� � ���������, ��������� � ���� ������ ������
	m_fact->push_DataPoint(this);
	m_dim->push_DataPoint(this);
	m_dim_position->push_DataPoint(m_fact->get_Measure_name(),this);
}	

/**
 * @brief ��������� ���������.
 * 
 * @return ��������� ���������
 */
const Dimension* const DataPoint::get_Dimension() const {
	return m_dim;
}

/**
 * @brief ��������� �����.
 * 
 * @return ��������� ����
 */
Fact* const DataPoint::get_Fact() const {
	return m_fact;
}

/**
 * @brief ��������� �������� ������� � ��������� ���������.
 * 
 * @return �������� ������� �� ��������� ���������
 */
const std::string& DataPoint::get_dim_position_name() const {
	return m_dim_position->get_name();
}
