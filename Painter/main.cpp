#include <SFML/Graphics.hpp>

int main() {
	sf::VideoMode mode(642, 640);
	sf::RenderWindow window(mode, L"¤pµe®a");
	sf::Texture background;
	background.loadFromFile("background.png");

	sf::Sprite backgroundSprite(background);

	while ( window.isOpen() ) {
		sf::Event evt;
		if (window.pollEvent(evt)) {
			if (evt.type == sf::Event::Closed) {
				window.close();
			}
		}

		window.clear();
		window.draw(backgroundSprite);
		window.display();

	}
}