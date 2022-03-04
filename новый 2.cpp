if (!m_selection_points.empty()) {
		std::map<std::pair<const std::string, const std::string>, std::vector<DataPoint*>> temp_map = m_selection_points;
		m_selection_points.clear();
		for (auto it_list = a_positions_list.begin(); it_list != a_positions_list.end(); it_list++) {
			m_selection_points[std::make_pair(a_dim_name, *it_list)] = temp_map[std::make_pair(a_dim_name, *it_list)];
			
			for (auto it_dpoint = temp_map[std::make_pair(a_dim_name, *it_list)].begin(); it_dpoint != temp_map[std::make_pair(a_dim_name, *it_list)].end(); it_dpoint++) {
				for (auto it_dpoint2 = (*it_dpoint)->get_Fact()->get_DataPoints().begin(); it_dpoint2 != (*it_dpoint)->get_Fact()->get_DataPoints().end(); it_dpoint2++) {
					if ((*it_dpoint2)->get_Dimension()->get_name() != a_dim_name) {
						m_selection_points[std::make_pair((*it_dpoint2)->get_Dimension()->get_name(), (*it_dpoint2)->get_dim_position_name())].push_back(*it_dpoint2);
					}
				}
			}
		}
	}
	else {
		for (auto it_list = a_positions_list.begin(); it_list != a_positions_list.end(); it_list++) {
			auto temp = (*it_dim)->get_positions().find(*it_list)->second->get_DataPoints();
			m_selection_points[std::make_pair(a_dim_name, *it_list)] = temp;
			
			for (auto it_dpoint = temp.begin(); it_dpoint != temp.end(); it_dpoint++) {
				for (auto it_dpoint2 = (*it_dpoint)->get_Fact()->get_DataPoints().begin(); it_dpoint2 != (*it_dpoint)->get_Fact()->get_DataPoints().end(); it_dpoint2++) {
					if ((*it_dpoint2)->get_Dimension()->get_name() != a_dim_name) {
						m_selection_points[std::make_pair((*it_dpoint2)->get_Dimension()->get_name(), (*it_dpoint2)->get_dim_position_name())].push_back(*it_dpoint2);
					}
				}
			}
			
		}
	}