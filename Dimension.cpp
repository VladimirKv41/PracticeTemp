#include "Dimension.h"
#include "DimensionMark.h"

/**
 * @brief �����������.
 *
 * @param [in] a_name �������� ���������
 */
Dimension::Dimension(const std::string& a_name) : m_name(a_name) {

}

/**
 * @brief ���������� ��������� ����� ������.
 *
 * @param [in,out] a_point ����� ������
 */
void Dimension::push_FactClassifier(FactClassifier* a_point) {
	m_points.push_back(a_point);
}

/**
 * @brief ��������� ������� ��������� ����� ������.
 *
 * @return ������ ���� ����� ������ ��������� � ���� ����������
 */
const std::vector<FactClassifier*>& Dimension::get_FactClassifiers() const {
	return m_points;
}

/**
 * @brief ��������� ������� �� ���������.
 *
 * ���� ������� �� ����������, �� ������� ��������.
 *
 * @param [in] a_mark �������� �������
 * @return �������
 */
DimensionMark* const Dimension::get_DimensionPosition(const std::string& a_mark) {
	if (m_marks_map.find(a_mark) == m_marks_map.end()) {
		m_marks_map[a_mark] = new DimensionMark(a_mark);
		return m_marks_map[a_mark];
	}
	else
		return m_marks_map[a_mark];
}

/**
 * @brief ����� ������� � ���������.
 *
 * @param [in] a_mark �������� �������
 * @return ���������� ������� ��� ���
 */
bool Dimension::search_mark(const std::string& a_mark) const {
	if (m_marks_map.find(a_mark) == m_marks_map.end()) {
		return false;
	}
	else
		return true;
}

/**
 * @brief ��������� �������� ���������.
 *
 * @return �������� ���������
 */
const std::string& Dimension::get_name() const {
	return m_name;
}

/**
 * @brief ��������� ������� ������� �� ���������.
 *
 * @return ������ ������� �� ���������
 */
const std::map<std::string, DimensionMark*>& Dimension::get_marks() const {
	return m_marks_map;
}

Dimension::~Dimension() {
	for (auto it = m_marks_map.begin(); it != m_marks_map.end(); it++) {
		delete it->second;
	}
}
