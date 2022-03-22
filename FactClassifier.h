#pragma once
#include <string>

class Dimension;
class DimensionMark;
class Fact;

/**
 * @brief ����� ������.
 *
 * �����, ������������ ��� ����� ������� �� ����������.
 * ���������� �������������� ����� � ���������,
 * ������ ��� ����������� �������.
 */
class FactClassifier {
public:
	FactClassifier(Fact* a_fact, Dimension* a_dim, DimensionMark* a_dim_mark);

	// ��������� ���������� ���������
	const Dimension* const get_Dimension() const;

	// ��������� ���������� �����
	Fact* const get_Fact() const;

	// ��������� ������� � ��������� ���������
	const std::string& get_dim_mark_name() const;

private:
	// ��������� ����/���������/������� � ���������
	Fact* m_fact;
	Dimension* m_dim;
	DimensionMark* m_dim_mark;
};