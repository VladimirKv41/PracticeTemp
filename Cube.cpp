#include "Cube.h"
#include "Dimension.h"
#include "DimensionMark.h"
#include "Metric.h"
#include "Fact.h"
#include "FactClassifier.h"
#include "Selection.h"
#include <unordered_map>
/**
 * @brief �����������.
 *
 * �� ��������� ������ �� ������� ��������.
 */
Cube::Cube() : m_selection(nullptr) {

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
 * @param [in] a_metric_name �������� �������, ��� ������, � ����� ����������
 * @return ��������� ������� ��� ���
 */
bool Cube::add_Metric(const std::string& a_metric_name) {
	for (std::vector<Metric*>::iterator it = m_metrics.begin(); it != m_metrics.end(); it++) {
		if ((*it)->get_name() == a_metric_name) {
			return false;
		}
	}
	m_metrics.push_back(new Metric(a_metric_name));
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
 * @param [in] a_metric �������� ������� ������������ �����
 * @param [in] a_marks_list ������ ������� � ���������� ������������ �����
 * @return enum class add_result ��������� ���������� ����� : ����������� �������/��� ����������/��������
 */
add_result Cube::add_Fact(double a_value, const std::string& a_metric_name, const std::vector<std::string>& a_marks_list) {
	// ����� ��������� �������
	std::vector<Metric*>::const_iterator it_metric = find_metric(a_metric_name);
	if (it_metric == m_metrics.end()) {
		return add_result::UNKNOWN_MEASURE;
	}
	// ����� � ��� ����������� ����� ������
	if (m_facts.find({ a_metric_name, a_marks_list }) != m_facts.end())
		return add_result::ALREADY_EXIST;
	auto it_list = a_marks_list.begin();
	auto iter_new_fact = m_facts.insert({ { a_metric_name, a_marks_list }, new Fact(a_value, *it_metric) });
	// ���������� ����� ������
	for (auto it_dim = m_dims.begin(); it_dim != m_dims.end(); it_dim++, it_list++) {
		m_points.push_back(new FactClassifier(iter_new_fact->second, *it_dim, (*it_dim)->get_DimensionPosition(*it_list)));
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
	clean_vector(m_metrics);
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
std::vector<Metric*>::const_iterator Cube::find_metric(const std::string& a_metric_name) {
	for (auto it_meas = m_metrics.begin(); it_meas != m_metrics.end(); it_meas++) {
		if ((*it_meas)->get_name() == a_metric_name) {
			return it_meas;
		}
	}
	return m_metrics.end();
}

/**
 * @brief ������� ������� ����������.
 *
 * @param [in,out] a_vector ������ �� ����������� �������
 */
template <class T>
void Cube::clean_vector(std::vector<T*>& a_vector) {
	for (class std::vector<T*>::iterator it = a_vector.begin(); it != a_vector.end(); it++) {
		delete* it;
	}
	// ���������� ������� ������� ��� ����, �.�. ����� ������� �������� ������� ���������
	a_vector.resize(0);
}
