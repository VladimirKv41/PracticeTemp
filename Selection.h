#pragma once
#include <vector>
#include <string>
#include <map>
#include <unordered_map>
#include <set>

/**
 * @brief ��� ����������.
 */
enum class agg_type {
	COUNT, // ����������
	SUM, // �����
	AVERAGE // �������
};

/**
 * @brief ��������� ����������� �������.
 */
enum class make_result {
	CUBE_NOT_FOUND = -2, // ��� ������
	UNKNOWN_DIMENSION, // ����������� ���������
	NO_FACT_FOUND, // ����� �� �������
	CREATED // ������� �������
};

class FactClassifier;
class Dimension;
class DimensionMark;
class Cube;
class Fact;
class Metric;

struct hash_pair {
	template <class T1, class T2>
	size_t operator()(const std::pair<T1, T2>& p) const
	{
		auto hash1 = std::hash<T1>{}(p.first);
		auto hash2 = std::hash<T2>{}(p.second);
		return hash1 ^ hash2;
	}
};
// ��� ������ ������� ����� ������
// ����: ���� <�������� ���������, �������� ������� �� ���������>,
// ��������: ����: �������� �������, ��������: ��������� �� ����� ������
typedef std::unordered_map<std::pair<std::string, std::string>, std::unordered_multimap<std::string, FactClassifier*>, hash_pair> dpoint_ummaps_map;

/**
 * @brief �������.
 *
 * ����� ��� ����������� � ������������� ������� �� ������ ������ �� ����.
 */
class Selection {
public:

	Selection(Cube* a_cube);

	// �������� �������
	make_result make(const std::string& a_dim_name, const std::vector<std::string>& a_marks_list, const std::set<std::string>& a_metric_list = {});

	// ������� �������
	void clean(bool cube_cleaning = false);

	~Selection();

private:

	// ����� ���������
	std::vector<Dimension*>::const_iterator find_dimension(const std::string& a_dimension_name) const;

	// ���������� ������� (��� ���������� ������ �������)
	void making
	(
		const std::map<std::string, DimensionMark*>& a_map_from,
		const std::string& a_dim_name, const std::vector<std::string>& a_marks_list,
		const std::set<std::string>& a_metric_list
	);

	// ���������� ������� (��� ��� ����������� �������)
	void making
	(
		const dpoint_ummaps_map& a_map_from,
		const std::string& a_dim_name,
		const std::vector<std::string>& a_marks_list,
		const std::set<std::string>& a_metric_list
	);

	// ��������� ����� ������ � ���������� ���������
	void get_FactClassifiers_by_metric
	(
		std::unordered_multimap<std::string, FactClassifier*>& a_umap_to,
		const std::unordered_multimap<std::string, FactClassifier*>& a_umap_from,
		const std::set<std::string>& a_metric_list
	);

	// ���������� ���������� ����� ������
	void add_rest_fact_points(dpoint_ummaps_map& a_map, const std::string& a_dim_name, const std::string& a_dim_mark);


	// �� ����� ������ ����, �� ������� ������� �������
	dpoint_ummaps_map m_selection_points;
	// �������, ��������� �� ������ ���������
	std::vector<Metric*> m_aggregation_metrics;
	// ��������� ��� ���������
	Dimension* m_aggregation_dim;
	// ��������� ��� ��� �������� �������
	Cube* m_cube;
};