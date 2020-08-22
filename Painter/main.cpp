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
	cursor.setOrigin(10, 10);

	sf::RenderTexture canvas;
	canvas.create(600, 560);
	canvas.clear(sf::Color::White);


	sf::Vector2f canvasOffset(20, 60);
	sf::Sprite canvasSprite(canvas.getTexture());
	canvasSprite.setPosition(canvasOffset);

	while ( window.isOpen() ) {
		sf::Event evt;
		if (window.pollEvent(evt)) {
			if (evt.type == sf::Event::Closed) {
				window.close();
			}
		}

		sf::Vector2i mousePos = sf::Mouse::getPosition(window);
		cursor.setPosition(sf::Vector2f(mousePos) - canvasOffset);

		canvas.draw(cursor);
		canvas.display();

		window.clear(sf::Color::White);
		window.draw(backgroundSprite);
		window.draw(canvasSprite);
		cursor.setPosition(sf::Vector2f(mousePos));
		window.draw(cursor);
		window.display();

	}
}