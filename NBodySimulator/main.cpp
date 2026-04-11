#include "header.h"

int main()
{
    //zmienna przechowuajca czcionke
    sf::Font font1;
    font1.loadFromFile("Cabin-SemiBold.ttf");


    GameWindow gameWindow;
    Menu startingMenu("FLOPPY BIRDS", 4, {"Play","Load game","Ranking list","Settings","Exit"}, font1);
    GameState currentGameState = GameState::Starting_menu;

    //selectedIndex wskazuje na opcje, na ktorej graficznie znajduej sie selector
    int selectedIndex = 0;

    //glowna petla programu
    while (gameWindow.getWindow().isOpen())
    {
        sf::Event event;
        //petla rejestrujaca eventy
        while (gameWindow.getWindow().pollEvent(event))
        {
            //warunek obslugujacy klikniecie myszka na "x" okna
            if (event.type == sf::Event::Closed)
                gameWindow.getWindow().close();

            //obsluga klawiatury w starting_menu
            if (currentGameState == GameState::Starting_menu && event.type == sf::Event::KeyPressed)
            {           
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

                //obsluga selectedIndex, wskazuje na opcje, na ktorej graficznie znajduej sie selector
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
        //odswiezanie klatki
        gameWindow.getWindow().clear();

        //ustawiamy selector na selectedIndex wektora selectorPositions
        startingMenu.getSelectorMod().setPosition(400.f, startingMenu.getSelectorPositions()[selectedIndex]);

        //rysujemy starting_menu
        if (currentGameState == GameState::Starting_menu)
        {
            gameWindow.getWindow().draw(startingMenu.getSelector());
            gameWindow.getWindow().draw(startingMenu.getTitle());
            for (auto text : startingMenu.getOptions())
                gameWindow.getWindow().draw(text);
        }
        //rysujemy GameState Playing
        else if (currentGameState == GameState::Playing)
        {
            sf::Text playText("W trakcie gry", font1, 50);
            playText.setPosition({ 250.f, 150.f });
            gameWindow.getWindow().draw(playText);
        }

        //wyswietlamy to co narysowalismy
        gameWindow.getWindow().display();

    }
    return 0;
}
