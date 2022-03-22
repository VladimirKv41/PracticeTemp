#pragma once
#include <string>
#include <vector>
#include <map>
#include <unordered_map>

/**
 * @brief ��������� ���������� �����.
 */
enum class add_result {
	UNKNOWN_MEASURE = -1, // ����������� �������
	ALREADY_EXIST, // ���� ��� ����������
	ADDED // ���� ��������
};

class Dimension;
class Fact;
class Metric;
class FactClassifier;
class Selection;

/**
 * @brief ���.
 *
 * ����� ��� ���������� � �������� ������.
 */
class Cube {
public:

	friend Selection;

	Cube();

	// ���������� ���������
	bool add_Dimension(const std::string& a_dim_name);

	// ���������� �������
	bool add_Metric(const std::string& a_metric_name);

	// ���������� �����
	add_result add_Fact(double a_value, const std::string& a_metric_name, const std::vector<std::string>& a_marks_list);

	// ������� ����
	void clean();

	~Cube();

private:

	// ����� �������
	std::vector<Metric*>::const_iterator find_metric(const std::string& a_metric_name);

	// ������� ������� ����������
	template <class T>
	void clean_vector(std::vector<T*>& a_vector);
	// ���������� ������/���������/������/����� ������ ����
	std::multimap<std::pair<std::string, std::vector<std::string>>, Fact*> m_facts;
	std::vector<Dimension*> m_dims;
	std::vector<Metric*> m_metrics;
	std::vector<FactClassifier*> m_points;
	// ��������� �������
	Selection* m_selection;
};