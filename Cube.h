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
class Measure;
class DataPoint;
class Selection;

/**
 * @brief ���.
 * 
 * ����� ��� ���������� � �������� ������.
 */
class Cube{
public:

	friend Selection;

	Cube();

	// ���������� ���������
	bool add_Dimension(const std::string& a_dim_name);

	// ���������� �������
	bool add_Measure(const std::string& a_measure_name);

	// ���������� �����
	add_result add_Fact(double a_value, const std::string& a_measure_name, const std::vector<std::string>& a_positions_list);

    // ������� ����
	void clean();
	
	~Cube();

private:

	// ����� �������
	std::vector<Measure*>::const_iterator find_measure(const std::string& a_measure_name);

	// ������� ������� ����������
	template <class T>
	void clean_vector(std::vector<T*>& a_vector);
	// ���������� ������/���������/������/����� ������ ����
	std::multimap<std::pair<std::string, std::vector<std::string>>, Fact*> m_facts;
	std::vector<Dimension*> m_dims;
	std::vector<Measure*> m_measures;
	std::vector<DataPoint*> m_points;
	// ��������� �������
	Selection* m_selection;
};