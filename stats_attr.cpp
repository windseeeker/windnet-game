#include "stats_attr.h"

void StatsAttr::addStatAttr(short type, double val) {
	std::map<short, double>::iterator it = m_statsAttr.find(type);
	if (it == m_statsAttr.end()) {
		m_statsAttr[type] = val;
		return;
	}
	it->second += val;
}

void StatsAttr::updateStatAttr(short type, double val) {
	std::map<short, double>::iterator it = m_statsAttr.find(type);
	if (it == m_statsAttr.end()) {
		return;
	}
	it->second = val;
}

double StatsAttr::getStatAttr(short type) {
	std::map<short, double>::iterator it = m_statsAttr.find(type);
	return it == m_statsAttr.end() ? -1 : it->second;
}
