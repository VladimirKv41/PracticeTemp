#include "Selection.h"
#include "FactClassifier.h"
#include "Cube.h"
#include "Dimension.h"
#include "DimensionMark.h"
#include "Fact.h"
#include "Metric.h"

/**
 * @brief �����������.
 *
 * 1. ������������� ���� � ��������� ��� ���������.
 * 2. �������� ������ �� ������ ������� ����.
 *
 * @param [in] a_cube ��� ��� �������� �������
 */
Selection::Selection(Cube* a_cube) : m_cube(a_cube), m_aggregation_dim(new Dimension("���������")) {
	m_cube->m_selection = this;
}


/**
 * @brief �������� �������.
 *
 * 1. ��������, ���������� �� ���.
 * 2. ����� ���������� ��������� ����� ����� �����������.
 * 3. ���������� �������, ��������� �� ����, ������ �� ���������� �������.
 * 4. ��������, ������ �� ���� ���� ����.
 *
 * @param [in] a_dim_name �������� ��������� (����������� ����� ������ ������ � ���� ��������� ���������,
 * � ����� ��� ����� ������ ��������� � ����� ����� ����)
 * @param [in] a_marks_list ������ �������� ������� � ���������
 * (����������� ����� ������ ������ � ���������� ���������� �������)
 * @param [in] a_metric_list ������ �������� ������
 * (����������� ����� ������ ������ � ����� ���������� ������)
 * @return enum �lass make_result ��������� ����������� �������:
 * ��� �� ������/����������� ���������/�������/����� �� �������
 */
make_result Selection::make(const std::string& a_dim_name, const std::vector<std::string>& a_marks_list, const std::set<std::string>& a_metric_list)
{
	// ��������, ���������� �� ���
	if (m_cube == nullptr)
		return make_result::CUBE_NOT_FOUND;
	// ����� ���������� ��������� ����� ����� �����������
	std::vector<Dimension*>::const_iterator it_dim = find_dimension(a_dim_name);
	if (it_dim == m_cube->m_dims.end()) {
		return make_result::UNKNOWN_DIMENSION;
	}
	// ���������� �������
	if (m_selection_points.empty()) {
		making((*it_dim)->get_marks(), a_dim_name, a_marks_list, a_metric_list);
	}
	else {
		making(m_selection_points, a_dim_name, a_marks_list, a_metric_list);
	}
	// ������� �� �����
	if (m_selection_points.empty())
		return make_result::NO_FACT_FOUND;
	else
		return make_result::CREATED;
}

/**
 * @brief ������� �������.
 *
 * 1. �����������, ����� �� ������� ��� (�� ��������� - ���).
 * 2. �����������, ���� �� ����������� ����� ������, ���� ��,
 * �� ������� ���������.
 *
 * @param [in] cube_cleaning ������� �� ���������� � ���� (�� ��������� - ���)
 */
void Selection::clean(bool cube_cleaning) {
	if (cube_cleaning) {
		m_cube = nullptr;
	}
	if (!m_selection_points.empty()) {
		m_selection_points.clear();
		m_cube->clean_vector(m_aggregation_metrics);
	}
}

/**
 * @brief ����������.
 */
Selection::~Selection() {
	clean();
	delete m_aggregation_dim;
	m_aggregation_dim = nullptr;
	// ��������� ����, ��� ������� �������
	if (m_cube != nullptr)
		m_cube->m_selection = nullptr;
}

/**
 * @brief ���������� ������� (��� ���������� ������ �������).
 *
 * �������� �� ������ �������, ������ ������� ������ � ���������:
 *	1. �������� ����� �����������, ���������� �� ����� ������� � ���������.
 *	2. ���������� ����� ������ ������� � ������ ����� ������ �������
 *	(����������� ������� � ���������� ���������).
 *	3. ��������, ���� �� ������� ���� ���� ����� ������ � ��������� ��������,
 *	���� ���, �� ��������������� ������� � ������� ���������.
 *	4. ���������� ���������� ����� ������, ��� "������" ����������� ������.
 *
 * @param [in] a_map_from ������ � ��������� ���������, �� ����� ������� ����� ������������ �������
 * @param [in] a_dim_name �������� ���������, ����������� ����� ������ ������ � ���� ��������� ���������
 * @param [in] a_marks_list ������ �������� �������,
 * ����������� ����� ������ ������ � ���������� ���������� �������
 * @param [in] a_metric_list ������ �������� ������, ����������� ����� ������ ������ � ����� ���������� ������
 */
void Selection::making
(
	const std::map<std::string, DimensionMark*>& a_map_from,
	const std::string& a_dim_name,
	const std::vector<std::string>& a_marks_list,
	const std::set<std::string>& a_metric_list
)
{
	for (auto it_dim_mark = a_marks_list.begin(); it_dim_mark != a_marks_list.end(); it_dim_mark++) {
		// C��������� �� ����� �������
		if (a_map_from.find(*it_dim_mark) == a_map_from.end()) \
			continue;
		// ���������� ����� ������
		get_FactClassifiers_by_metric(m_selection_points[{a_dim_name, * it_dim_mark}], a_map_from.at(*it_dim_mark)->get_FactClassifiers(), a_metric_list);
		// ���� �� ������� ���� ���� ����� ������
		if (m_selection_points[{a_dim_name, * it_dim_mark}].empty()) {
			m_selection_points.erase({ a_dim_name, *it_dim_mark });
			continue;
		}
		// ���������� ���������� ����� ������
		add_rest_fact_points(m_selection_points, a_dim_name, *it_dim_mark);
	}
}

/**
 * @brief ���������� ������� (��� ��� ����������� �������).
 *
 * 1. ���������� ��������� ������� ��� ���������� ������� ������ �� ��������,
 * � ����� �������� ����� ���������� �� ���������.
 * 2. �������� �� ������ �������, ������ ������� ������ � �������:
 *	2.1. �������� ����� �����������, ���������� �� ����� ������� � �������.
 *	2.2. ���������� ����� ������ ������� �� ��������� ������ ����� ������ ������� (����������� ������� � ���������� ���������).
 *	2.3. ��������, ���� �� ������� ���� ���� ����� ������ � ��������� ��������,
 *		���� ���, �� ��������������� ������� � ��������� ������� ���������.
 *	2.4. ���������� ���������� ����� ������, ��� "������" ����������� ������.
 * 3. �������� ������� �������������� � ���������.
 *
 * @param [in] a_map_from ������ � ��������� ���������, �� ����� ������� ����� ������������ �������
 * @param [in] a_dim_name �������� ���������, ����������� ����� ������ ������ � ���� ��������� ���������
 * @param [in] a_marks_list ������ �������� �������, ����������� ����� ������ ������ � ���������� ���������� �������
 * @param [in] a_metric_list ������ �������� ������, ����������� ����� ������ ������ � ����� ���������� ������
 */
void Selection::making
(
	const dpoint_ummaps_map& a_map_from,
	const std::string& a_dim_name,
	const std::vector<std::string>& a_marks_list,
	const std::set<std::string>& a_metric_list
)
{
	// ���������� ��������� �������
	dpoint_ummaps_map temp_selection_points;
	for (auto it_dim_mark = a_marks_list.begin(); it_dim_mark != a_marks_list.end(); it_dim_mark++) {
		// C��������� �� ����� �������
		if (a_map_from.find({ a_dim_name, *it_dim_mark }) == a_map_from.end())
			continue;
		// ���������� ����� ������
		get_FactClassifiers_by_metric(temp_selection_points[{a_dim_name, * it_dim_mark}], a_map_from.at({ a_dim_name, *it_dim_mark }), a_metric_list);
		// ���� �� ������� ���� ���� ����� ������
		if (temp_selection_points[{a_dim_name, * it_dim_mark}].empty()) {
			temp_selection_points.erase({ a_dim_name, *it_dim_mark });
			continue;
		}
		// ���������� ���������� ����� ������
		add_rest_fact_points(temp_selection_points, a_dim_name, *it_dim_mark);
	}
	m_selection_points = temp_selection_points;
}

/**
 * @brief ��������� ����� ������ � ���������� ���������.
 *
 * 1. ��������, ������ �� ������ ������, ���� ��, ������� ��� ����� ������.
 * 2. �������� �� ������ ������, ������ ������ � �����������:
 *	2.2. ���������� ���������� ��� �������� ���� ����������.
 *	2.2. ���������� ����� ������.
 *
 * @param [out] a_map_to ������, � ������� ����� ����������� ����� ������
 * @param [in] a_map_from  ������, �� �������� ����� ������� ����� ������
 * @param [in] a_metric_list ������ �������� ������, ����������� ����� ������ ������ � ����� ���������� ������
 */
void Selection::get_FactClassifiers_by_metric
(
	std::unordered_multimap<std::string, FactClassifier*>& a_ummap_to,
	const std::unordered_multimap<std::string, FactClassifier*>& a_ummap_from,
	const std::set<std::string>& a_metric_list
)
{
	// ������ �� ������ ������
	if (a_metric_list.empty()) {
		a_ummap_to = a_ummap_from;
		return;
	}
	for (auto it = a_metric_list.begin(); it != a_metric_list.end(); it++) {
		auto range_iter = a_ummap_from.equal_range(*it);
		// ���������� ����� ������
		a_ummap_to.insert(range_iter.first, range_iter.second);
	}
}

/**
 * @brief ���������� ���������� ����� ������.
 *
 * 1. �������� �� ������ ������(1) � �������:
 *		2. �������� �� ������ ������(2) �����, ���������� � ������ ������(1),
 *		��� "c�����" ������������ �����:
 *			3. ���������� ���� ����� ������ ���������� �� ��������������� ������ � �������,
 *			����� ����� ������(1).
 *
 * @param [in] a_map ������, � ������� ����� ����������� ����� ������
 * @param [in] a_dim_name �������� ���������, ����������� ��� ����� ������ � �� ����� ��������� ���������
 * @param [in] a_dim_mark �������� ������� � ���������
 * (��������� ���������� ����� ������ ����� ����� ������ � �������� a_dim_mark � ���������� a_dim_name)
 */
void Selection::add_rest_fact_points(dpoint_ummaps_map& a_map, const std::string& a_dim_name, const std::string& a_dim_mark) {
	auto& a_map_value = a_map[{a_dim_name, a_dim_mark}];
	for (auto it_dpoint = a_map_value.begin(); it_dpoint != a_map_value.end(); it_dpoint++) {
		// �������� �� ������ ������ �����
		auto fact_datapoints = (*it_dpoint).second->get_Fact()->get_FactClassifiers();
		for (auto it_dpoint2 = fact_datapoints.begin(); it_dpoint2 != fact_datapoints.end(); it_dpoint2++) {
			auto& dim_name = (*it_dpoint2)->get_Dimension()->get_name();
			// ���������� ���� ����� ������
			if (dim_name != a_dim_name) {
				a_map[{dim_name, (*it_dpoint2)->get_dim_mark_name()}].emplace((*it_dpoint).second->get_Fact()->get_Metric_name(), *it_dpoint2);
			}
		}
	}
}

/**
 * @brief ����� ���������.
 *
 * @param [in] a_dimension_name �������� ���������
 * @return �������� �� ��������� ��� end()
 */
std::vector<Dimension*>::const_iterator Selection::find_dimension(const std::string& a_dimension_name) const {
	for (auto it_dim = m_cube->m_dims.begin(); it_dim != m_cube->m_dims.end(); it_dim++) {
		if ((*it_dim)->get_name() == a_dimension_name) {
			return it_dim;
		}
	}
	return m_cube->m_dims.end();
}