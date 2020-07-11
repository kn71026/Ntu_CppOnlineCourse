#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <string>


int main() {
	int window_width = 800;
	int window_height = 600;
	sf::Font f;
	if (!f.loadFromFile("jf-openhuninn-1.0.ttf")) {
		return EXIT_FAILURE;
	}

	std::ifstream inFile("text.txt");
	std::string line;
	getline(inFile, line);
	std::cout << line << std::endl;
	inFile.close();

	enum class DIR { LEFT_TO_RIGHT, RIGHT_TO_LEFT };
	enum class SPEED { FASTFAST = 20, FAST = 15, MID = 10, SLOW = 5, SLOWSLOW = 1 };

	sf::RenderWindow w(sf::VideoMode(800, 600), line);

	DIR dir = DIR::LEFT_TO_RIGHT;
	float speed = (float)SPEED::MID;

	sf::Text text1(line, f, 50);
	text1.setFillColor(sf::Color(0, 255, 0));
	sf::CircleShape circle(2);
	circle.setFillColor(sf::Color(0, 0, 0));


	if (dir == DIR::RIGHT_TO_LEFT) {
		text1.setPosition((float)w.getSize().x, 0);
	}

	//text1.setFillColor(sf::Color::Blue);
	//text2.setPosition(-text_width-oringin_point_offset, 0);


	sf::Event evt; //傳參呼叫 不複製



	while (w.isOpen()) //檢查是否開著
	{


		//ref不可是暫時物件
		if (w.pollEvent(evt)) {  //產生事件
			//evt存放使用者的事件
			if (evt.type == sf::Event::Closed) {
				std::cout << "close" << std::endl;
				w.close();
			}
			if (evt.type == sf::Event::KeyPressed) {
				if (evt.key.code == sf::Keyboard::Key::Left) {
					dir = DIR::RIGHT_TO_LEFT;
				}
				else if (evt.key.code == sf::Keyboard::Key::Right) {
					dir = DIR::LEFT_TO_RIGHT;
				}
				else if (evt.key.code == sf::Keyboard::Key::Num1 || evt.key.code == sf::Keyboard::Key::Numpad1)
					speed = (float)SPEED::FASTFAST;
				else if (evt.key.code == sf::Keyboard::Key::Num2 || evt.key.code == sf::Keyboard::Key::Numpad2)
					speed = (float)SPEED::FAST;
				else if (evt.key.code == sf::Keyboard::Key::Num3 || evt.key.code == sf::Keyboard::Key::Numpad3)
					speed = (float)SPEED::MID;
				else if (evt.key.code == sf::Keyboard::Key::Num4 || evt.key.code == sf::Keyboard::Key::Numpad4)
					speed = (float)SPEED::SLOW;
				else if (evt.key.code == sf::Keyboard::Key::Num5 || evt.key.code == sf::Keyboard::Key::Numpad5)
					speed = (float)SPEED::SLOWSLOW;
				else if (evt.key.code == sf::Keyboard::Key::Add || evt.key.code == sf::Keyboard::Key::Equal)
					speed *= 2;
				else if (evt.key.code == sf::Keyboard::Key::Subtract || evt.key.code == sf::Keyboard::Key::Hyphen)
					speed /= 2;



			}

		}

		w.clear();
		w.draw(text1);
		for (unsigned int i = 0; i < w.getSize().x; i += 4) {
			for (unsigned int j = 0; j < w.getSize().y; j += 4) {
				circle.setPosition((float)i, (float)j);
				w.draw(circle);
			}
		}
		w.display();




		switch (dir)
		{
		case DIR::LEFT_TO_RIGHT:
			text1.move(speed, 0);
			break;
		case DIR::RIGHT_TO_LEFT:
			text1.move(-speed, 0);
			break;
		}



	}
	return 0;

}