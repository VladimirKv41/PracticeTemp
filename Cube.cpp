#include "Cube.h"
#include "Dimension.h"
#include "DimensionPosition.h"
#include "Measure.h"
#include "Fact.h"
#include "DataPoint.h"
#include "Selection.h"
#include <unordered_map>
/**
 * @brief �����������.
 * 
 * �� ��������� ������ �� ������� ��������.
 */
Cube::Cube() : m_selection(nullptr){

}

/**
 * @brief ���������� ���������.
 * 
 * @param [in] a_dim_name �������� ���������, ��� ������, � ����� ����������
 * @return ��������� ��������� ��� ���
 */
bool Cube::add_Dimension(const std::string& a_dim_name) {
	for (std::vector<Dimension*>::iterator it = m_dims.begin(); it != m_dims.end(); it++) {
		if ((*it)->get_name() == a_dim_name) {
			return false;
		}
	}
	m_dims.push_back(new Dimension(a_dim_name));
	return true;
}

/**
 * @brief ���������� �������.
 * 
 * @param [in] a_measure_name �������� �������, ��� ������, � ����� ����������
 * @return ��������� ������� ��� ���
 */
bool Cube::add_Measure(const std::string& a_measure_name) {
	for (std::vector<Measure*>::iterator it = m_measures.begin(); it != m_measures.end(); it++) {
		if ((*it)->get_name() == a_measure_name) {
			return false;
		}
	}
	m_measures.push_back(new Measure(a_measure_name));
	return true;
}

/**
 * @brief ���������� �����.
 * 
 * 1. ����� ��������� ������� ����� ����� �����������.
 * 2. ����� ����� � ��� ����������� �����.
 * 3. ���������� ����� � ����� ������.
 * 
 * @param [in] a_value �������� ������������ �����
 * @param [in] a_measure �������� ������� ������������ �����
 * @param [in] a_positions_list ������ ������� � ���������� ������������ �����
 * @return enum class add_result ��������� ���������� ����� : ����������� �������/��� ����������/��������
 */
add_result Cube::add_Fact(double a_value, const std::string& a_measure_name, const std::vector<std::string>& a_positions_list) {
	// ����� ��������� �������
	std::vector<Measure*>::const_iterator it_measure = find_measure(a_measure_name);
	if (it_measure == m_measures.end()) {
		return add_result::UNKNOWN_MEASURE;
	}
	// ����� � ��� ����������� ����� ������
	if(m_facts.find({ a_measure_name, a_positions_list }) != m_facts.end())
		return add_result::ALREADY_EXIST;
	auto it_list = a_positions_list.begin();
	auto iter_new_fact = m_facts.insert({ { a_measure_name, a_positions_list }, new Fact(a_value, *it_measure) });
	// ���������� ����� ������
	for (auto it_dim = m_dims.begin(); it_dim != m_dims.end(); it_dim++, it_list++) {
		m_points.push_back(new DataPoint(iter_new_fact->second, *it_dim, (*it_dim)->get_DimensionPosition(*it_list)));
	}
	return add_result::ADDED;
}

/**
 * @brief ������� ����.
 */
void Cube::clean() {
	// ���� ������� �� �������/����������
	if (m_selection != nullptr) {
		m_selection->clean(true);
	}
	for (auto it = m_facts.begin(); it != m_facts.end(); it++) {
		delete it->second;
	}
	m_facts.clear();
	clean_vector(m_measures);
	clean_vector(m_dims);
	clean_vector(m_points);
}

/**
 * @brief ����������.
 */
Cube::~Cube() {
	clean();
}

/**
 * @brief ����� �������.
 *
 * @param [in] a_dimension_name �������� �������
 * @return �������� �� ������� ��� end()
 */
std::vector<Measure*>::const_iterator Cube::find_measure(const std::string& a_measure_name) {
	for (auto it_meas = m_measures.begin(); it_meas != m_measures.end(); it_meas++) {
		if ((*it_meas)->get_name() == a_measure_name) {
			return it_meas;
		}
	}
	return m_measures.end();
}

/**
 * @brief ������� ������� ����������.
 * 
 * @param [in,out] a_vector ������ �� ����������� �������
 */
template <class T>
void Cube::clean_vector(std::vector<T*>& a_vector) {
	for (class std::vector<T*>::iterator it = a_vector.begin(); it != a_vector.end(); it++) {
		delete *it;
	}
	// ���������� ������� ������� ��� ����, �.�. ����� ������� �������� ������� ���������
	a_vector.resize(0);
}
