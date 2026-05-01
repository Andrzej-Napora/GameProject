#include "header.h"
#include "Obstacles.h"

int main()
{

    //generator liczb losowych
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0,100);
    int randomValue = dist(gen);

    //zmienna przechowuajca czcionke
    sf::Font font1;
    font1.loadFromFile("Cabin-SemiBold.ttf");

    //tworzenie obiektow poszczegolnych klas
    GameWindow gameWindow;
    User user(font1);
    RankingList rankingList;
    Menu startingMenu("FLAPPY BIRDS", 4, {"Play","Ranking list","Settings","Exit"}, font1);
    Menu gameOver("Game Over",font1);
    Menu rankingMenu("Ranking Menu", rankingList.getList().size(), rankingList.getList(), font1);
    GameState currentGameState = GameState::Starting_menu;
    Bird bird;
    ObstacleQueue obstacleQueue;
    Score score;
    Vortex vortex(randomValue);


    rankingMenu.optionsUpdate(rankingList.getList());

    //selectedIndex wskazuje na opcje, na ktorej graficznie znajduej sie selector
    int selectedIndex = 0;

    // 60 aktualizacji na sekundę 
    sf::Clock clock;

    //glowna petla programu
    while (gameWindow.getWindow().isOpen())
    {
        //zmienna dt sluzy do uniezaleznienie ruchu obietkow od szybkosci dzialania komputera
        float dt = clock.restart().asSeconds();
        dt = std::min(dt, 0.05f);

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
                        currentGameState = GameState::Ranking_list;
                        break;
                    case 2:
                        currentGameState = GameState::Settings;
                        break;
                    case 3:
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
            //z ranking menu wychodzi sie naciskajac Escape
            if (currentGameState == GameState::Ranking_list && event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Escape)
                {
                    currentGameState = GameState::Starting_menu;
                }
            }
            //obluga klawaitury i myszy podczas grania
            if (currentGameState == GameState::Playing)
            {
                // 1. obługa klawiatury
                if (event.type == sf::Event::KeyPressed)
                {
                    if (event.key.code == sf::Keyboard::Space)
                        bird.jumpHandle();
                    else if (event.key.code == sf::Keyboard::A || event.key.code == sf::Keyboard::Left)
                        bird.leftMovementHandle();
                    else if (event.key.code == sf::Keyboard::D || event.key.code == sf::Keyboard::Right)
                        bird.rightMovementHandle();
                }
                // 2. obsługa myszy
                else if (event.type == sf::Event::MouseButtonPressed)
                {
                    if (event.mouseButton.button == sf::Mouse::Left)
                        bird.jumpHandle();
                }
            }
            //wyswietlenie ekranu Game Over
            if (currentGameState == GameState::Game_Over && (event.type == sf::Event::KeyPressed || event.type == sf::Event::MouseButtonPressed))
            {   
                if (event.key.code == sf::Keyboard::Space || event.key.code == sf::Mouse::Left)
                    currentGameState = GameState::Starting_menu;
            }
            //Obsluga ekranu wpisywania nazwy uzytkownika
            if (currentGameState == GameState::Username_Input)
            {
                if (event.type == sf::Event::TextEntered)
                {
                    if (event.text.unicode < 128)
                    {

                        if (event.text.unicode == 8)
                        {
                            if (!user.getName().empty())
                                user.getName().pop_back();
                        }
                        else if (event.text.unicode >= 32)
                        {
                            user.getName() += static_cast<char>(event.text.unicode);
                        }
                    }
                }
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter)
                {
                    //po wprowadzeniu nazwy uzytkownika aktualizujemy liste rankingowa, zapisujemy ja do pliku i aktualizujemy ranking menu
                    rankingList.rankingListUpdate(score, user);
                    rankingList.rankingListSave();
                    rankingMenu.optionsUpdate(rankingList.getList());
                    score.setScore(0.f);
                    user.nameReset();
                    currentGameState = GameState::Starting_menu;
                }
            }

        }
        //odswiezanie klatki
        gameWindow.getWindow().clear();


        //rysujemy starting_menu
        if (currentGameState == GameState::Starting_menu)
        {
            //ustawiamy selector na selectedIndex wektora selectorPositions
            startingMenu.getSelectorMod().setPosition(400.f, startingMenu.getSelectorPositions()[selectedIndex]);

            gameWindow.getWindow().draw(startingMenu.getSelector());
            gameWindow.getWindow().draw(startingMenu.getTitle());
            for (auto text : startingMenu.getOptions())
                gameWindow.getWindow().draw(text);
        }
        //rysujemy liste rankingowa
        if (currentGameState == GameState::Ranking_list)
        {
            gameWindow.getWindow().draw(rankingMenu.getTitle());
            for (auto text : rankingMenu.getOptions())
                gameWindow.getWindow().draw(text);
        }
        //rysujemy ekrna wprowadzania username do rankingu
        if (currentGameState == GameState::Username_Input)
        {
            user.updateNameText();
            sf::Text text("Prosze wprowadzic nazwe uzytkownika", font1, 40);
            text.setPosition({ 60,100 });
            gameWindow.getWindow().draw(text);
            gameWindow.getWindow().draw(user.getNameText());
        }
        //obsluga GameState Playing
        else if (currentGameState == GameState::Playing)
        {

            //aktualizujemy predkosc ptaka, w zaleznosci czy zostal wcisniety "jump"
            bird.birdUpdate(dt);

            //obsluga generowania i usuwania przeszkod i wirow
            randomValue = dist(gen);
            obstacleQueue.spawnObstacleCondition(dt, randomValue);
            obstacleQueue.removeObstacleCondition(dt);
            vortex.vortexUpdate(dt, randomValue);

            //petla aktualizujaca wszystkie przeszkody naraz
            for (auto& obstacle : obstacleQueue.getQueue())
            {
                obstacle.getdoubleObs().first.updateObstacle(dt);
                obstacle.getdoubleObs().second.updateObstacle(dt);
            }
            score.incrementScore(dt);
            //sprawdzamy kolizje
            sf::FloatRect ramkPtaka = bird.getBirdShape().getGlobalBounds();
            for (auto& obstacle : obstacleQueue.getQueue())
            {
                sf::FloatRect ramkaPrzeszkodyGornej = obstacle.getdoubleObs().first.getObstacle().getGlobalBounds();
                sf::FloatRect ramkaPrzeszkodyDolnej = obstacle.getdoubleObs().second.getObstacle().getGlobalBounds();
                if(ramkPtaka.intersects(ramkaPrzeszkodyGornej)|| ramkPtaka.intersects(ramkaPrzeszkodyDolnej))
                {

                    //po kolizji resetujemy stan gry, przed kolejnym Play
                    bird.resetBird();
                    obstacleQueue.resetObstacleQueue();

                    //jesli podczas kolizji wynik byl wystarczajaco wysoki, zeby dostac sie na liste rankingowa,
                    //przechodzimy do ekranu wpisywania nazwy uzytkownika; w przeciwnym razie, do ekranu GameOver
                    if (rankingList.getList().size() < 100 || score.getScore() > rankingList.getList().back().first)
                    {
                        currentGameState = GameState::Username_Input;
                        break;
                    }
                    else
                    {
                        currentGameState = GameState::Game_Over;
                        break;
                    }
                }
            }
            //rysujemy
            gameWindow.getWindow().draw(gameWindow.getSprite());
            for (auto& obstacle : obstacleQueue.getQueue())
            {
                gameWindow.getWindow().draw(obstacle.getdoubleObs().first.getObstacle());
                gameWindow.getWindow().draw(obstacle.getdoubleObs().second.getObstacle());
            }
            gameWindow.getWindow().draw(vortex.getVortex());
            gameWindow.getWindow().draw(bird.getBirdShape());
            gameWindow.getWindow().draw(score.getText());
        }

        //wyswietlamy to co narysowalismy
        gameWindow.getWindow().display();

    }
    return 0;
}
