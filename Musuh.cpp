#include <raylib.h>
// #include "Musuh.h"

struct Musuh
{
    Texture2D img;
    Vector2 pos;
    Rectangle rect;
    bool collide;

    Musuh(Texture2D img, float x, float y)
    {
        this->img = img;
        this->pos = Vector2{x, y};
        this->rect = Rectangle{x, y, static_cast<float>(img.width), static_cast<float>(img.height)};
    }
    void Draw(float &speed)
    {
        pos.x += speed;

        rect.x = pos.x;
        rect.y = pos.y;
        rect.width = img.width;
        rect.height = img.height;

        DrawTextureV(img, pos, WHITE);
    }
};

// Musuh::

// void Musuh::
