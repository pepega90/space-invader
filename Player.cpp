#include <raylib.h>
// #include "Player.h"

struct Player
{
    Texture2D img;
    Vector2 pos;
    Vector2 vel;
    Vector2 acc;
    Rectangle rect;
    int live;

    Player(Texture2D img, float x, float y, int live)
    {
        this->img = img;
        this->pos = Vector2{x, y};
        this->live = live;
        this->rect = Rectangle{x, y, static_cast<float>(img.width), static_cast<float>(img.height)};
    }
    void Update()
    {
        if (IsKeyDown(KEY_D) && pos.x < GetScreenWidth() - img.width - 5)
            vel.x += 5;
        if (IsKeyDown(KEY_A) && pos.x > 10)
            vel.x -= 5;

        acc.x = -vel.x * 0.4;
        acc.y = -vel.y * 0.4;

        vel.x += acc.x;
        vel.y += acc.y;

        pos.x += vel.x;
        pos.y += vel.y;

        rect.x = pos.x;
        rect.y = pos.y;
    }
    void Draw()
    {
        DrawTextureV(img, pos, WHITE);
    }
};

// Player::

// void Player::

// void Player::