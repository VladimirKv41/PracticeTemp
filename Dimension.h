#pragma once
#include <string>
#include <vector>
#include <map>

class DataPoint;
class DimensionPosition;

/**
 * @brief ���������.
 * 
 * ����� ��� �������� � ������ � ����� �� "����" ����.
 */
class Dimension {
public:

	Dimension(const std::string &a_name);

	// ���������� ��������� ����� ������
	void push_DataPoint(DataPoint* a_point);

	// ��������� ������� ��������� ����� ������
	const std::vector<DataPoint*>& get_DataPoints() const;

	// ��������� ������� � ���������
	DimensionPosition* const get_DimensionPosition(const std::string &a_mark);

	// ����� ������� � ���������
	bool search_mark(const std::string &a_mark) const;

	// ��������� �������� ���������
	const std::string& get_name() const;

	// ��������� ������� ������� �� ���������
	const std::map<std::string, DimensionPosition*>& get_positions() const;

	~Dimension();
	
private:
	// ������ ������� �� ���������
	// ����: �������� �������
	std::map<std::string,DimensionPosition*> m_positions_map;
	// �������� ���������
	std::string m_name;
	// ������ ��������� ����� ������
	std::vector<DataPoint*> m_points;
};