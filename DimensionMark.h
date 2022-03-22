#pragma once
#include <string>
#include <unordered_map>

class Dimension;
class Metric;
class FactClassifier;

/**
 * @brief ������� (�� ���������).
 *
 * ����� �������� ����� ������ ������� �� ��������� � �� �������.
 */
class DimensionMark {
public:

	DimensionMark(const std::string& a_name);

	// ���������� ��������� ����� ������
	void push_FactClassifier(const std::string& a_meas, FactClassifier* a_point);

	// ��������� �������� ������� �� ���������
	const std::string& get_name() const;

	// ��������� ������� ��������� ����� ������
	const std::unordered_multimap<std::string, FactClassifier*>& get_FactClassifiers() const;

private:

	// �������� �������
	std::string m_name;
	// ������ ��������� ����� ������
	// ����: �������� �������
	std::unordered_multimap<std::string, FactClassifier*> m_points;
};