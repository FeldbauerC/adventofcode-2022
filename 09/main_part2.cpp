#include <iostream>
#include <fstream>
#include <math.h>
#include <set>

enum class direction
{
    UP,
    DOWN,
    LEFT,
    RIGHT,
    INVALID
};

struct position
{
    int x;
    int y;
};

direction getDirection(char input)
{
    direction dir;
    switch (std::toupper(input))
    {
    case 'U':
        dir = direction::UP;
        break;

    case 'D':
        dir = direction::DOWN;
        break;

    case 'L':
        dir = direction::LEFT;
        break;

    case 'R':
        dir = direction::RIGHT;
        break;

    default:
        dir = direction::INVALID;
        break;
    }
    return dir;
}

void move(direction movingDirection, position *currentPosition)
{
    switch (movingDirection)
    {
    case direction::UP:
        currentPosition->y++;
        break;

    case direction::DOWN:
        currentPosition->y--;
        break;

    case direction::LEFT:
        currentPosition->x--;
        break;

    case direction::RIGHT:
        currentPosition->x++;
        break;

    default:
        break;
    }
}

void moveTail(position *currentPositionTail, position *currentPositionHead)
{
    /* calculate delta */
    int deltaX = currentPositionHead->x - currentPositionTail->x;
    int deltaY = currentPositionHead->y - currentPositionTail->y;
    int deltaXabs = fabs(deltaX);
    int deltaYabs = fabs(deltaY);

    if (!((deltaXabs == 0 && deltaYabs == 0) ||
          (deltaXabs == 0 && deltaYabs == 1) ||
          (deltaXabs == 1 && deltaYabs == 0) ||
          (deltaXabs == 1 && deltaYabs == 1)))
    {
        if ((deltaXabs == 1 && deltaYabs == 2) ||
            (deltaXabs == 2 && deltaYabs == 1) ||
            (deltaXabs == 2 && deltaYabs == 2))
        {
            // move diagonal
            direction dir1 = direction::INVALID;
            direction dir2 = direction::INVALID;

            if (deltaX < 0)
                dir1 = direction::LEFT;
            else if (deltaX > 0)
                dir1 = direction::RIGHT;
            else
                dir1 = direction::INVALID;

            if (deltaY < 0)
                dir2 = direction::DOWN;
            else if (deltaY > 0)
                dir2 = direction::UP;
            else
                dir2 = direction::INVALID;

            move(dir1, currentPositionTail);
            move(dir2, currentPositionTail);
        }
        else
        {
            direction dir;
            /* get direction */
            if (deltaX < 0)
                dir = direction::LEFT;
            else if (deltaX > 0)
                dir = direction::RIGHT;
            else if (deltaY < 0)
                dir = direction::DOWN;
            else if (deltaY > 0)
                dir = direction::UP;
            else
                dir = direction::INVALID;
            move(dir, currentPositionTail);
        }
    }
}

int main(int argc, char **argv)
{
    std::cout << "called: " << argv[0] << " with " << (argc - 1) << " arguments" << std::endl;
    if (argc != 2)
        return -1;

    position currentPositionHead = {0, 0};
    position currentPositionTail1 = {0, 0};
    position currentPositionTail2 = {0, 0};
    position currentPositionTail3 = {0, 0};
    position currentPositionTail4 = {0, 0};
    position currentPositionTail5 = {0, 0};
    position currentPositionTail6 = {0, 0};
    position currentPositionTail7 = {0, 0};
    position currentPositionTail8 = {0, 0};
    position currentPositionTail9 = {0, 0};
    std::set<std::string> tailPositions;

    std::ifstream input;
    input.open(argv[1]);

    while (!input.eof())
    {
        char line_buf[5];
        input.getline(&line_buf[0], sizeof(line_buf));

        if (std::string(line_buf).length() == 0)
        {
            std::cerr << "parsed empty line" << std::endl; // ok if last line
            continue;
        }

        int steps;
        char direction;
        sscanf(line_buf, "%c %d", &direction, &steps);
        for (int i = 0; i < steps; i++)
        {
            /* move head */
            move(getDirection(direction), &currentPositionHead);

            /* move tail(s) */
            moveTail(&currentPositionTail1, &currentPositionHead);
            moveTail(&currentPositionTail2, &currentPositionTail1);
            moveTail(&currentPositionTail3, &currentPositionTail2);
            moveTail(&currentPositionTail4, &currentPositionTail3);
            moveTail(&currentPositionTail5, &currentPositionTail4);
            moveTail(&currentPositionTail6, &currentPositionTail5);
            moveTail(&currentPositionTail7, &currentPositionTail6);
            moveTail(&currentPositionTail8, &currentPositionTail7);
            moveTail(&currentPositionTail9, &currentPositionTail8);

            /* save current position */
            char posStr[20];
            std::string visitedPos;
            sprintf(posStr, "x%i:y%i", currentPositionTail9.x, currentPositionTail9.y);
            visitedPos = posStr;

            tailPositions.insert(visitedPos);
        }

        /* do something with line */
    }
    input.close();
    int size = tailPositions.size();

    std::cout << "the result is: " << size << std::endl;

    return 0;
}