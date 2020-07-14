#include <iostream>
#include <SFML/Graphics.hpp>
#include <fstream>
#include <algorithm>

enum class Dir { up, down, left, right };


bool deadCheck(sf::Vector2i &head, sf::Vector2i &filedsize, std::vector<sf::Vector2i> &snake) {
	if (head.x < 0 || head.x >= filedsize.x || head.y < 1 || head.y >= filedsize.y) {
		return true;
	}
	else if (std::find(snake.begin(), snake.end(), head) != snake.end()) {
		//撞到身體
		return true;
	}
	return false;
}

Dir getDir(sf::Vector2i prev, sf::Vector2i next) {
	auto difference = next - prev;
	if (difference.x == 0 && difference.y < 0)
		return Dir::up;
	else if (difference.x == 0 && difference.y > 0)
		return Dir::down;
	else if (difference.x > 0 && difference.y == 0)
		return Dir::right;
	else if (difference.x < 0 && difference.y == 0)
		return Dir::left;
	else throw std::exception("error direction");
}

int main() {
	sf::Vector2i filedsize(20, 15);
	std::ifstream inFile("score.txt"); //用來記歷史最高分
	int highscore;
	inFile >> highscore;
	inFile.close();


	//用來記歷史最高分
	sf::String hscoreString = (L"歷史最高分：");
	sf::String nowscoreString = (L"現在分數：");
	sf::String timerString = (L"TIME：");
	int score = 0; //記錄這場的分數

	sf::Font f;
	if (f.loadFromFile("jf-openhuninn-1.0.ttf") == false) {
		return EXIT_FAILURE;
	}

	sf::Text text1(timerString, f, 20);
	sf::Text text2(hscoreString, f, 20);
	sf::Text text3(nowscoreString, f, 20);
	text2.setPosition(380, 0);
	text3.setPosition(180, 0);



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


	//記分板跟時間區塊
	sf::RectangleShape rectangle(sf::Vector2f(blocksize.x * 20, blocksize.y * 1));
	rectangle.setFillColor(sf::Color::Transparent);
	rectangle.setOutlineThickness(3);
	rectangle.setOutlineColor(sf::Color(250, 150, 100));

	bool isDead = false;
	sf::VideoMode mode(
		unsigned int(filedsize.x * blocksize.x),
		unsigned int(filedsize.y * blocksize.y));

	sf::RenderWindow w(mode, L"貪吃蛇");

	Dir d = Dir::left;
	Dir nextD = d;

	sf::Vector2i nextHead = snake[0] + sf::Vector2i(-1, 0);


	sf::Clock clock;
	sf::Clock time;
	sf::Clock deadTimer;//讓死亡後按下按鍵不會立刻重新開始時間


	int output_time = time.getElapsedTime().asSeconds();
	float speed = 1.0f;
	while (w.isOpen())
	{
		sf::Event evt;
		float spf = 1. / speed;
		
		if (w.pollEvent(evt)) {
			if (evt.type == sf::Event::Closed) {
				w.close();
			}
			if (evt.type == sf::Event::KeyPressed) {
				if (evt.key.code == sf::Keyboard::Up) {
					nextD = Dir::up;
				}
				if (evt.key.code == sf::Keyboard::Down) {
					nextD = Dir::down;
				}
				if (evt.key.code == sf::Keyboard::Left) {
					nextD = Dir::left;
				}
				if (evt.key.code == sf::Keyboard::Right) {
					nextD = Dir::right;
				}
				
			}
		}


		if (clock.getElapsedTime().asSeconds() >= spf) {

			if (!isDead) {
				output_time = time.getElapsedTime().asSeconds();

				sf::Vector2i head = snake[0];

				head = nextHead;

				snake.insert(snake.begin(), head);

				//判定食物碰撞
				if (food == head) {
					std::cout << "eaten" << std::endl;
					
					food = { rand() % filedsize.x, rand() % (filedsize.y - 1) + 1 };
					speed += 1.0f; //吃到越多速度越快
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

				if (nextD == Dir::up)
					nextHead.y--;
				else if (nextD == Dir::down)
					nextHead.y++;
				else if (nextD == Dir::left)
					nextHead.x--;
				else if (nextD == Dir::right)
					nextHead.x++;

				if (deadCheck(nextHead, filedsize, snake)) {
					isDead = true;
					deadTimer.restart();
				}

				d = nextD;
			}
			clock.restart();
		}

		


		if (isDead) {
			w.clear(sf::Color::Red);
			
			if (deadTimer.getElapsedTime().asMilliseconds() > 500 && evt.type == sf::Event::KeyPressed) {
				isDead = false;
				score = 0;
				snake = { sf::Vector2i(3, 4) };
				speed = 1;
				output_time = 0;
				time.restart();
			}
		}
		else {
			w.clear();
		}
		w.draw(rectangle);

		text1.setString(timerString + std::to_string(output_time));
		w.draw(text1);
		
		text2.setString(hscoreString + std::to_string(highscore));
		w.draw(text2);

		text3.setString(nowscoreString + std::to_string(score));
		w.draw(text3);

		//食物
		sf::Vector2f foodpos(
			food.x * blocksize.x,
			food.y * blocksize.y);
		block.setPosition(foodpos);
		block.setColor(sf::Color::Red); //食物畫成紅色的
		w.draw(block);

		//讓蛇的移動變滑順，不是格一格的
		float interpolationPosition = ((isDead ? 1: clock.getElapsedTime().asMilliseconds()) / (spf * 1000));;
		float interpolationX = 0, interpolationY = 0;

		block.setColor(sf::Color::White);
		for (int i = snake.size() - 1; i >= 0; i--) {
			auto nowDir = i != 0 ? getDir(snake[i], snake[i - 1]) : d;
			auto body = snake[i];

			interpolationX = interpolationY = 0;

			switch (nowDir)
			{
			case Dir::up:
				interpolationY = -interpolationPosition;
				break;
			case Dir::down:
				interpolationY = interpolationPosition;
				break;
			case Dir::left:
				interpolationX = -interpolationPosition;
				break;
			case Dir::right:
				interpolationX = interpolationPosition;
				break;
			default:
				break;
			}
			sf::Vector2f bodypos(
				(body.x + interpolationX)* blocksize.x,
				(body.y + interpolationY)* blocksize.y);
			block.setPosition(bodypos);
			w.draw(block);
		}


		w.display();

	}
	return EXIT_SUCCESS;

}
