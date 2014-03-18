#include "util.h"

#include <stdlib.h>
#include <time.h>

unsigned int getCurrentSecond() {
	return time(0);
}

int randInt(int from, int to) {
	if (from == to) {
		return from;
	}
	return from + rand() % (to - from);
}

double randDouble() {
	return randInt(0, 100) / 100.0;
}

std::vector<std::string>
split(const std::string &str, const char splitChar) {
	std::vector<std::string> ret;

	std::string val;
	for (size_t i = 0; i < str.size(); ++i) {
		if (str[i] == splitChar) {
			ret.push_back(val);
			val.clear();
			continue;
		}
		val.push_back(str[i]);
	}
	return ret;	
}
