#include <iostream>
#include <SFML/Graphics.hpp>
#include <fstream>

int main() {
	sf::Vector2i filedsize(20, 15);
	std::ifstream inFile("score.txt"); //用來記歷史最高分
	int highscore;
	inFile >> highscore;
	inFile.close();

	//用來記歷史最高分
	sf::String hscoreString = (L"歷史最高分：");
	sf::String nowscoreString = (L"現在分數：");
	int score = 0; //記錄這場的分數
	sf::Font f;
	sf::String timer = "time:";

	if (f.loadFromFile("jf-openhuninn-1.0.ttf") == false) {
		return EXIT_FAILURE;
	}
	sf::Text text1(timer, f, 20);
	sf::Text text2(hscoreString, f, 20);
	text2.setPosition(380, 0);

	sf::Text text3(nowscoreString, f, 20);
	text3.setPosition(200, 0);



	sf::Texture blocktexture;
	blocktexture.loadFromFile("block.png");

	if (!blocktexture.loadFromFile("block.png")) {
		std::cout << "not found" << std::endl;
		return EXIT_FAILURE;
	}


	std::vector<sf::Vector2i> snake = { sf::Vector2i(3, 4) };


	sf::Vector2i food(rand() % filedsize.x, rand() % filedsize.y);

	sf::Sprite block(blocktexture);

	sf::Vector2f blocksize(
		block.getLocalBounds().width,
		block.getLocalBounds().height);
	bool isDead = false;
	sf::VideoMode mode(
		unsigned int(filedsize.x * blocksize.x),
		unsigned int(filedsize.y * blocksize.y));

	sf::RenderWindow w(mode, L"貪吃蛇");

	enum class Dir { up, down, left, right };
	Dir d = Dir::left;

	sf::Clock clock;
	sf::Clock time;

	int output_time = time.getElapsedTime().asSeconds();
	float speed = 1.0f;
	while (w.isOpen())
	{
		sf::Event evt;
		if (w.pollEvent(evt)) {
			if (evt.type == sf::Event::Closed) {
				w.close();
			}
			if (evt.type == sf::Event::KeyPressed) {
				if (evt.key.code == sf::Keyboard::Up) {
					d = Dir::up;
				}
				if (evt.key.code == sf::Keyboard::Down) {
					d = Dir::down;
				}
				if (evt.key.code == sf::Keyboard::Left) {
					d = Dir::left;
				}
				if (evt.key.code == sf::Keyboard::Right) {
					d = Dir::right;
				}
			}
		}


		if (clock.getElapsedTime().asSeconds() >= 1 / speed) {

			sf::Vector2i head = snake[0];

			if (d == Dir::up) {
				head.y--;
			}
			if (d == Dir::down) {
				head.y++;
			}
			if (d == Dir::left) {
				head.x--;
			}
			if (d == Dir::right) {
				head.x++;
			}

			if (head.x < 0 || head.x >= filedsize.x ||
				head.y < 0 || head.y >= filedsize.x) {
				isDead = true;
			}

			if (!isDead) {
				snake.insert(snake.begin(), head);
				//判定食物碰撞
				if (food == head) {
					std::cout << "eaten" << std::endl;
					food = { rand() % filedsize.x, rand() % filedsize.y };
					speed += 1.0f;
					score++;
					if (score > highscore) {
						std::ofstream outFile("score.txt", std::ios::out | std::ios::trunc);
						outFile << score;
						highscore = score;
						outFile.close();
					}
				}
				else {
					snake.pop_back();
				}
				output_time = time.getElapsedTime().asSeconds();

			}
			clock.restart();
		}

		if (isDead) {
			w.clear(sf::Color::Red);
			if (evt.type == sf::Event::KeyPressed) {
				isDead = false;
				clock.restart();
				score = 0;
				snake = { sf::Vector2i(3, 4) };
				speed = 1;
			}
		}
		else {
			w.clear();
		}
		text1.setString(timer + std::to_string(output_time));
		w.draw(text1);

		text2.setString(hscoreString + std::to_string(highscore));

		w.draw(text2);

		text3.setString(nowscoreString + std::to_string(score));
		w.draw(text3);


		sf::Vector2f foodpos(
			food.x * blocksize.x,
			food.y * blocksize.y);
		block.setPosition(foodpos);
		w.draw(block);



		for (const sf::Vector2i& body : snake) {

			sf::Vector2f bodypos(
				body.x * blocksize.x,
				body.y * blocksize.y);
			block.setPosition(bodypos);
			w.draw(block);
		}

		w.display();

	}
	return EXIT_SUCCESS;

}
