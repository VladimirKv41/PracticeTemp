#include "FactClassifier.h"
#include "Fact.h"
#include "Dimension.h"
#include "DimensionMark.h"

/**
 * @brief �����������.
 *
 * @param [in,out] a_fact ��������� ����
 * @param [in,out] a_dim ��������� ���������
 * @param [in,out] a_dim ��������� ������� �� ���������
 */
FactClassifier::FactClassifier(Fact* a_fact, Dimension* a_dim, DimensionMark* a_dim_mark) : m_fact(a_fact), m_dim(a_dim), m_dim_mark(a_dim_mark) {
	// ��� ��������� �� ��� ����� ������ ����� � ���������, ��������� � ���� ������ ������
	m_fact->push_FactClassifier(this);
	m_dim->push_FactClassifier(this);
	m_dim_mark->push_FactClassifier(m_fact->get_Metric_name(), this);
}

/**
 * @brief ��������� ���������.
 *
 * @return ��������� ���������
 */
const Dimension* const FactClassifier::get_Dimension() const {
	return m_dim;
}

/**
 * @brief ��������� �����.
 *
 * @return ��������� ����
 */
Fact* const FactClassifier::get_Fact() const {
	return m_fact;
}

/**
 * @brief ��������� �������� ������� � ��������� ���������.
 *
 * @return �������� ������� �� ��������� ���������
 */
const std::string& FactClassifier::get_dim_mark_name() const {
	return m_dim_mark->get_name();
}
