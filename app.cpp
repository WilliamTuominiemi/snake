#include <iostream>
#include <vector>
#include <termios.h>
#include <unistd.h>

const int WIDTH = 20;
const int HEIGHT = 20;

class SnakeGame
{
public:
    std::vector<std::vector<char>> grid;
    char lastInput;
    int x, y;

    SnakeGame() : lastInput(' '), x(WIDTH / 2), y(HEIGHT / 2)
    {
        grid = std::vector<std::vector<char>>(HEIGHT, std::vector<char>(WIDTH, '.'));
        grid[y][x] = 'O';
    }

    void drawMap()
    {
        system("clear");
        for (const auto &row : grid)
        {
            for (const char cell : row)
            {
                std::cout << cell << " ";
            }
            std::cout << "\n";
        }
        std::cout << "WASD movement | 'q' quit.\n";
    }

    void run()
    {
        setRawMode(true);
        while (true)
        {
            if (readInput())
            {
                if (lastInput == 'q')
                    break;
                updatePlayerPosition();
                drawMap();
            }
        }
        setRawMode(false);
    }

private:
    void setRawMode(bool enable)
    {
        static struct termios oldt, newt;
        if (enable)
        {
            tcgetattr(STDIN_FILENO, &oldt);
            newt = oldt;
            newt.c_lflag &= ~(ICANON | ECHO);
            tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        }
        else
        {
            tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        }
    }

    bool readInput()
    {
        if (read(STDIN_FILENO, &lastInput, 1) == 1)
        {
            if (lastInput == 'w' || lastInput == 'a' || lastInput == 's' || lastInput == 'd' || lastInput == 'q')
            {
                return true;
            }
        }
        return false;
    }

    void updatePlayerPosition()
    {
        grid[y][x] = '.';

        switch (lastInput)
        {
        case 'w':
            y = std::max(0, y - 1);
            break;
        case 'a':
            x = std::max(0, x - 1);
            break;
        case 's':
            y = std::min(HEIGHT - 1, y + 1);
            break;
        case 'd':
            x = std::min(WIDTH - 1, x + 1);
            break;
        }

        grid[y][x] = 'O';
    }
};

int main()
{
    SnakeGame game;
    game.run();
    return 0;
}