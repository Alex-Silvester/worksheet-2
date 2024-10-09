
#include "Game.h"
#include <iostream>

Game::Game(sf::RenderWindow& game_window)
  : window(game_window)
{
  srand(time(NULL));
}

Game::~Game()
{
  delete[] animals;
  delete[] passports;
  delete character;
  delete passport;
  delete dragged;
}

bool Game::init()
{
  character = new sf::Sprite;
  passport = new sf::Sprite;

  lives = 3;
  game_state = PLAY;

  loadTexture(background_texture, "../Data/WhackaMole Worksheet/background.png", background);

  loadTexture(accept_texture, "../Data/Critter Crossing Customs/accept button.png", accept);
  accept.setPosition(
    window.getSize().x - accept.getGlobalBounds().getSize().x - buffer_size,
    window.getSize().y - 2*accept.getGlobalBounds().getSize().y - 2*buffer_size);

  loadTexture(reject_texture, "../Data/Critter Crossing Customs/reject button.png", reject);
  reject.setPosition(
    window.getSize().x - reject.getGlobalBounds().getSize().x - buffer_size,
    window.getSize().y - reject.getGlobalBounds().getSize().y - buffer_size);

  loadTexture(animals[0], "../Data/Critter Crossing Customs/buffalo.png");
  loadTexture(animals[1], "../Data/Critter Crossing Customs/moose.png");
  loadTexture(animals[2], "../Data/Critter Crossing Customs/narwhal.png");

  loadTexture(passports[0], "../Data/Critter Crossing Customs/elephant passport.png");
  loadTexture(passports[1], "../Data/Critter Crossing Customs/moose passport.png");
  loadTexture(passports[2], "../Data/Critter Crossing Customs/penguin passport.png");

  character_box.setFillColor(sf::Color(100, 240, 0, 100));

  if (!font.loadFromFile("../Data/Fonts/open-sans/OpenSans-Semibold.ttf"))
    printf("font failed to load");

  score_text.setFont(font);
  score_text.setCharacterSize(30);
  score_text.setString("Score: " + std::to_string(score));
  score_text.setPosition(
    window.getSize().x - score_text.getGlobalBounds().getSize().x - buffer_size,
    buffer_size/2);

  lives_text.setFont(font);
  lives_text.setCharacterSize(30);
  lives_text.setString("Lives: " + std::to_string(lives));
  lives_text.setPosition(
    window.getSize().x - lives_text.getGlobalBounds().getSize().x - buffer_size,
    buffer_size + score_text.getGlobalBounds().getSize().y);

  lose_text.setFont(font);
  lose_text.setCharacterSize(window.getSize().x/2);
  lose_text.setLetterSpacing(0);
  lose_text.setString("LOSE");
  lose_text.setPosition(window.getSize().x/2 - lose_text.getGlobalBounds().getSize().x/2,
                        window.getSize().y/2 - lose_text.getGlobalBounds().getSize().y);

  newAnimal();

  return true;
}

void Game::loadTexture(sf::Texture& texture, std::string location, sf::Sprite& sprite)
{
  if (!texture.loadFromFile(location))
    printf("failed to load texture");
  sprite.setTexture(texture);
}

void Game::loadTexture(sf::Texture& texture, std::string location)
{
  if (!texture.loadFromFile(location))
    printf("failed to load texture");
}

void Game::update(float dt)
{
  if(game_state == PLAY)
    dragSprite(dragged);
}

void Game::render()
{
  if (game_state == PLAY)
  {
    window.draw(background);
    window.draw(score_text);
    window.draw(lives_text);
    window.draw(*character);
    window.draw(character_box);
    window.draw(*passport);
    window.draw(accept);
    window.draw(reject);
  }
  if (game_state == LOSE)
  {
    window.draw(lose_text);
  }
}

void Game::mouseClicked(sf::Event event)
{
  if (game_state == PLAY)
  {
    sf::Vector2i click = sf::Mouse::getPosition(window);
    sf::Vector2f clickf = static_cast<sf::Vector2f>(click);

    if (event.mouseButton.button == sf::Mouse::Left)
    {
      if (passport->getGlobalBounds().contains(clickf) && 
          !accept.getGlobalBounds().contains(clickf) &&
          !reject.getGlobalBounds().contains(clickf))
        dragged = passport;

      if (accept.getGlobalBounds().contains(clickf))
        passport_accepted = true;
      if (reject.getGlobalBounds().contains(clickf))
        passport_rejected = true;
    }
  }
}

void Game::mouseReleased(sf::Event event)
{
  if (game_state == PLAY)
  {
    if (dragged != nullptr)
    {
      drag_offset = sf::Vector2f(-1, -1);
      dragged = nullptr;

      sf::Vector2i click = sf::Mouse::getPosition(window);
      sf::Vector2f clickf = static_cast<sf::Vector2f>(click);

      if (character->getGlobalBounds().contains(clickf) && answered)
      {
        newAnimal();
        answered = false;
      }
    }
    else if (answered == false)
    {
      answered = true;
      if (animal_type[animal_index] == passport_animal[passport_index])
      {
        if (passport_accepted)
          score++;
        if (passport_rejected)
          lives--;
      }
      if (animal_type[animal_index] != passport_animal[passport_index])
      {
        if (passport_accepted)
          lives--;
        if (passport_rejected)
          score++;
      }

      score_text.setString("Score: " + std::to_string(score));
      score_text.setPosition(
        window.getSize().x - score_text.getGlobalBounds().getSize().x - buffer_size,
        buffer_size / 2);

      lives_text.setString("Lives: " + std::to_string(lives));
      lives_text.setPosition(
        window.getSize().x - lives_text.getGlobalBounds().getSize().x - buffer_size,
        buffer_size + score_text.getGlobalBounds().getSize().y);

      lives == 0 ? game_state = LOSE : game_state = PLAY;
    }
  }
}

void Game::keyPressed(sf::Event event)
{

}

void Game::newAnimal()
{
  passport_accepted = false;
  passport_rejected = false;

  animal_index = rand() % 3;
  passport_index = rand () % 3;

  should_accept = animal_index == passport_index ? true : false;

  character->setTexture(animals[animal_index], true);
  character->setScale(1.8, 1.8);
  character->setPosition(window.getSize().x / 12, window.getSize().y / 12);

  passport->setTexture(passports[passport_index]);
  passport->setScale(0.6, 0.6);
  passport->setPosition(window.getSize().x / 2, window.getSize().y / 3);

  character_box.setSize(character->getGlobalBounds().getSize());
  character_box.setPosition(character->getPosition());
  
}

void Game::dragSprite(sf::Sprite* sprite)
{
  if (sprite != nullptr)
  {
    sf::Vector2i mouse_position = sf::Mouse::getPosition(window);
    sf::Vector2f mouse_positionf = static_cast<sf::Vector2f>(mouse_position);

    if(drag_offset.x == -1 || drag_offset.y == -1)
      drag_offset = mouse_positionf - sprite->getPosition();

    sf::Vector2f drag_position = mouse_positionf - drag_offset;
    sprite->setPosition(drag_position);
  }
}


