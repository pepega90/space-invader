#include <raylib.h>
// #include "Bullet.h"

struct Bullet
{
    Texture2D img;
    Vector2 pos;
    Rectangle rect;
    bool active;

    Bullet() : pos(Vector2{0.0, 0.0})
    {
    }
    Bullet(Texture2D img, float x, float y)
    {
        this->img = img;
        this->pos = Vector2{x, y};
        this->rect = Rectangle{x, y, static_cast<float>(img.width), static_cast<float>(img.height)};
    }
    void Draw()
    {
        pos.y -= 5.;

        rect.x = pos.x;
        rect.y = pos.y;
        rect.width = img.width;
        rect.height = img.height;

        DrawTextureV(img, pos, WHITE);
    }
};

// Bullet::

// Bullet::

// void Bullet::