#include "selection.h"
#include "factclassifier.h"
#include "cube.h"
#include "dimension.h"
#include "dimensionmark.h"
#include "fact.h"
#include "metric.h"
#include "variant.h"
#include <iostream>

/**
 * @brief Конструктор.
 *
 * 1. Инициализация Куба для агрегации.
 * 2. Передача ссылки на данную Выборку Кубу.
 *
 * @param [in] a_cube Куб для создания Выборки
 */
Selection::Selection(Cube* a_cube) : m_cube(a_cube), m_aggregation_dim(new Dimension("Агрегации")) {
	m_cube->m_selection = this;
}


/**
 * @brief Создание Выборки.
 *
 * 1. Проверка, существует ли Куб.
 * 2. Поиск указанного Измерения среди ранее добавленных.
 * 3. Заполнение Выборки, зависящее от того, пустая ли изначально Выборка.
 * 4. Проверка, найден ли хоть один Факт.
 *
 * @param [in] a_dim_name Название Измерения (Добавляются Классификаторы только с этим названием Измерения,
 * а также все Классификаторы связанные с этими через Факт)
 * @param [in] a_marks_list Список названий Отметок на Измерении
 * (Добавляются Классификаторы только с указанными названиями Отметок)
 * @param [in] a_metric_list Список названий Метрик
 * (Добавляются Классификаторы только с этими названиями Метрик)
 * @return enum сlass make_result Результат составления Выборки:
 * Куб не найден/неизвестное Измерение/создана/Факты не найдены
 */
make_result Selection::make(const std::string& a_dim_name, const std::initializer_list<std::string>& a_marks_list, const std::set<std::string>& a_metric_list)
{
	// Проверка, существует ли Куб
	if (m_cube == nullptr)
		return make_result::CUBE_NOT_FOUND;
	// Поиск указанного Измерения среди ранее добавленных
	std::vector<Dimension*>::const_iterator it_dim = findDimension(a_dim_name);
	if (it_dim == m_cube->m_dims.end()) {
		return make_result::UNKNOWN_DIMENSION;
	}
	// Заполнение Выборки
	if (m_selection_classifiers.empty()) {
		making((*it_dim), a_marks_list, a_metric_list);
	}
	else {
		making(a_dim_name, a_marks_list, a_metric_list);
	}
	// Найдены ли Факты
	if (m_selection_classifiers.empty())
		return make_result::NO_FACT_FOUND;
	else
		return make_result::CREATED;
}

/**
 * @brief Очистка Выборки.
 *
 * 1. Проверяется, нужно ли очищать Куб (по умолчанию - нет).
 * 2. Проверяется, есть ли добавленные Классификаторы, если да,
 * то Выборка очищается.
 *
 * @param [in] cube_cleaning Очищать ли информацию о Кубе (по умолчанию - нет)
 */
void Selection::clean(bool cube_cleaning) {
	if (cube_cleaning) {
		m_cube = nullptr;
	}
	if (!m_selection_classifiers.empty()) {
		m_selection_classifiers.clear();
		m_cube->cleanVector(m_aggregation_metrics);
		m_cube->cleanVector(m_aggregation_classifiers);
		for (class std::vector<Fact*>::iterator it = m_aggregation_facts.begin(); it != m_aggregation_facts.end(); it++) {
			delete* it;
		}
		m_aggregation_facts.clear();
	}
}

/**
 * @brief Деструктор.
 */
Selection::~Selection() {
	clean();
	delete m_aggregation_dim;
	m_aggregation_dim = nullptr;
	// Сообщение Кубу, что Выборка удалена
	if (m_cube != nullptr)
		m_cube->m_selection = nullptr;
}

/**
 * @brief Заполнение Выборки (для изначально пустой Выборки).
 *
 * Итерация по списку Отметок, каждая Отметка ищется на Измерении:
 *	1. Проверка перед добавлением, существует ли такая Отметка на Измерении.
 *	2. Добавление Классификаторов Отметки с одной из указанных Метрик.
 *
 * @param [in] a_dimension Измерения, добавляются Классификаторы из данного Измерения
 * @param [in] a_marks_list Список названий Отметок,
 * добавляются Классификаторы только с указанными названиями Отметок
 * @param [in] a_metric_list Список названий Метрик, добавляются Классификаторы только с этими названиями Метрик
 */
void Selection::making
(
	const Dimension* a_dimension,
	const std::initializer_list<std::string>& a_marks_list,
	const std::set<std::string>& a_metric_list
)
{
	for (auto it_list = a_marks_list.begin(); it_list != a_marks_list.end(); it_list++) {
		auto it_mark = a_dimension->Marks().find(*it_list);
		// Существует ли такая Отметка
		if (it_mark != a_dimension->Marks().end()) {
			if (a_metric_list.empty())
				for (auto it_fc = it_mark->second->FactClassifiers().begin(); it_fc != it_mark->second->FactClassifiers().end(); it_fc++) {
					m_selection_classifiers.push_back((*it_fc).second);
				}
			else
				for (auto it_metric = a_metric_list.begin(); it_metric != a_metric_list.end(); it_metric++) {
					auto eq_r = it_mark->second->FactClassifiers().equal_range(*it_metric);
					for (auto it_fc2 = eq_r.first; it_fc2 != eq_r.second; it_fc2++) {
						auto vector_fc = it_fc2->second->fact()->FactClassifiers();
						m_selection_classifiers.insert(m_selection_classifiers.end(), vector_fc.begin(), vector_fc.end());
					}
				}
		}
	}
}

 /**
  * @brief Заполнение Выборки (для уже заполненной Выборки).
  *
  * 1. Объявление временной Выборки для поиска необходимых Классификаторов.
  * 2. Итерация по временной Выборки:
  *	2.1. Проверка, совпадает ли Измерение с указанным.
  *	2.2. Добавление Классификаторов с одной из указанных Метрик.
  *
  * @param [in] a_dim_name Название Измерения, добавляются Классификаторы только с этим названием Измерения
  * @param [in] a_marks_list Список названий Отметок, добавляются Классификаторы только с указанными названиями Отметок
  * @param [in] a_metric_list Список названий Метрик, добавляются Классификаторы только с этими названиями Метрик
  */
  void Selection::making
  (
  	const std::string& a_dim_name,
  	const std::initializer_list<std::string>& a_marks_list,
  	const std::set<std::string>& a_metric_list
  )
  {
	  std::vector<FactClassifier*> temp_vector = m_selection_classifiers;
	  m_selection_classifiers.clear();
	  for (auto it_fc = temp_vector.begin(); it_fc != temp_vector.end(); it_fc++) {
		  if ((*it_fc)->dimension()->name() == a_dim_name) {
			  if (a_metric_list.empty()) {
					 addFactClassifiers((*it_fc), a_marks_list);
			  }
			  else {
					 addFactClassifiers((*it_fc), a_marks_list);
			  }
		  }
	  }
  }

  /**
   * @brief Добавление Классификаторов c указанными Отметками
   *
   * 1. Проверка, совпадает ли Отметка Классификатора с указанными в списке.
   * 2. Добавление Классификаторов.
   *
   * @param [in] a_fact_classifier Классификатор, 
   * с которым будут добавлены связанные Классификаторы
   * @param [in] a_marks_list Список названий Отметок
   */
   void Selection::addFactClassifiers
   (
	   const FactClassifier* a_fact_classifier,
	   const std::initializer_list<std::string>& a_marks_list
   )
   {
	   if (std::find(a_marks_list.begin(), a_marks_list.end(), a_fact_classifier->DimMarkName()) != a_marks_list.end()) {
		   auto vector_fc = a_fact_classifier->fact()->FactClassifiers();
		   m_selection_classifiers.insert(m_selection_classifiers.end(), vector_fc.begin(), vector_fc.end());
	   }
   }

	/**
	 * @brief Поиск Измерения.
	 *
	 * @param [in] a_dimension_name Название Bзмерения
	 * @return Итератор на Измерение или end()
	 */
std::vector<Dimension*>::const_iterator Selection::findDimension(const std::string& a_dimension_name) const {
	for (auto it_dim = m_cube->m_dims.begin(); it_dim != m_cube->m_dims.end(); it_dim++) {
		if ((*it_dim)->name() == a_dimension_name) {
			return it_dim;
		}
	}
	return m_cube->m_dims.end();
}