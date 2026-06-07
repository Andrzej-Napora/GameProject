#include "header.h"
#include "Obstacles.h"


//********************************************************************************************************************
//Klasa GameWindow
//********************************************************************************************************************

//konstruktor domyslny
GameWindow::GameWindow():width(window_size), height(window_size)
{
    tex.loadFromFile("resources/textures/background.png");
    sprite.setTexture(tex);
    sprite.setOrigin(0.f, 0.f);
	window.create(sf::VideoMode(window_size, window_size), "Flappy birds - Menu");
    window.setFramerateLimit(60);
}

//konstruktor parametryczny
void GameWindow::setWindow(unsigned int width_arg, unsigned int height_arg)
{
    width = width_arg;
    height = height_arg;

    window.create(sf::VideoMode( width,height ), "Flappy birds - Menu");
    window.setFramerateLimit(60);
}

//gettery settery
const unsigned int& GameWindow::getWidth() const { return width; }
const unsigned int& GameWindow::getHight() const { return height; }
void GameWindow::setWidth(unsigned int width) { this->width = width; }
void GameWindow::setHight(unsigned int height) { this->height = height; }
sf::RenderWindow& GameWindow::getWindow() { return window; }
sf::Sprite GameWindow::getSprite() { return sprite; }

//********************************************************************************************************************
//Klasa User
//********************************************************************************************************************

User::User(sf::Font& font):nickname(""),font1(font)
{
    nameText.setCharacterSize(50.f);
    nameText.setFillColor(sf::Color::Cyan);
    nameText.setFont(font);
    nameText.setPosition({});
    sf::FloatRect nameFrame = nameText.getLocalBounds();
    nameText.setOrigin(nameFrame.left + nameFrame.width / 2.f, nameFrame.top + nameFrame.height / 2.f);
    nameText.setPosition({ 400.f,200.f });
}
string& User::getName() { return nickname; }
sf::Text& User::getNameText() { return nameText;}
void User::setNickname(string& name)
{
    nickname = name;
    nameText.setString("name");
}

void User::updateNameText() 
{ 
    nameText.setString(nickname); 
    sf::FloatRect nameFrame = nameText.getLocalBounds();
    nameText.setOrigin(nameFrame.left + nameFrame.width / 2.f, nameFrame.top + nameFrame.height / 2.f);
    nameText.setPosition({ 400.f,200.f });
}
void User::nameReset(){ nickname = ""; }

// 1. Konstruktor dla starting menu (Menu Główne)
Menu::Menu(string menu_title, int optionsSize, vector<string> labels, sf::Font& font_arg)
{
    float const startingPositionMain = 250.f;

    font = font_arg;

    // Próba załadowania tła dla menu startowego
    if (backgroundTexture.loadFromFile("resources/textures/menu_bg.png"))
    {
        backgroundSprite.setTexture(backgroundTexture);
        hasBackground = true;

        // Pobieramy oryginalny rozmiar wczytanej tekstury
        sf::Vector2u textureSize = backgroundTexture.getSize();

        // Określamy docelowy rozmiar okna
        float targetWidth = 800.f;
        float targetHeight = 800.f;

        // Obliczamy współczynnik skali
        float scaleX = targetWidth / textureSize.x;
        float scaleY = targetHeight / textureSize.y;

        // Nakładamy skalę na sprite
        backgroundSprite.setScale(scaleX, scaleY);
    }

    // Ustawienia selectora
    selector.setSize({ 500.f, 60.f });
    selector.setFillColor(sf::Color(255, 255, 255, 100));
    selector.setOrigin(250.f, 30.f);

    // Ustawienia tytułu menu
    title.setFont(font);
    title.setString(menu_title);
    title.setCharacterSize(50);
    title.setFillColor(activeColor);
    title.setOutlineColor(sf::Color::Black);
    title.setOutlineThickness(3.f);

    sf::FloatRect titleFrame = title.getLocalBounds();
    title.setOrigin(titleFrame.left + titleFrame.width / 2.f, titleFrame.top + titleFrame.height / 2.f);
    title.setPosition({ 400.f, 50.f });

    for (int i = 0; i < optionsSize; i++)
    {
        sf::Text text(labels[i], font, 50);
        text.setFillColor(inactiveColor);
        text.setOutlineColor(outlineColor);
        text.setOutlineThickness(2.f);

        sf::FloatRect Frame = text.getLocalBounds();
        text.setOrigin(Frame.left + Frame.width / 2.f, Frame.top + Frame.height / 2.f);
        text.setPosition({ 400.f, (startingPositionMain + (100 * i)) });
        options.push_back(text);
        selectorPositions.push_back(startingPositionMain + (100 * i));
    }

    menuCount = options.size();
    maxScroll = 0.f; // Brak przewijania w menu startowym
}

// 2. Konstruktor dla ranking menu
Menu::Menu(string menu_title, int optionsSize, vector<pair<int, string>> labels, sf::Font& font_arg)
{
    font = font_arg;
    hasBackground = false; // Ranking nie potrzebuje tła obrazkowego

    // Ustawienia tytułu rankingu
    title.setFont(font);
    title.setString(menu_title);
    title.setCharacterSize(50);
    title.setFillColor(activeColor);
    title.setOutlineColor(sf::Color::Black);
    title.setOutlineThickness(2.f);

    sf::FloatRect titleFrame = title.getLocalBounds();
    title.setOrigin(titleFrame.left + titleFrame.width / 2.f, titleFrame.top + titleFrame.height / 2.f);
    title.setPosition({ 400.f, 50.f });

    int filledCount = 0;

    for (int i = 0; i < optionsSize; i++)
    {
        // Ignorujemy puste pozycje
        if (labels[i].second.empty())
        {
            continue;
        }

        string line = std::to_string(labels[i].first) + " " + labels[i].second;
        sf::Text text(line, font, 35);
        text.setFillColor(sf::Color::White);
        text.setOutlineColor(sf::Color::Black);
        text.setOutlineThickness(1.5f);

        sf::FloatRect Frame = text.getLocalBounds();
        text.setOrigin(Frame.left + Frame.width / 2.f, Frame.top + Frame.height / 2.f);
        text.setPosition({ 400.f, (startingPosition + (itemHeight * filledCount)) });

        options.push_back(text);
        selectorPositions.push_back(startingPosition + (itemHeight * filledCount));

        filledCount++;
    }

    menuCount = options.size();

    // Obliczenie maxScroll na podstawie faktycznie zapełnionych pozycji
    float totalHeight = menuCount * itemHeight;
    if (totalHeight > visibleHeight)
    {
        maxScroll = totalHeight - visibleHeight;
    }
    else
    {
        maxScroll = 0.f;
    }
}

// 3. Konstruktor uproszczony
Menu::Menu(string menu_title, sf::Font& font)
{
    title.setFont(font);
    title.setString(menu_title);
    title.setFillColor(sf::Color::Red);
    title.setCharacterSize(70);
    sf::FloatRect titleFrame = title.getLocalBounds();
    title.setOrigin(titleFrame.left + titleFrame.width / 2.f, titleFrame.top + titleFrame.height / 2.f);
    title.setPosition({ 400.f, 200.f });
}

// Aktualizacja rankingu
void Menu::optionsUpdate(vector<pair<int, string>> list)
{
    options.clear();
    selectorPositions.clear();
    scrollOffset = 0.f; // Resetujemy przewinięcie przy aktualizacji danych

    int filledCount = 0;
    for (int i = 0; i < list.size(); i++)
    {
        if (list[i].second.empty())
        {
            continue;
        }

        string line = std::to_string(list[i].first) + " " + list[i].second;
        sf::Text text(line, font, 35);
        text.setFillColor(sf::Color::White);
        text.setOutlineColor(sf::Color::Black);
        text.setOutlineThickness(1.5f);

        sf::FloatRect Frame = text.getLocalBounds();
        text.setOrigin(Frame.left + Frame.width / 2.f, Frame.top + Frame.height / 2.f);
        text.setPosition({ 400.f, (startingPosition + (itemHeight * filledCount)) });

        options.push_back(text);
        selectorPositions.push_back(startingPosition + (itemHeight * filledCount));

        filledCount++;
    }

    menuCount = options.size();

    // Ponowne obliczenie zakresu przewijania po aktualizacji
    float totalHeight = menuCount * itemHeight;
    if (totalHeight > visibleHeight)
    {
        maxScroll = totalHeight - visibleHeight;
    }
    else
    {
        maxScroll = 0.f;
    }
}

void Menu::updateSelection(int selectedIndex)
{
    for (int i = 0; i < options.size(); ++i)
    {
        if (i == selectedIndex)
        {
            options[i].setFillColor(activeColor);
            options[i].setCharacterSize(52); // Dopasowane powiększenie do rozmiaru 50 w menu startowym
        }
        else
        {
            options[i].setFillColor(inactiveColor);
            options[i].setCharacterSize(50);
        }

        sf::FloatRect frame = options[i].getLocalBounds();
        options[i].setOrigin(frame.left + frame.width / 2.f, frame.top + frame.height / 2.f);
    }
}

void Menu::drawBackground(sf::RenderWindow& window, int selectedIndex)
{
    if (hasBackground)
    {
        window.draw(backgroundSprite);
    }
    else
    {
        window.clear(sf::Color(25, 25, 35));
    }

    updateSelection(selectedIndex);

    window.draw(title);
    for (const auto& option : options)
    {
        window.draw(option);
    }
}

void Menu::scroll(float delta)
{
    float scrollSpeed = 20.f;
    float newOffset = scrollOffset - (delta * scrollSpeed);

    if (newOffset < 0.f) newOffset = 0.f;
    if (newOffset > maxScroll) newOffset = maxScroll;

    float difference = newOffset - scrollOffset;
    scrollOffset = newOffset;

    for (auto& text : options)
    {
        text.move({ 0.f, -difference });
    }
}

void Menu::draw(sf::RenderWindow& window)
{
    window.draw(title);

    for (const auto& text : options)
    {
        float y = text.getPosition().y;
        // Rysowanie z obcinaniem elementów poza widocznym zakresem rankingu
        if (y >= startingPosition - 10.f && y <= startingPosition + visibleHeight)
        {
            window.draw(text);
        }
    }
}

// Gettery
const char& Menu::getMenuCount() const { return menuCount; }
const sf::Font& Menu::getFont() const { return font; }
const sf::RectangleShape& Menu::getSelector() const { return selector; }
sf::RectangleShape& Menu::getSelectorMod() { return selector; }
const sf::Text& Menu::getTitle() const { return title; }
const vector<sf::Text>& Menu::getOptions() const { return options; }
const vector<float>& Menu::getSelectorPositions() const { return selectorPositions; }




//********************************************************************************************************************
//Klasa Bird
//********************************************************************************************************************

//konstruktor ptaka
Bird::Bird() : velocity(0.f,0.f), gravity(550.f),jump(-330.f),xPosition(200.f),yPosition(400.f)
{
    move = 100.f;
    tex.loadFromFile("resources/textures/bird.png");
    birdShape.setTexture(&tex);
    radius = static_cast<float>(tex.getSize().x) / 2.0f;
    birdShape.setRadius(45.f);
    birdShape.setFillColor(sf::Color::Yellow);
    birdShape.setOutlineColor(sf::Color::White);
    birdShape.setOrigin({20.f,20.f});
    birdShape.setPosition({ 200.f,400.f });
};
sf::CircleShape Bird::getBirdShape() { return birdShape; }
const float Bird::getRadius() const { return radius; }

float& Bird::getDistans() { return distans; }
const float Bird::getXPosition() const { return xPosition; }
const float Bird::getYPosition() const { return yPosition; }

void Bird::setVelocity(float x, float y)
{
    velocity.x = x;
    velocity.y = y;
}
void Bird::birdUpdate(float dt, sf::Vector2f vec)
{
    float breaks = 70.f;

    velocity.y += gravity * dt;
    birdShape.move((velocity.x + vec.x )* dt, (velocity.y+vec.y )* dt);
    xPosition += (velocity.x + vec.x) * dt;
    yPosition += (velocity.y + vec.y) * dt;
    if (velocity.x > 0)
    {
        velocity.x -= breaks * dt;
    }
    else if (velocity.x < 0)
    {
        velocity.x += breaks * dt;
    }
}

void Bird::jumpHandle(){ velocity.y = jump;}
void Bird::leftMovementHandle(){ velocity.x = -move;}
void Bird::rightMovementHandle(){ velocity.x = move;}
void Bird::resetBird()
{
    birdShape.setPosition({ 200.f,400.f });
    velocity = { 0.f,0.f };
}


//********************************************************************************************************************
//Klasa Score
//********************************************************************************************************************

Score::Score() :score(0)
{
    scoreText.setString(std::to_string(score));
    scoreText.setCharacterSize(30);
    font.loadFromFile("resources/textures/Cabin-SemiBold.ttf");
    scoreText.setFont(font);
    scoreText.setFillColor(sf::Color::Blue);
    scoreText.setPosition({ 700.f,50.f });
}

const int Score::getScore() const { return score; }
const sf::Text& Score::getText() const { return scoreText; }
void Score::setScore(int score) { this->score = score; }
void Score::incrementScore(float change) 
{ 
    score += 60 * change;
    scoreText.setString(std::to_string(score));
}

//********************************************************************************************************************
//Klasa RankingList
//********************************************************************************************************************

//konstruktor listy rankingowej wczytuje dane z pliku .txt do vectora list
RankingList::RankingList()
{
    fstream file{path, std::ios::in};
    if (file.good())
    {
        string line;
        pair<int, string> scoreUser;
        while (std::getline(file,line))
        {
            stringstream ss(line);
            ss >> scoreUser.first;
            ss >> scoreUser.second;
            list.push_back(scoreUser);
        }
    }
    else
        cerr << "Nie powiodło sie otwarcie RankingList.txt" << endl;
    file.close();
}

//funkcja aktualizujaca liste rankingowa po wprowadzeniu nowej pozycji do rankingu
vector<pair<int, string>>& RankingList::getList() { return list; }
void RankingList::rankingListUpdate(Score& score, User& user)
{
    if (list.size() > 98)
        list.pop_back();
    pair<int, string> temp;
    temp.first = score.getScore();
    temp.second = user.getName();
    list.push_back(temp);
    std::sort(list.begin(), list.end(), [](const auto& a, const auto& b) {
        return a.first > b.first;
        });
}

//funkcja zapisujaca aktualna liste rankingowa do pliku .txt
void RankingList::rankingListSave()
{
    fstream file(path, std::ios::out);
    if (file.good())
    {
        for (auto& pair : list)
        {
            file << pair.first << " " << pair.second << "\n";
        }
    }
    else
        cerr << "Nie powiodło sie otwarcie RankingList.txt" << endl;
    file.close();
}

//**************************************************************************************************************
//klasa Vortex
//*************************************************************************************************************

Vortex::Vortex(int randomValue):spawnTimer(0),vortex(90.f),xPosition(800.f + 50.f),yPosition(600 + (randomValue)),force(18000)
{
    tex.loadFromFile("resources/textures/vortex.png");
    vortex.setTexture(&tex);
    velocity = { -200.f,0.f };
    vortex.setPosition({ xPosition, yPosition });
}
sf::CircleShape Vortex::getVortex() { return vortex; }

void Vortex::vortexUpdate(float dt, int randomValue)
{
    spawnTimer += dt;
    vortex.move(velocity.x * dt, velocity.y*dt);
    xPosition += velocity.x * dt;
    if (spawnTimer > 4.f)
    {
        spawnTimer = 0;
        if (vortex.getPosition().x < -50.f)
        {
            if(randomValue>50)
                yPosition = 400 + 2 * randomValue;
            else
                yPosition = 100 + 3 * randomValue;
            xPosition = 1000.f;
            vortex.setPosition({ xPosition,yPosition });
        }
    }
}

sf::Vector2f Vortex::gravity(Bird bird)
{
    sf::Vector2f vec;
    float yDist = vortex.getPosition().y - bird.getYPosition();
    float xDist = vortex.getPosition().x - bird.getXPosition();
    float distans = sqrt(pow(xDist, 2) + pow(yDist, 2));
    if (distans < 80.f) distans = 80.f;
    float current = force / distans;
    vec.x = current * xDist / distans;
    vec.y = current * yDist / distans;
    return vec;
}
void Vortex::reset(int randomValue)
{
    vortex.setPosition({ 800.f + 50.f ,600 });
}