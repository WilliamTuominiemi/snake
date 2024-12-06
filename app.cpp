#include <iostream>
#include <vector>
#include <termios.h>
#include <unistd.h>
#include <thread>
#include <chrono>
#include <stdlib.h>
#include <time.h>

const int WIDTH = 15;
const int HEIGHT = 15;
const int GAME_SPEED_MS = 200;

class SnakeGame
{
public:
    std::vector<std::vector<char>> grid;
    char lastInput;
    int x, y;
    char direction;

    int foodX, foodY;

    int score = 0;

    std::vector<std::pair<int, int>> snakeBody;

    SnakeGame() : lastInput(' '), x(WIDTH / 2), y(HEIGHT / 2), direction('d'), foodX(rand() % WIDTH), foodY(rand() % HEIGHT)
    {
        grid = std::vector<std::vector<char>>(HEIGHT, std::vector<char>(WIDTH, '.'));
        snakeBody.push_back(std::make_pair(x, y));
        grid[snakeBody[0].second][snakeBody[0].first] = 'O';
        placeFood();
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
        std::cout << "Score: " << score << "\n";
    }

    void run()
    {
        setRawMode(true);
        std::thread inputThread(&SnakeGame::processInput, this);

        while (true)
        {
            if (lastInput == 'q')
                break;
            updatePlayerPosition();
            drawMap();
            std::this_thread::sleep_for(std::chrono::milliseconds(GAME_SPEED_MS));
        }
        setRawMode(false);
        inputThread.join();
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

    void processInput()
    {
        while (true)
        {
            char input;
            if (read(STDIN_FILENO, &input, 1) == 1)
            {
                if (input == 'q')
                {
                    lastInput = 'q';
                    break;
                }

                if (input == 'w' || input == 'a' || input == 's' || input == 'd')
                {
                    if (!((lastInput == 'w' && input == 's') ||
                          (lastInput == 's' && input == 'w') ||
                          (lastInput == 'a' && input == 'd') ||
                          (lastInput == 'd' && input == 'a')))
                    {
                        lastInput = input;
                        direction = input;
                    }
                }
            }
        }
    }

    void placeFood()
    {
        do
        {
            foodX = rand() % WIDTH;
            foodY = rand() % HEIGHT;
        } while (grid[foodY][foodX] != '.');

        grid[foodY][foodX] = '@';
    }

    void checkIfOnFood()
    {
        if (x == foodX && y == foodY)
        {
            grid[foodY][foodX] = '.';
            foodX = rand() % WIDTH;
            foodY = rand() % HEIGHT;
            score++;
        }
    }

    void updatePlayerPosition()
    {
        int newX = x;
        int newY = y;

        switch (direction)
        {
        case 'w':
            newY = std::max(0, y - 1);
            break;
        case 'a':
            newX = std::max(0, x - 1);
            break;
        case 's':
            newY = std::min(HEIGHT - 1, y + 1);
            break;
        case 'd':
            newX = std::min(WIDTH - 1, x + 1);
            break;
        }

        snakeBody.push_back(std::make_pair(x, y));

        if (newX == foodX && newY == foodY)
        {
            score++;
            placeFood();
        }
        else
        {
            grid[snakeBody.front().second][snakeBody.front().first] = '.';
            snakeBody.erase(snakeBody.begin());
        }

        for (const auto &bodyPart : snakeBody)
        {
            grid[bodyPart.second][bodyPart.first] = 'O';
        }

        x = newX;
        y = newY;
    }
};

int main()
{
    srand(time(NULL));
    SnakeGame game;
    game.run();
    return 0;
}