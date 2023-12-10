#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "piece.h"

class Player {
    PlayerColour c;
    PlayerType t;
    float score;

public:
    Player(PlayerColour c, PlayerType t, float score);
    ~Player();

    void setPlayerColour(PlayerColour OtherCol);
    void setPlayerType(PlayerType OtherType);
    void setScore(float n);
    void addScore(float n);

    PlayerColour getPlayerColour();
    PlayerType getPlayerType();
    float getScore();
};

#endif
