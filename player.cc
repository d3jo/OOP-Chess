#include "player.h"

Player::Player(PlayerColour c, PlayerType t, float score): c{c}, t{t}, score{score}{}

Player::~Player() {}

void Player::setPlayerColour(PlayerColour otherCol) {
    c = otherCol;
}

void Player::setPlayerType(PlayerType otherType){
    t = otherType;
}

void Player::setScore(float n) {
    score = n;
}

void Player::addScore(float n) {
    score += n;
}

PlayerColour Player::getPlayerColour() {
    return c;
}
PlayerType Player::getPlayerType() {
    return t;
}

float Player::getScore() {
    return score;
}
