
#ifndef PLATFORMER_GAME_H
#define PLATFORMER_GAME_H

#include <SFML/Graphics.hpp>

class Game
{

  enum states
  {
    PLAY,
    LOSE
  };

 public:
  Game(sf::RenderWindow& window);
  ~Game();
  bool init();
  void update(float dt);
  void render();
  void mouseClicked(sf::Event event);
  void mouseReleased(sf::Event event);
  void keyPressed(sf::Event event);

 private:

   void newAnimal();
   void dragSprite(sf::Sprite* sprite);
   void loadTexture(sf::Texture& texture, std::string location, sf::Sprite& sprite);
   void loadTexture(sf::Texture& texture, std::string location);

  sf::RenderWindow& window;
  
  sf::Sprite background;
  sf::Texture background_texture;

  sf::Sprite* character;
  sf::Sprite* passport;
  
  sf::Texture* animals = new sf::Texture[3];
  sf::Texture* passports = new sf::Texture[3];

  sf::Sprite* dragged = nullptr;

  sf::Sprite accept;
  sf::Texture accept_texture;

  sf::Sprite reject;
  sf::Texture reject_texture;

  float buffer_size = 10;
  sf::Vector2f drag_offset = sf::Vector2f(-1, -1);

  bool passport_accepted, passport_rejected, should_accept;

  int animal_index, passport_index;

  int score = 0;
  int lives;

  sf::Text score_text;
  sf::Text lives_text;
  sf::Text lose_text;
  sf::Font font;

  std::string animal_type[3] = { "buffalo", "moose", "narwhal" };
  std::string passport_animal[3] = {"elephant", "moose", "penguin"};

  bool answered = false;

  int game_state;
};

#endif // PLATFORMER_GAME_H
