#include "Map.hpp"

Map::Map(const std::string &mapfile) : map(std::vector<std::vector<Cube> >(1, std::vector<Cube>())) {
	std::ifstream in(mapfile.c_str(), std::ifstream::in);
	VBE_ASSERT(in, "While parsing map: Cannot open " << mapfile );
	char c;
	int i = 0;
	while(std::cin >> c != '%') {
		if(c == '/') {
			map.push_back(std::vector<Cube>());
			++i;
		}
		else map[i] = translate_(c);
	}
}

Map::~Map() {

}
