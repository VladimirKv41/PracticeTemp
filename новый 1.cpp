std::map<std::pair<const std::string, const std::string>, std::vector<DataPoint*>> temp_map = m_selection_points;
if (!m_selection_points.empty()) {
	m_selection_points.clear();
}
for (auto it_list = a_positions_list.begin(); it_list != a_positions_list.end(); it_list++) {
	// Если Выборка уже заполнена, то новая Выборка составляется из уже имеющихся ТочекДанных
	//if (m_selection_points.find(std::make_pair(a_dim_name, *it_list)) == m_selection_points.end())
	//	continue;
	if (!m_selection_points.empty()) {
		if (temp_map.find(std::make_pair(a_dim_name, *it_list)) == temp_map.end())
			continue;
		m_selection_points[std::make_pair(a_dim_name, *it_list)] = temp_map[std::make_pair(a_dim_name, *it_list)];
	}
	// Если не заполнена, то берутся все ТочкиДанных указанного измерения
	else {
		if ((*it_dim)->get_positions().find(*it_list) == (*it_dim)->get_positions().end())
			continue;
		m_selection_points[std::make_pair(a_dim_name, *it_list)] = (*it_dim)->get_positions().find(*it_list)->second->get_DataPoints();
	}	
	for (auto it_dpoint = m_selection_points[std::make_pair(a_dim_name, *it_list)].begin(); it_dpoint != m_selection_points[std::make_pair(a_dim_name, *it_list)].end(); it_dpoint++) {
		// Итерация по ТочкамДанных из вектора
		for (auto it_dpoint2 = (*it_dpoint)->get_Fact()->get_DataPoints().begin(); it_dpoint2 != (*it_dpoint)->get_Fact()->get_DataPoints().end(); it_dpoint2++) {
			if ((*it_dpoint2)->get_Dimension()->get_name() != a_dim_name) {
				m_selection_points[std::make_pair((*it_dpoint2)->get_Dimension()->get_name(), (*it_dpoint2)->get_dim_position_name())].push_back(*it_dpoint2);
			}
		}
	}
}