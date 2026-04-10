#include "header.h"

int main()
{

    sf::Font font1;
    font1.loadFromFile("Cabin-SemiBold.ttf");

    GameWindow gameWindow;
    Menu startingMenu("FLOPPY BIRDS", 4, {"Play","Load game","Ranking list","Settings","Exit"}, font1);
    GameState currentGameState = GameState::Starting_menu;
    int selectedIndex = 0;

    while (gameWindow.getWindow().isOpen())
    {
        sf::Event event;
        while (gameWindow.getWindow().pollEvent(event))
        {
            if (currentGameState == GameState::Starting_menu && event.type == sf::Event::KeyPressed)
            {
                if (event.type == sf::Event::Closed)
                    gameWindow.getWindow().close();

                if (event.key.code == sf::Keyboard::Enter)
                {
                    switch (selectedIndex)
                    {
                    case 0:
                        currentGameState = GameState::Playing;
                        break;
                    case 1:
                        currentGameState = GameState::Load_list;
                        break;
                    case 2:
                        currentGameState = GameState::Ranking_list;
                        break;
                    case 3:
                        currentGameState = GameState::Settings;
                        break;
                    case 4:
                        return 0;
                    }

                }
                else if (event.key.code == sf::Keyboard::W)
                {
                    selectedIndex--;
                    if (selectedIndex < 0)
                        selectedIndex = startingMenu.getMenuCount()-1;
                }
                else if (event.key.code == sf::Keyboard::S)
                {
                    selectedIndex++;
                    if (selectedIndex > startingMenu.getMenuCount()-1)
                        selectedIndex = 0;
                }

            }

        }
        gameWindow.getWindow().clear();

        startingMenu.getSelectorMod().setPosition(400.f, startingMenu.getSelectorPositions()[selectedIndex]);

        if (currentGameState == GameState::Starting_menu)
        {
            gameWindow.getWindow().draw(startingMenu.getSelector());
            gameWindow.getWindow().draw(startingMenu.getTitle());
            for (auto text : startingMenu.getOptions())
                gameWindow.getWindow().draw(text);
        }
        else if (currentGameState == GameState::Playing)
        {
            sf::Text playText("W trakcie gry", font1, 50);
            playText.setPosition({ 250.f, 150.f });
            gameWindow.getWindow().draw(playText);
        }
        gameWindow.getWindow().display();

    }
    return 0;
}
