#pragma once
#include <vector>
#include <string>

class Dimension;
class Metric;
class FactClassifier;

/**
 * @brief ����.
 *
 * ����� �������� �������� � ��� �������.
 */
class Fact {
public:

	Fact(double a_value, Metric* a_mes);

	// ���������� ��������� ����� ������
	void push_FactClassifier(FactClassifier* a_point);

	// ��������� ������� ��������� ����� ������
	const std::vector<FactClassifier*>& get_FactClassifiers() const;

	// ��������� �������� �����
	double get_value() const;

	// ��������� �������� ������� �����
	const std::string& get_Metric_name() const;

	// ��������� ��������� �������
	const Metric* const get_Metric() const;

private:
	// �������� �����
	double m_value;
	// ��������� �������
	Metric* m_mes;
	// ������ ��������� ����� ������
	std::vector<FactClassifier*> m_points;
};