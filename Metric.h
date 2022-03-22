#pragma once
#include <string>

/**
 * @brief �������.
 *
 * ����� �������� �������� ������� ��������� ������.
 */
	class Metric {
	public:

		Metric(const std::string& a_name);

		// ��������� �������� �������
		const std::string& get_name() const;

	private:
		// �������� �������
		std::string m_name;
};