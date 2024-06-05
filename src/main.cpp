#include <iostream>
#include <vector>
#include <math.h>
#include <raylib.h>

inline int mod(int a, int b)
{
    int k = a%6;
    if (k<0)
        k+=b;
    return k;
}
Color colorBG = {0xBB, 0xAD, 0xA0, 0xFF};

std::vector<Color> tileColors = {
    Color{0xCD, 0xC1, 0xB4, 0xFF},
    Color{0xEE, 0xE4, 0xDA, 0xFF},
    Color{0xEE, 0xE1, 0xC9, 0xFF},
    Color{0xF3, 0xB2, 0x7A, 0xFF},
    Color{0xF6, 0x96, 0x64, 0xFF},
    Color{0xF7, 0x7C, 0x5F, 0xFF},
    Color{0xF7, 0x5F, 0x3B, 0xFF},
    Color{0xED, 0xD0, 0x73, 0xFF},
    Color{0xED, 0xCC, 0x62, 0xFF},
    Color{0xED, 0xC9, 0x50, 0xFF},
    Color{0xED, 0xC9, 0x50, 0xFF},
    Color{0xED, 0xC9, 0x50, 0xFF}
    };


class Game
{
public: 
    int board[4][4];
    Vector2 direction = {0, 0};
    Game()
    {
        //Sets the board to 0
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
                board[i][j] = 0;
        }

    }

    void Update(int direction, bool isVertical)
    {
        if (isVertical && direction == 1)
        {
            collapse();
            for (int i = 0; i < 4; i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    if (board[i][j] != 0 && board[i][j] == board[i][j+1])
                    {
                        board[i][j] = board[i][j] + 1;
                        board[i][j+1] = 0;
                        j++;
                    }
                }
            }
            collapse();
        }
        else if (isVertical && direction == -1)
        {
            flipBoard();
            Update(1, true);
            flipBoard();
        }
        else if(!isVertical && direction == 1)
        {
            rotateBoardClockwise();
            Update(1, true);
            rotateBoardCounterClockwise();
        }
        else if (!isVertical && direction == -1)
        {
            rotateBoardCounterClockwise();
            Update(1, true);
            rotateBoardClockwise();
        }
    }

    void collapse()
    {
        for (int i = 0; i < 4; i++)
        {
            while (!isColumnOkay(i))
            {
                for (int j = 1; j < 4; j++)
                {
                    if (board[i][j] != 0 && board[i][j-1] == 0)
                    {
                        board[i][j-1] = board[i][j];
                        for (int k = j; k < 4; k++)
                            board[i][k] = board[i][k+1];
                        board[i][3] = 0;
                    }

                }
            }    
        }
    }
    bool isColumnOkay(int i)
    {
        for (int j = 1; j < 4; j++)
            if (board[i][j] != 0 && board[i][j-1] == 0)
                return false;
        return true;
    }
    
    void rotateBoardClockwise() //Rotates the board counterclockwise
    {
        int transpose[4][4];
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                transpose[i][j] = board[j][i];
        
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                board[i][j] = transpose[3-i][j];
    }

    void rotateBoardCounterClockwise()
    {
        int transpose[4][4];
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                transpose[i][j] = board[j][i];
        
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                board[i][j] = transpose[i][3-j];
    }
    void flipBoard()
    {
        int copy[4][4];
        for (int i = 0; i < 4; i++)
            for (int j = 0; j<4; j++)
                copy[i][j] = board[i][j];
        for (int i = 0; i < 4; i++)
            for (int j = 0; j<4; j++)
                board[i][j] = copy[i][3-j];
    }
    void GenerateNewTile()
    {
        //
        int i, j;
        do 
        {
            i = GetRandomValue(0, 3);
            j = GetRandomValue(0, 3);
        }
        while (board[i][j] != 0);
        board[i][j] = 1;
        
    }

    void Draw()
    {
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                DrawRectangleRounded(Rectangle{90 + 180*i - 160/2.f, 90 + 180*j - 160/2.f, 160, 160},
                    0.2, 1, tileColors[board[i][j]]);
                int power = pow(2, board[i][j]);
                if (power > 1)
                {
                    Vector2 v = MeasureTextEx(GetFontDefault(), TextFormat("%d", power), 100, 10);
                    DrawText(TextFormat("%d", power), 90 + 180* i - v.x/2, 90 + 180*j - v.y/2, 100, WHITE);
                }
            }
        }
    }
};


int main()
{

    const int screenWidth = 720;
    const int screenHeight = 720;

    InitWindow(screenWidth, screenHeight, "2048");
    SetTargetFPS(60);

    Game game;
    game.board[3][0] = 1;
    game.board[3][1] = 1;
    game.board[3][2] = 1;
    game.board[3][3] = 1;
    while (!WindowShouldClose())
    {

        //Logic
        //INPUT
        int direction = 1;
        bool isVertical = 1;
        bool movement = false;
        if (IsKeyPressed(KEY_UP))
        {
            isVertical = 1;
            direction = 1;
            movement = true;
        }
        else if (IsKeyPressed(KEY_DOWN))
        {
            isVertical = 1;
            direction = -1;
            movement = true;
        }
        else if (IsKeyPressed(KEY_LEFT))
        {
            isVertical = 0;
            direction = 1;
            movement = true;
        }
        else if (IsKeyPressed(KEY_RIGHT))
        {
            isVertical = 0;
            direction = -1;
            movement = true;
        }

        if (movement)
        {
            game.Update(direction, isVertical);
            game.GenerateNewTile();
        }

        //Drawing
        BeginDrawing();
        ClearBackground(colorBG);    
        game.Draw();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}