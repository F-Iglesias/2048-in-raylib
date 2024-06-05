#include <raylib.h>

class Game
{
public:
int Board[4][4];

    Game()
    {
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                Board[i][j] = 0;
            }
        }
    }

    void Draw()
    {
        
    }

};