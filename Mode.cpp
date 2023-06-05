#include "Mode.hpp"
#include <stdexcept>

#include <iostream>
#include <sstream>
#include <vector>
#include <fstream>
#include <string>

Mode::Mode() : score(0), prefixCount(0), suffixCount(0), matchingCount(0) {

}

static std::string::size_type hexValueNoException(char c) {
	const std::string hex = "0123456789abcdef";
	const std::string::size_type ret = hex.find(tolower(c));
	return ret;
}

Mode Mode::matching(std::string input) {
	Mode r;
	std::vector<std::string> list;

	std::string prefix = input.substr(0, 2);

	if(input.size() == 42 && prefix == "0x") {
		list.push_back(input.substr(2));
	} else {
		std::ifstream file(input);
		if (file.is_open()) {
			std::string line;
			while (std::getline(file, line)) {
				if(line.size() == 42) {
					list.push_back(line.substr(2));
				}
			}
		} else {
			std::cout << "error: Failed to open matching file. :<" << std::endl;
		}
	}
	
	if(list.size() > 0) {
		r.matchingCount = list.size();
		for( size_t j = 0; j < list.size(); j += 1) {
			const std::string item = list[j];
			for( size_t i = 0; i < item.size(); i += 2 ) {
				const size_t indexHi = hexValueNoException(item[i]);
				const size_t indexLo = (i + 1) < item.size() ? hexValueNoException(item[i+1]) : std::string::npos;
				const unsigned long valHi = (indexHi == std::string::npos) ? 0 : indexHi << 4;
				const unsigned long valLo = (indexLo == std::string::npos) ? 0 : indexLo;
				const int maskHi = (indexHi == std::string::npos) ? 0 : 0xF << 4;
				const int maskLo = (indexLo == std::string::npos) ? 0 : 0xF;
				r.data1.push_back(maskHi | maskLo);
				r.data2.push_back(valHi | valLo);
			}
		}
	}

	return r;
}
