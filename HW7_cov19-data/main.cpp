#include <SFML/Graphics.hpp>
#include <nlohmann/json.hpp>

int main(){
	const std::string data =
		"{\"TotalConfirmed\": 20619022, \"TotalDeaths\" : 749323, \"TotalRecovered\" : 12825953}";

	nlohmann::json j = nlohmann::json::parse(data);
	int con = j["TotalConfirmed"];
	int death = j["TotalDeaths"];

	sf::Font font;
	if(font.loadFromFile("jf-openhuninn-1.0.ttf") == false) {
		return EXIT_FAILURE;
	}

	sf::String conString(std::to_string(con));
	sf::Text conText(conString, font);
	conText.setPosition(200, 20);

	sf::String deaString(std::to_string(death));
	sf::Text deaText(deaString, font);
	deaText.setPosition(200, 80);


	sf::Texture confirmedTexture;
	if (!confirmedTexture.loadFromFile("confirmed.png")) {
		return EXIT_FAILURE;
	}

	sf::Texture deathTexture;
	if (!deathTexture.loadFromFile("death.png")) {
		return EXIT_FAILURE;
	}

	sf::Sprite ConfirmedSprite(confirmedTexture);
	ConfirmedSprite.setPosition(15, 20);
	sf::Sprite deathdSprite(deathTexture);
	deathdSprite.setPosition(15, 80);



	sf::RenderWindow window(sf::VideoMode(400, 160), L"COV-19");
	while (window.isOpen()) {
		sf::Event evt;
		if (window.pollEvent(evt)) {
			if (evt.type == sf::Event::Closed) {
				window.close();
			}
		}
		window.clear();
		window.draw(ConfirmedSprite);
		window.draw(deathdSprite);
		window.draw(conText);

		window.draw(deaText);

		window.display();


	}


}