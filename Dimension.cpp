#include "Dimension.h"
#include "DimensionPosition.h"

/**
 * @brief �����������.
 * 
 * @param [in] a_name �������� ���������
 */
Dimension::Dimension(const std::string &a_name) : m_name(a_name) {
	
}

/**
 * @brief ���������� ��������� ����� ������.
 * 
 * @param [in,out] a_point ����� ������
 */
void Dimension::push_DataPoint(DataPoint* a_point) {
	m_points.push_back(a_point);
}

/**
 * @brief ��������� ������� ��������� ����� ������.
 * 
 * @return ������ ���� ����� ������ ��������� � ���� ����������
 */
const std::vector<DataPoint*>& Dimension::get_DataPoints() const {
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
DimensionPosition* const Dimension::get_DimensionPosition(const std::string &a_mark) {
	if (m_positions_map.find(a_mark) == m_positions_map.end()) {
		m_positions_map[a_mark] = new DimensionPosition(a_mark);
		return m_positions_map[a_mark];
	}
	else 
		return m_positions_map[a_mark];
}

/**
 * @brief ����� ������� � ���������.
 * 
 * @param [in] a_mark �������� �������
 * @return ���������� ������� ��� ���
 */
bool Dimension::search_mark(const std::string &a_mark) const {
	if (m_positions_map.find(a_mark) == m_positions_map.end()) {
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
const std::map<std::string, DimensionPosition*>& Dimension::get_positions() const {
	return m_positions_map;
}

Dimension::~Dimension(){
	for (auto it = m_positions_map.begin(); it != m_positions_map.end(); it++) {
		delete it->second;
	}
}
