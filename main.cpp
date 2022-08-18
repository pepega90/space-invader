#include <raylib.h>
#include <math.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include "Player.cpp"
#include "Bullet.cpp"
#include "Musuh.cpp"

enum Scene
{
    MENU = 0,
    PLAY,
    WIN,
    GAMEOVER,
    END,
};

const int WIDTH = 800;
const int HEIGHT = 600;

int main()
{
    InitWindow(WIDTH, HEIGHT, "Space Pepega Invader");
    InitAudioDevice();
    SetTargetFPS(60);
    Scene currentScene = MENU;

    // game variabel
    int score = 0;
    int round = 1;

    // load font
    Font f = LoadFont("assets/Facon.ttf");

    // load assets
    Texture2D player_img = LoadTexture("assets/player_ship.png");
    Texture2D musuh_img = LoadTexture("assets/alien.png");
    Texture2D bullet_img = LoadTexture("assets/green_laser.png");
    Texture2D bullet_musuh_img = LoadTexture("assets/red_laser.png");

    // load sfx
    Sound shoot_sfx = LoadSound("assets/player_fire.wav");
    Sound new_round_sfx = LoadSound("assets/new_round.wav");

    // init player
    Player player = Player(player_img, GetScreenWidth() / 2.0, GetScreenHeight() - 70., 3);

    // init player bullet
    std::vector<Bullet *> bullet_arr;
    float lastUpdateBullet = GetTime();

    // init musuh
    std::vector<Musuh *> list_musuh;
    std::vector<Bullet *> bullet_musuh;
    float lastMusuhNembak = GetTime();
    int col = 4;
    int row = 2;
    float speed = 2.0;
    bool fall = false;
    bool mShoot = false;
    for (int i = 0; i < col; i++)
    {
        for (int j = 0; j < row; j++)
        {
            Musuh *m = new Musuh(musuh_img, i * musuh_img.width + 30., j * musuh_img.height + 50.);
            list_musuh.push_back(m);
        }
    }

    while (!WindowShouldClose())
    {

        BeginDrawing();
        ClearBackground(BLACK);

        switch (currentScene)
        {
        case MENU:
        {
            DrawTextEx(f, "Space Pepega Invader", Vector2{GetScreenWidth() / 2.f - 180.f, GetScreenHeight() / 4.f}, 30.0, 0.0, YELLOW);
            DrawTextEx(f, "Tekan \"Space\" untuk play", Vector2{GetScreenWidth() / 2.f - 150.f, GetScreenHeight() / 2.f}, 20.0, 0.0, WHITE);
            DrawTextEx(f, "created by aji mustofa @pepega90", Vector2{GetScreenWidth() / 2.f - 150.f, GetScreenHeight() - 35.f}, 15.0, 0.0, WHITE);

            if (IsKeyPressed(KEY_SPACE) && currentScene == MENU)
            {
                currentScene = PLAY;
            }

            break;
        }
        case PLAY:
        {
            // check jika player tekan space berarti nembak
            if (IsKeyPressed(KEY_SPACE) && GetTime() - lastUpdateBullet > 0.5)
            {

                // PlaySound(shoot_sfx);
                lastUpdateBullet = GetTime();
                Bullet *bullet = new Bullet(bullet_img, player.pos.x + 26, player.pos.y);
                bullet->active = true;
                bullet_arr.push_back(bullet);
            }

            // tampilkan semua peluru
            for (auto &b : bullet_arr)
            {
                b->Draw();
            }

            // check jika posisi koordinat peluru y kurang dari 0 atau jika di collide
            auto removeBullet = std::remove_if(bullet_arr.begin(), bullet_arr.end(), [&](Bullet *b)
                                               { return b->pos.y < -30 || !b->active; });
            if (removeBullet != bullet_arr.end())
            {
                bullet_arr.erase(removeBullet);
            }

            // update dan draw player
            player.Update();
            player.Draw();

            // check jika darah musuh sudah habis
            if (player.live < 1)
            {
                currentScene = GAMEOVER;
            }

            // bikin musuh nembak
            if (GetTime() - lastMusuhNembak > 0.5 && !mShoot)
            {
                mShoot = true;
                Musuh *m = list_musuh[GetRandomValue(0, list_musuh.size() - 1)];
                Bullet *bm = new Bullet(bullet_musuh_img, m->pos.x + 27., m->pos.y + 35.);
                bm->active = true;
                bullet_musuh.push_back(bm);
            }

            // check jika posisi koordinat peluru musuh y lebih besar GetScreenHeight() atau jika di collide
            auto removedMusuhBullet = std::remove_if(bullet_musuh.begin(), bullet_musuh.end(), [&](Bullet *b)
                                                     { return b->pos.y > HEIGHT || !b->active; });
            if (removedMusuhBullet != bullet_musuh.end())
            {
                bullet_musuh.erase(removedMusuhBullet);
                mShoot = false;
            }

            // ganti direction musuh ketika sudah mentok layar
            for (auto &m : list_musuh)
            {
                if (m->pos.x > GetScreenWidth() - musuh_img.width)
                {
                    speed = -2.0;
                    fall = true;
                }
                else if (m->pos.x < 0.0)
                {
                    speed = 2.0;
                    fall = true;
                }
            }

            // update position y musuh, setiap musuh ganti direction
            if (fall)
            {
                for (auto &m : list_musuh)
                {
                    m->pos.y += musuh_img.height / 2.0;
                }
                fall = false;
            }

            // check collision antara peluru player dengan musuh
            for (auto &musuh : list_musuh)
            {
                for (auto &b : bullet_arr)
                {
                    if (CheckCollisionRecs(b->rect, musuh->rect))
                    {
                        musuh->collide = true;
                        b->active = false;
                    }
                }
            }

            // check collision antara peluru musuh dengan player
            for (auto &bm : bullet_musuh)
            {
                if (CheckCollisionRecs(bm->rect, player.rect))
                {
                    bm->active = false;
                    player.live -= 1;
                }
            }

            // check collision antara player dengan musuh
            for (auto &m : list_musuh)
            {
                if (CheckCollisionRecs(m->rect, player.rect))
                {
                    currentScene = GAMEOVER;
                }
            }

            // check jika musuh collide dengan peluru player, maka kita hapus
            auto removeMusuh = std::remove_if(list_musuh.begin(), list_musuh.end(), [&](Musuh *m)
                                              { return m->collide == true; });
            if (removeMusuh != list_musuh.end())
            {
                list_musuh.erase(removeMusuh);
                score += 10;
            }

            // draw musuh
            for (auto &m : list_musuh)
            {
                if (!m->collide)
                    m->Draw(speed);
            }

            // draw peluru musuh
            for (auto &b : bullet_musuh)
            {
                b->pos.y += 5.;
                b->rect.x = b->pos.x;
                b->rect.y = b->pos.y;
                b->rect.width = b->img.width;
                b->rect.height = b->img.height;
                DrawTextureV(b->img, b->pos, WHITE);
            }

            // check jika musuh sudah habis
            if (list_musuh.size() < 1)
            {
                PlaySound(new_round_sfx);
                currentScene = WIN;
            }

            // check jika sudah 3 round
            if (round == 3 && list_musuh.size() < 1)
            {
                currentScene = END;
            }

            // draw round
            std::string roundText = "Round: " + std::to_string(round);
            DrawTextEx(f, roundText.c_str(), Vector2{10.0, 20.}, 20., 0.0, WHITE);

            // draw nyawa
            std::string nyawaText = "Nyawa: " + std::to_string(player.live);
            DrawTextEx(f, nyawaText.c_str(), Vector2{GetScreenWidth() - 140.0f, 20.}, 20., 0.0, WHITE);

            // draw score
            std::string scoreText = "Score: " + std::to_string(score);
            DrawTextEx(f, scoreText.c_str(), Vector2{GetScreenWidth() / 2.0f - 40.0f, 20.}, 20., 0.0, WHITE);

            // gambar garis pembatas
            DrawLineEx(Vector2{0, player.pos.y - 5}, Vector2{WIDTH, player.pos.y - 5}, 2.0, WHITE);
            DrawLineEx(Vector2{0, 50}, Vector2{WIDTH, 50}, 2.0, WHITE);
        }
        break;
        case WIN:
        {
            DrawTextEx(f, "Hebat!, Kamu berhasil bertahan", Vector2{150.0, GetScreenHeight() / 4.0f}, 30.0, 0.0, WHITE);
            DrawTextEx(f, "Tekan \"Space\" untuk next round", Vector2{140.0, GetScreenHeight() / 2.0f}, 30.0, 0.0, WHITE);

            if (IsKeyPressed(KEY_SPACE) && currentScene == WIN)
            {
                player.pos = Vector2{GetScreenWidth() / 2.0f, GetScreenHeight() - 70.f};
                round += 1;
                col += 2;
                row += 1;
                list_musuh.clear();
                speed = 2.0;
                for (int i = 0; i < col; i++)
                {
                    for (int j = 0; j < row; j++)
                    {
                        Musuh *m = new Musuh(musuh_img, i * musuh_img.width + 30., j * musuh_img.height + 50.);
                        list_musuh.push_back(m);
                    }
                }
                currentScene = PLAY;
            }

            break;
        }
        case GAMEOVER:
        {
            // draw ui untuk game over
            DrawTextEx(f, "Permainan Berakhir", Vector2{GetScreenWidth() / 2.0f - 160.0f, GetScreenHeight() / 4.0f}, 30, 0.0, WHITE);
            std::string scoreOverText = "Score Kamu = " + std::to_string(score);
            DrawTextEx(f, scoreOverText.c_str(), Vector2{GetScreenWidth() / 2.0f - 65.0f, GetScreenHeight() / 2.0f - 30.0f}, 20.0, 0.0, WHITE);
            DrawTextEx(f, "Tekan \"R\" untuk restart!", Vector2{GetScreenWidth() / 2.0f - 130.0f, GetScreenHeight() / 2.0f + 100.0f}, 20.0, 0.0, WHITE);

            // jika sedang game over dan tekan "R" maka restart
            if (IsKeyPressed(KEY_R) && currentScene == GAMEOVER)
            {
                round = 1;
                col = 4;
                row = 2;
                score = 0;
                player.live = 3;
                speed = 2.0;

                list_musuh.clear();
                for (int i = 0; i < col; i++)
                {
                    for (int j = 0; j < row; j++)
                    {
                        Musuh *m = new Musuh(musuh_img, i * musuh_img.width + 30., j * musuh_img.height + 50.);
                        list_musuh.push_back(m);
                    }
                }

                currentScene = PLAY;
            }
        }
        break;
        case END:
        {
            DrawTextEx(f, "\t\t\t\t\t\t\t\t\t\tTerima Kasih Prajurit\nkamu berhasil menyelamatkan dunia! ", Vector2{70.0, GetScreenHeight() / 4.0f}, 30.0, 0.0, WHITE);
            DrawTextEx(f, "Tekan \"R\" untuk restart!", Vector2{GetScreenWidth() / 2.0f - 130.0f, GetScreenHeight() / 2.0f + 100.0f}, 20.0, 0.0, WHITE);
            if (IsKeyPressed(KEY_R) && currentScene == END)
            {
                round = 1;
                col = 4;
                row = 2;
                score = 0;
                player.live = 3;
                speed = 2.0;

                list_musuh.clear();
                for (int i = 0; i < col; i++)
                {
                    for (int j = 0; j < row; j++)
                    {
                        Musuh *m = new Musuh(musuh_img, i * musuh_img.width + 30., j * musuh_img.height + 50.);
                        list_musuh.push_back(m);
                    }
                }

                currentScene = PLAY;
            }
            break;
        }
        }

        EndDrawing();
    }
    UnloadTexture(player_img);
    UnloadTexture(bullet_img);
    UnloadTexture(musuh_img);
    UnloadTexture(bullet_musuh_img);
    UnloadSound(shoot_sfx);
    UnloadSound(new_round_sfx);
    UnloadFont(f);
    CloseAudioDevice();
    CloseWindow();

    return 0;
}