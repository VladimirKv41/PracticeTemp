#pragma once
#include <vector>
#include <string>

class Dimension;
class Measure;
class DataPoint;

/**
 * @brief ����.
 * 
 * ����� �������� �������� � ��� �������.
 */
class Fact {
public:

	Fact(double a_value,Measure* a_mes);

	// ���������� ��������� ����� ������
	void push_DataPoint(DataPoint* a_point);

	// ��������� ������� ��������� ����� ������
	const std::vector<DataPoint*>& get_DataPoints() const;

	// ��������� �������� �����
	double get_value() const;

	// ��������� �������� ������� �����
	const std::string& get_Measure_name() const;

	// ��������� ��������� �������
	const Measure* const get_Measure() const;

private:
	// �������� �����
	double m_value;
	// ��������� �������
	Measure* m_mes;
	// ������ ��������� ����� ������
	std::vector<DataPoint*> m_points;
};