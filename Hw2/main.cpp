#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>

int main() {
	int window_width = 800;
	int window_height = 600;
	sf::Font f;
	if (f.loadFromFile("jf-openhuninn-1.0.ttf") == false) {
		return EXIT_FAILURE;
	}
	sf::VideoMode mode(window_width, window_height);//�����j�p
	sf::String title("HI!");//�i�H�������૬
	sf::RenderWindow w(mode, title);


	sf::Text text1("Hello", f, 50);
	sf::Text text2 = text1;
	//text1,text2�h���
	float text_width = text1.getGlobalBounds().width; //����text���e��
	float speedx = 0.05f;
	float speedy = 0;

	float oringin_point_offset = 5;

	//text1.setFillColor(sf::Color::Blue);
	text2.setPosition(-text_width - oringin_point_offset, 0);


	sf::Event evt; //�ǰѩI�s ���ƻs


	while (w.isOpen()) //�ˬd�O�_�}��
	{


		//ref���i�O�Ȯɪ���
		if (w.pollEvent(evt)) {  //���ͨƥ�
			//evt�s��ϥΪ̪��ƥ�
			if (evt.type == sf::Event::Closed) {
				std::cout << "close" << std::endl;
				w.close();
			}
		}

		w.clear();

		w.draw(text1);
		w.draw(text2);
		w.display();

		//std::cout << text1.getPosition().x << std::endl;



		/*text.getPosition().x;
		if (text.getPosition().x > 400) { //�Ĥ@�� �W�L�F�|�^��̥���
			std::cout << "reach" << std::endl;
			text.setPosition(text.getOrigin());
		}
		text.move(0.1, 0);
		*/


		//if (text1.getPosition().x > window_width) { //�ĤG�� �W�L�F�|�C�C�X��
		//	std::cout << "reach" << std::endl;
		//	text1.setPosition(-text_width,0);
		//}

		if (text1.getPosition().x + text_width > window_width) {
			text2.move(speedx, speedy);
		}

		if (text1.getPosition().x > window_width) {
			text1.setPosition(-oringin_point_offset, 0);
			text2.setPosition(-text_width - oringin_point_offset, 0);
		}

		text1.move(speedx, speedy);

	}
	return 0;

}