#include <SFML/Graphics.hpp>
int main()
{
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(800, 600), L"我的第一個視窗程式");
    // Load a sprite to display

    sf::Font font;
    if (!font.loadFromFile("jf-openhuninn-1.0.ttf"))
        return EXIT_FAILURE;
    sf::Text text(L"Hello 你好", font, 50);
    // Load a music to play

    // Start the game loop
    while (window.isOpen())
    {
        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Close window: exit
            if (event.type == sf::Event::Closed)
                window.close();
        }
        // Clear screen
        window.clear();
        // Draw the sprite

        // Draw the string
        window.draw(text);
        // Update the window
        window.display();
    }
    return EXIT_SUCCESS;
}