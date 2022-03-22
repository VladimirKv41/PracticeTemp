#pragma once
#include <string>
#include <vector>
#include <map>

class FactClassifier;
class DimensionMark;

/**
 * @brief ���������.
 *
 * ����� ��� �������� � ������ � ����� �� "����" ����.
 */
class Dimension {
public:

	Dimension(const std::string& a_name);

	// ���������� ��������� ����� ������
	void push_FactClassifier(FactClassifier* a_point);

	// ��������� ������� ��������� ����� ������
	const std::vector<FactClassifier*>& get_FactClassifiers() const;

	// ��������� ������� � ���������
	DimensionMark* const get_DimensionPosition(const std::string& a_mark);

	// ����� ������� � ���������
	bool search_mark(const std::string& a_mark) const;

	// ��������� �������� ���������
	const std::string& get_name() const;

	// ��������� ������� ������� �� ���������
	const std::map<std::string, DimensionMark*>& get_marks() const;

	~Dimension();

private:
	// ������ ������� �� ���������
	// ����: �������� �������
	std::map<std::string, DimensionMark*> m_marks_map;
	// �������� ���������
	std::string m_name;
	// ������ ��������� ����� ������
	std::vector<FactClassifier*> m_points;
};