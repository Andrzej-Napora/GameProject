#include "header.h"

int main()
{

    //generator liczb losowych
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0,100);
    int randomValue = dist(gen);

    sf::Clock clock;

    //zmienna przechowuajca czcionke
    sf::Font font1;
    font1.loadFromFile("Cabin-SemiBold.ttf");

    //tworzenie obiektow poszczegolnych klas
    GameWindow gameWindow;
    Menu startingMenu("FLAPPY BIRDS", 4, {"Play","Load game","Ranking list","Settings","Exit"}, font1);
    GameState currentGameState = GameState::Starting_menu;
    Bird bird;
    ObstacleQueue obstacleQueue;

    //selectedIndex wskazuje na opcje, na ktorej graficznie znajduej sie selector
    int selectedIndex = 0;

    //glowna petla programu
    while (gameWindow.getWindow().isOpen())
    {
        //zmienna dt sluzy do uniezaleznienie ruchu obietkow od szybkosci dzialania komputera
        sf::Time time = clock.restart();
        float dt = time.asSeconds();

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
                else if (event.key.code == sf::Keyboard::W || event.key.code == sf::Keyboard::Up)
                {
                    selectedIndex--;
                    if (selectedIndex < 0)
                        selectedIndex = startingMenu.getMenuCount()-1;
                }
                else if (event.key.code == sf::Keyboard::S || event.key.code == sf::Keyboard::Down)
                {
                    selectedIndex++;
                    if (selectedIndex > startingMenu.getMenuCount()-1)
                        selectedIndex = 0;
                }

            }
            //obluga klawaitury i myszy podczas grania
            if (currentGameState == GameState::Playing && event.type == sf::Event::KeyPressed)
            {
                if(event.key.code == sf::Keyboard::Space || event.key.code == sf::Mouse::Left)
                    bird.inputHandle();
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
            //aktualizujemy predkosc ptaka, w zaleznosci czy zostal wcisniety "jump"
            bird.birdUpdate(dt);

            //sprawdzamy spawnTimer, po przekroczeniu 2sec pojawia sie nowa przeszkoda(jeszcze nie ma obslugi usuwania starych przeszkod)
            obstacleQueue.getSpawnTimer() += dt;
            if (obstacleQueue.getSpawnTimer() > 2)
            {
                randomValue = dist(gen);
                obstacleQueue.addRandomObstacle(randomValue);
                obstacleQueue.getSpawnTimer() = 0;
            }

            //petla obslugujaca wszystkie przeszkody naraz, przesuwamy je i rysujemy
            for (auto& obstacle : obstacleQueue.getQueue())
            {
                obstacle.updateObstacle(dt);
                gameWindow.getWindow().draw(obstacle.getObs());
            }
            gameWindow.getWindow().draw(bird.getBirdShape());
        }

        //wyswietlamy to co narysowalismy
        gameWindow.getWindow().display();

    }
    return 0;
}
