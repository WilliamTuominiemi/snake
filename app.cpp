#include <iostream>
#include <vector>

const int WIDTH = 20;
const int HEIGHT = 20;

class SnakeGame
{
public:
    std::vector<std::vector<char>> grid;
    SnakeGame()
    {
        grid = std::vector<std::vector<char>>(HEIGHT, std::vector<char>(WIDTH, '.'));
    }

    void drawMap()
    {
        for (const auto &row : grid)
        {
            for (const char cell : row)
            {
                std::cout << cell << " ";
            }
            std::cout << "\n";
        }
    }

    void run()
    {
        drawMap();
    }
};

int main()
{
    SnakeGame game;
    game.run();
    return 0;
}