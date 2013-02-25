#include <SFML/Graphics.hpp>
#include <array>
#include <iostream>

const int bounds = 100;
const float scale = 10;

typedef std::array<bool, bounds> Row;
typedef std::array<Row, bounds> Map;
Map map;

int getAliveNeighborCount(int x, int y);

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Game of Life");

	// init the map
	Row empty;
	empty.fill(false);
	map.fill(empty);

	// repeatedly used
	const sf::Vector2f size(scale, scale);
	const sf::Color aliveColor(255, 255, 255);

	int generationCounter = 0;
	bool running = false;

	// main cycle
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Pause)
				running = !running;
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R)
				map.fill(empty);

			if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
				int x = static_cast<int>(event.mouseButton.x / scale);
				int y = static_cast<int>(event.mouseButton.y / scale);

				if (x > 0 && x < bounds && y > 0 && y < bounds) {
					map[x][y] = !map[x][y];
				}
			}

			if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Right) {
				int x = static_cast<int>(event.mouseButton.x / scale);
				int y = static_cast<int>(event.mouseButton.y / scale);

				if (x > 0 && x < bounds && y > 0 && y < bounds) {
					std::cout << std::to_string(getAliveNeighborCount(x, y)) << " neighbors" << std::endl;
				}
			}
        }

		// game mechanics
		if (running) {
			Map tmp;
			tmp.fill(empty);

			for (int i = 0; i < bounds; ++i) {
				for (int j = 0; j < bounds; ++j) {
					int neighbors = getAliveNeighborCount(i, j);

					if (neighbors < 2 || neighbors > 3) { // die motherfucker
						tmp[i][j] = false;
					} else if (neighbors == 3) {
						tmp[i][j] = true;
					} else {
						tmp[i][j] = map[i][j];
					}
				}
			}

			map = tmp;
			++generationCounter;
		}

		// rendering
        window.clear();

		for (int i = 0; i < bounds; ++i) {
			for (int j = 0; j < bounds; ++j) {
				if (map[i][j]) { // no need to draw dead as they are black
					sf::RectangleShape r(size);
					r.setPosition(i * scale, j * scale);
					r.setFillColor(aliveColor);
					window.draw(r);
				}
			}
		}

		sf::Font f;
		f.loadFromFile("arial.ttf");
		sf::String str = "generation " + std::to_string(generationCounter);
		sf::Text s(str, f);
		s.setPosition(700, 10);
		s.setCharacterSize(15);
		window.draw(s);

        window.display();
    }

    return 0;
}

int getAliveNeighborCount(int x, int y) {
	int result = 0;
	for (int dx = -1; dx <= 1; ++dx) {
		for (int dy = -1; dy <= 1; ++dy) {
			if (dx == 0 && dy == 0) continue;
			if (x + dx < 0 || x + dx >= bounds || y + dy < 0 || y + dy >= bounds) continue;
			if (map[x + dx][y + dy]) ++result;
		}
	}
	return result;
}