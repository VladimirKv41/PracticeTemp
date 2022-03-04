#pragma once
#include <string>

class Dimension;
class DimensionPosition;
class Fact;

/**
 * @brief ����� ������.
 * 
 * ����� ����������� ��������� � ���� ��� ����������� ��������������.
 */
class DataPoint {
public:
	DataPoint(Fact* a_fact, Dimension* a_dim, DimensionPosition* a_dim_position);

	// ��������� ���������� ���������
	const Dimension* const get_Dimension() const;

	// ��������� ���������� �����
	Fact* const get_Fact() const;

	// ��������� ������� � ��������� ���������
	const std::string& get_dim_position_name() const;
	
private:
	// ��������� ����/���������/������� � ���������
	Fact* m_fact;
	Dimension* m_dim;
	DimensionPosition* m_dim_position;
};