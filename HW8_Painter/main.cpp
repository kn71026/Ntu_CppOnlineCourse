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


	std::vector<sf::Color> Colors = {
		sf::Color::White,
		sf::Color::Blue,
		sf::Color::Red,
		sf::Color::Green,
		sf::Color::Yellow,
		sf::Color::Cyan,
		sf::Color::Magenta,
		sf::Color::Black

	};

	std::vector<sf::RectangleShape> swatches;

	int swatchOffset = 20;


	for (auto color : Colors)
	{
		sf::RectangleShape swatch(sf::Vector2f(20, 20));
		swatch.setFillColor(color);
		swatch.setOutlineColor(sf::Color(230, 230, 230));
		swatch.setOutlineThickness(2);
		swatch.setPosition(swatchOffset, 10);
		swatches.push_back(swatch);

		swatchOffset += 25;

	}

	sf::RenderTexture canvas;
	canvas.create(600, 560);
	canvas.clear(sf::Color::White);


	sf::Vector2f canvasOffset(20, 60);
	sf::Sprite canvasSprite(canvas.getTexture());
	canvasSprite.setPosition(canvasOffset);

	while (window.isOpen()) {
		sf::Event evt;
		if (window.pollEvent(evt)) {
			if (evt.type == sf::Event::Closed) {
				window.close();
			}
			if (evt.type == sf::Event::MouseButtonPressed) {
				if (evt.mouseButton.button == sf::Mouse::Left) {
					evt.mouseButton.x;
					evt.mouseButton.y;

					for (const auto& swatch : swatches) {
						if(swatch.getGlobalBounds().contains(
							evt.mouseButton.x,
							evt.mouseButton.y)){
							cursor.setFillColor(swatch.getFillColor());
}

					}
				}
			}
		}

		sf::Vector2i mousePos = sf::Mouse::getPosition(window);
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {

		cursor.setPosition(sf::Vector2f(mousePos) - canvasOffset);
		canvas.draw(cursor);
		canvas.display();
		}

		window.clear(sf::Color::White);
		window.draw(backgroundSprite);

		for (const auto& swatch : swatches) {
			window.draw(swatch);
		}
		window.draw(canvasSprite);
		cursor.setPosition(sf::Vector2f(mousePos));
		window.draw(cursor);
		window.display();

	}
}