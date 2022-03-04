#pragma once
#include <string>
#include <unordered_map>

class Dimension;
class Measure;
class DataPoint;

/**
 * @brief ������� (�� ���������).
 *
 * ����� �������� ����� ������ ������� �� ��������� � �� �������.
 */
class DimensionPosition {
public:

	DimensionPosition(const std::string& a_name);

	// ���������� ��������� ����� ������
	void push_DataPoint(const std::string& a_meas, DataPoint* a_point);

	// ��������� �������� ������� �� ���������
	const std::string& get_name() const;

	// ��������� ������� ��������� ����� ������
	const std::unordered_multimap<std::string, DataPoint*>& get_DataPoints() const;

private:

	// �������� �������
	std::string m_name;
	// ������ ��������� ����� ������
	// ����: �������� �������
	std::unordered_multimap<std::string, DataPoint*> m_points;
};