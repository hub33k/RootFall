#pragma once

// Components
struct Transform {
  float x;
  float y;
};

struct Velocity {
  float x;
  float y;
};

struct Size {
  float width;
  float height;
};

struct Color {
  float r;
  float g;
  float b;
};

struct Score {
  int playerOne;
  int playerTwo;
};

struct Health {
  int health;
};

// Tags
struct PlayerOne {};
struct PlayerTwo {};
