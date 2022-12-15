#include <iostream>
#include <fstream>
#include <vector>
#include <limits>

struct position
{
    int x;
    int y;
};

void drawRockFromTo(std::vector<std::vector<char>> *caveMap, position from, position to)
{
    if (from.x == to.x)
    {
        if (from.y < to.y)
        {
            for (int y = from.y; y <= to.y; y++)
                caveMap->at(y).at(from.x) = '#';
        }
        else
        {
            for (int y = from.y; y >= to.y; y--)
                caveMap->at(y).at(from.x) = '#';
        }
    }
    else
    {
        if (from.x < to.x)
        {
            for (int x = from.x; x <= to.x; x++)
                caveMap->at(from.y).at(x) = '#';
        }
        else
        {
            for (int x = from.x; x >= to.x; x--)
                caveMap->at(from.y).at(x) = '#';
        }
    }
}

bool dropSand(std::vector<std::vector<char>> *caveMap, position sandstart, position topleft, position bottomright)
{
    bool blocked = false;
    bool ret = false;
    position currentPos = sandstart;
    while (!blocked)
    {
        if (currentPos.y > (bottomright.y - 1))
        {
            blocked = true;
            continue;
        }
        if (currentPos.x <= (topleft.x) || currentPos.x >= bottomright.x)
        {
            ret = true;
            blocked = true;
            continue;
        }
        if (caveMap->at(currentPos.y + 1).at(currentPos.x) == '.')
        {
            currentPos.y++;
            continue;
        }
        else if (caveMap->at(currentPos.y + 1).at(currentPos.x - 1) == '.')
        {
            currentPos.y++;
            currentPos.x--;
            continue;
        }
        else if (caveMap->at(currentPos.y + 1).at(currentPos.x + 1) == '.')
        {
            currentPos.y++;
            currentPos.x++;
            continue;
        }
        else
        {
            blocked = true;
        }
    }
    if (currentPos.x == sandstart.x && currentPos.y == sandstart.y)
        ret = true;
    caveMap->at(currentPos.y).at(currentPos.x) = 'o';
    return ret;
}

int main(int argc, char **argv)
{
    std::cout << "called: " << argv[0] << " with " << (argc - 1) << " arguments" << std::endl;
    if (argc != 2)
        return -1;
    std::vector<std::vector<position>> caveMapElements;
    std::vector<std::vector<char>> caveMap;
    std::vector<position> caveElement;
    int smallestX = INT32_MAX;
    int biggestX = 0;
    int biggestY = 0;
    position sand = {500, 0};
    std::ifstream input;
    input.open(argv[1]);

    while (!input.eof())
    {
        char line_buf[400];
        std::string line;

        input.getline(&line_buf[0], sizeof(line_buf));
        line = line_buf;
        if (line.length() == 0)
        {
            std::cerr << "parsed empty line" << std::endl; // ok if last line
            continue;
        }

        std::vector<position> caveElement;
        for (size_t i = 0; i < line.length();)
        {
            int coordinateX, coordinateY;
            size_t idx = line.find_first_of(" -> ", i);
            if (idx != line.npos)
            {
                sscanf(line.substr(i, (idx - i)).c_str(), "%d,%d", &coordinateX, &coordinateY);
                i = (idx + 4);
            }
            else if (idx == line.npos && i < line.length())
            {
                sscanf(line.substr(i).c_str(), "%d,%d", &coordinateX, &coordinateY);
                i = line.length();
            }
            if (coordinateX < smallestX)
                smallestX = coordinateX;
            if (coordinateX > biggestX)
                biggestX = coordinateX;
            if (coordinateY > biggestY)
                biggestY = coordinateY;
            caveElement.push_back({coordinateX, coordinateY});
        }
        caveMapElements.push_back(caveElement);
    }
    input.close();

    /* calculate new outer borders */
    position caveTopLeft;
    position caveBottomRight;
    bool recalculationNeeded;
    caveBottomRight.x = sand.x + biggestY + 1; // +1 for extra space and to avoid std::out_of_range
    caveBottomRight.y = biggestY + 2;
    caveTopLeft.x = sand.x - caveBottomRight.y - 1; // -1 for extra space and to avoid std::out_of_range
    caveTopLeft.y = 0;

    /* fill map with air */
    for (int y = 0; y < caveBottomRight.y; y++)
    {
        std::vector<char> current;
        for (int x = 0; x < (caveBottomRight.x + 10); x++)
            current.push_back('.');
        caveMap.push_back(current);
    }

    /* add solid bottom */
    std::vector<char> current;
    for (int x = 0; x < (caveBottomRight.x + 10); x++)
        current.push_back('#');
    caveMap.push_back(current);

    /* insert sand start */
    caveMap.at(sand.y).at(sand.x) = '+';

    /* draw rocks in cave map */
    for (auto elem : caveMapElements)
    {
        for (size_t i = 0; i < (elem.size() - 1); i++)
            drawRockFromTo(&caveMap, elem.at(i), elem.at(i + 1));
    }

    while (!dropSand(&caveMap, sand, {0, 0}, {caveBottomRight.x + 10, caveBottomRight.y}))
        continue;

    int cnt = 0;

    /* print cave map */
    for (int y = 0; y <= caveBottomRight.y; y++)
    {
        for (int x = caveTopLeft.x; x <= caveBottomRight.x; x++)
        {
            std::cout << caveMap.at(y).at(x);
            if (caveMap.at(y).at(x) == 'o')
                cnt++;
        }
        std::cout << std::endl;
    }

    std::cout << "the result is: " << cnt << std::endl;

    return 0;
}