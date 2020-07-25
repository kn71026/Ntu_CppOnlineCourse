#include<SFML/Graphics.hpp>
#include<algorithm>
#include<string>
#include<random>
#include<iostream>

static std::random_device rd;
static std::default_random_engine gen = std::default_random_engine(rd());

enum class Action{
	Hold,
	MoveLeft,
	MoveRight,
	MoveDown,
	MoveDownByUser,
	MoveDownImmediate
};

enum class GameState
{
	Playing,
	Win,
	Lose
};

template<std::size_t fieldWidth, std::size_t fieldHeight>
void fieldInit(bool (&field)[fieldWidth][fieldHeight]) {
	std::uniform_int_distribution<int> distribution(0, fieldWidth - 1);

	std::fill(&field[0][0], &field[0][0] + sizeof(field), false);

	for (int y = 3; y < fieldHeight; y++) {
		for (int i = 0; i < 3; i++) {
			field[distribution(gen)][y] = true;
		}
	}
}

sf::Vector2f getPixelPosition(const sf::Vector2i& pos,const sf::Vector2u& blockSize) {
	return sf::Vector2f(float(pos.x * blockSize.x), float(pos.y * blockSize.y));
}

int main() {
	const int fieldWidth = 6;
	const int fieldHeight = 10;
	const sf::IntRect fieldBound(0, 0, fieldWidth, fieldHeight);
	bool field[fieldWidth][fieldHeight];
	fieldInit(field);

	sf::Texture blockTexture;
	blockTexture.loadFromFile("block.png");
	sf::Vector2u blockSize(blockTexture.getSize());

	const unsigned int windowWidth = fieldWidth * blockSize.x;
	const unsigned int windowHeight = fieldHeight * blockSize.y;
	const sf::String title(L"簡易消行遊戲");
	sf::RenderWindow window(sf::VideoMode(windowWidth,windowHeight), title);
	
	sf::Vector2i origin(fieldWidth / 2, 0);
	sf::Vector2i pos(origin);
	sf::Sprite block(blockTexture);
	block.setPosition(getPixelPosition(pos, blockSize));


	sf::Font f;
	if (f.loadFromFile("jf-openhuninn-1.0.ttf") == false) {
		return EXIT_FAILURE;
	}
	sf::Text winLoseText("", f, 60U);
	sf::Text timeText("", f, 20U);
	sf::Text scoreText("", f, 20U);

	timeText.setFillColor(sf::Color::Green);
	scoreText.setPosition(100,0);
	int score = 0; //記錄這場的分數

	sf::Clock clock;
	sf::Clock winLoseTimer;//讓贏或失敗後按下按鍵不會立刻重新開始時間
	sf::Clock gameTimer;

	int time = 0;
	GameState gameState = GameState::Playing;
	while (window.isOpen()){
		Action action = Action::Hold;

		sf::Event evt;
		if (window.pollEvent(evt)) {
			if (evt.type == sf::Event::Closed)
				window.close();
			if (evt.type == sf::Event::KeyPressed) {
				switch (evt.key.code) {
				case sf::Keyboard::Left:
					action = Action::MoveLeft;
					break; 
				case sf::Keyboard::Right:
					action = Action::MoveRight;
					break;
				case sf::Keyboard::Down:
					action = Action::MoveDownByUser;
					break;
				case sf::Keyboard::Space :
					action = Action::MoveDownImmediate;
					break;
				}
			}
		}
		if (gameState == GameState::Playing) {
			time = gameTimer.getElapsedTime().asSeconds();
			if (clock.getElapsedTime().asSeconds() >= 0.5f) {
				action = Action::MoveDown;
				clock.restart();
			}

			sf::Vector2i nextPos(pos);
			switch (action) {
			case Action::Hold:
				break;
			case Action::MoveLeft:
				nextPos.x = (nextPos.x + fieldWidth - 1) % fieldWidth;
				break;
			case Action::MoveRight:
				nextPos.x = (nextPos.x + 1) % fieldWidth;
				break;
			case Action::MoveDown:
			case Action::MoveDownByUser:
				nextPos.y++;
				break;
			case Action::MoveDownImmediate:
				while (fieldBound.contains(nextPos) && field[nextPos.x][nextPos.y] == false)
					nextPos.y++;
				break;
			}
			if (fieldBound.contains(nextPos) && field[nextPos.x][nextPos.y] == false)
				pos = nextPos;
			else if (action == Action::MoveDown || action == Action::MoveDownByUser || action == Action::MoveDownImmediate) {
				//因為立即掉下去的時候要銷的不是原本的那列
				pos.y = nextPos.y - 1;
				field[pos.x][pos.y] = true;

				bool isWin = true;
				bool isFull = true;
				for (int x = 0; x < fieldWidth; x++) {
					if (field[x][pos.y] == false) {
						isFull = false;
						break;
					}
				}

				if (isFull) {
					score++;
					//std::cout << score << std::endl;
					// 如果滿了就把這列銷掉，並讓上面的所有格子往下一列
					for (int x = 0; x < fieldWidth; x++)
						for (int y = pos.y; y > 0; y--)
							field[x][y] = field[x][y - 1];
						
					for (int x = 0; x < fieldWidth; x++)
						field[x][0] = false;

					for (int x = 0; x < fieldWidth; x++)
						for (int y = 0; y < fieldHeight; y++)
							if (field[x][y])
								isWin = false;
				}

				isWin = isFull && isWin;
				if (isWin) {
					gameState = GameState::Win;
					winLoseTimer.restart();
				}
				else if (field[origin.x][origin.y]) {
					gameState = GameState::Lose;
					winLoseTimer.restart();
				}
				else
					pos = origin;
			}
		}

		window.clear();


		for (int x = 0; x < fieldWidth; x++) {
			for (int y = 0; y < fieldHeight; y++) {
				if (field[x][y]) {
					block.setPosition(getPixelPosition(sf::Vector2i(x, y), blockSize));
					window.draw(block);
				}
			}
		}
		if (gameState == GameState::Playing) {
			block.setPosition(getPixelPosition(pos, blockSize));
			window.draw(block);
		}
		else {
			if (winLoseTimer.getElapsedTime().asSeconds() > 0.5f && evt.type == sf::Event::KeyPressed) {
				clock.restart();
				gameTimer.restart();
				gameState = GameState::Playing;
				score = 0;
				fieldInit(field);
			}

			if (gameState == GameState::Win) {
				winLoseText.setString("Win");
				winLoseText.setFillColor(sf::Color::Yellow);
			}
			else {
				winLoseText.setString("Lose");
				winLoseText.setFillColor(sf::Color::Red);
			}
			winLoseText.setPosition(windowWidth / 2.f - winLoseText.getGlobalBounds().width / 2.f,
				windowHeight / 2.f - winLoseText.getGlobalBounds().height);
			window.draw(winLoseText);
		}

		timeText.setString("Time: " + std::to_string(time));
		scoreText.setString("score: " + std::to_string(score));


		window.draw(timeText);
		window.draw(scoreText);


		window.display();
	}
	return EXIT_SUCCESS;
}