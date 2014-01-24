#ifndef MAP_HPP
#define MAP_HPP
#include "commons.hpp"

class Map : public GameObject {
	public:
		class Cube {
			public:
				enum Color {
					WHITE,
					RED,
					GREEN,
					BLUE
				};

				enum Type {
					SPIKES,
					FLOOR,
					BUMP
				};

				Cube(Color col);

			private:
				Color col;
				friend class Map;
		};

		Map(const std::string& mapfile);
		~Map();

	private:
		std::vector<std::vector <Cube> > map;
};

#endif // MAP_HPP
