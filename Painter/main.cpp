#include <SFML/Graphics.hpp>
#include <iostream>

int main() {
	sf::VideoMode mode(642, 640);
	sf::RenderWindow window(mode, L"¤pµe®a");
	sf::Texture background;
	background.loadFromFile("background.png");

	sf::Sprite backgroundSprite(background);
	backgroundSprite.setPosition(0, 40);


	sf::CircleShape cursor(10);
	cursor.setFillColor(sf::Color::Black);
	while ( window.isOpen() ) {
		sf::Event evt;
		if (window.pollEvent(evt)) {
			if (evt.type == sf::Event::Closed) {
				window.close();
			}
		}

		sf::Vector2i mousePos = sf::Mouse::getPosition(window);
		cursor.setPosition(sf::Vector2f(mousePos));
		window.clear(sf::Color::White);
		window.draw(backgroundSprite);
		window.draw(cursor);
		window.display();

	}
}