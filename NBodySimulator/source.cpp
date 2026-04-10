#include "header.h"

GameWindow::GameWindow():width(800), hight(800)
{
	window.create(sf::VideoMode({ 800,800 }), "Flappy birds - Menu");
}

const unsigned int& GameWindow::getWidth() const{return width;}
const unsigned int& GameWindow::getHight() const{return hight;}
void GameWindow::setWidth(unsigned int width){this->width = width;}
void GameWindow::setHight(unsigned int hight){this->hight = hight;}
sf::RenderWindow& GameWindow::getWindow() { return window;}
void GameWindow::setWindow(unsigned int width, unsigned int hight)
{
    window.create(sf::VideoMode({ width,hight }), "Flappy birds - Menu");
}

Menu::Menu(string menu_title, int optionsSize, vector<string> labels, sf::Font& font_arg)
{
    float const startingPosition = 250.f;

    font = font_arg;

    selector.setSize({ 500.f,50.f });
    selector.setFillColor(sf::Color(255, 255, 255, 100));
    selector.setOrigin(250.f, 25.f);

    title.setFont(font);
    title.setString(menu_title);
    title.setCharacterSize(50);
    sf::FloatRect titleFrame = title.getLocalBounds();
    title.setOrigin(titleFrame.left + titleFrame.width / 2.f, titleFrame.top + titleFrame.height / 2.f);
    title.setPosition({ 400.f,50.f });

    for (int i = 0; i <= optionsSize;i++)
    {
        sf::Text text(labels[i], font, 50);
        sf::FloatRect Frame = text.getLocalBounds();
        text.setOrigin(Frame.left + Frame.width / 2.f, Frame.top + Frame.height / 2.f);
        text.setPosition({ 400.f,(startingPosition+(100*i))});
        options.push_back(text);
        selectorPositions.push_back(startingPosition + (100 * i));
    }

    menuCount = options.size();
}

const char& Menu::getMenuCount() const { return menuCount; }
const sf::Font& Menu::getFont() const { return font; }
const sf::RectangleShape& Menu::getSelector() const { return selector; }
const sf::Text& Menu::getTitle() const { return title; }
sf::RectangleShape& Menu::getSelectorMod() { return selector; }
const vector<sf::Text>& Menu::getOptions() const { return options; }
const vector<float>& Menu::getSelectorPositions() const { return selectorPositions; }