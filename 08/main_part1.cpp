#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

int main(int argc, char **argv)
{
    std::cout << "called: " << argv[0] << " with " << (argc - 1) << " arguments" << std::endl;
    if (argc != 2)
        return -1;

    std::ifstream input;
    input.open(argv[1]);

    std::vector<std::vector<std::pair<int, bool>>> treeMap;
    while (!input.eof())
    {
        char line_buf[100];
        std::string line;

        input.getline(&line_buf[0], sizeof(line_buf));
        line = line_buf;
        if (line.length() == 0)
        {
            std::cerr << "parsed empty line" << std::endl; // ok if last line
            continue;
        }
        std::vector<std::pair<int, bool>> currentTreeRow;
        for (size_t colum = 0; colum < line.length(); colum++)
        {
            currentTreeRow.push_back(std::make_pair((int)(line.at(colum) - '0'), false));
        }
        treeMap.push_back(currentTreeRow);
    }
    input.close();

    /* calculate tree map sizes (length & height) */
    size_t treesRow = treeMap.front().size();
    size_t treesColum = treeMap.size();

    /* set edge-rows as visible */
    for (size_t i = 0; i < treesRow; i++)
    {
        treeMap.front().at(i).second = true;
        treeMap.back().at(i).second = true;
    }
    for (size_t i = 0; i < treesColum; i++)
    {
        treeMap.at(i).front().second = true;
        treeMap.at(i).back().second = true;
    }

    /* left to right */
    for (auto rowIt = treeMap.begin(); rowIt != treeMap.end(); rowIt++)
    {
        for (auto colIt = rowIt->begin(); colIt != rowIt->end(); colIt++)
        {
            if (colIt->second)
                continue;

            if (colIt->first > std::max_element(rowIt->begin(), colIt)->first)
                colIt->second = true;
        }
    }

    /* right to left */
    for (auto rowIt = treeMap.begin(); rowIt != treeMap.end(); rowIt++)
    {
        for (auto colIt = rowIt->rbegin(); colIt != rowIt->rend(); colIt++)
        {
            if (colIt->second)
                continue;

            if (colIt->first > std::max_element(rowIt->rbegin(), colIt)->first)
                colIt->second = true;
        }
    }

    /* initialize flipped tree map */
    std::vector<std::vector<std::pair<int, bool>>> treeMapflipped;
    std::vector<std::pair<int, bool>> emptyFlippedColum; // template 
    for (size_t t = 0; t < treesRow; t++)
    {
        emptyFlippedColum.push_back(std::make_pair(0, false));
    }
    for (size_t i = 0; i < treesColum; i++)
    {
        treeMapflipped.push_back(emptyFlippedColum);
    }

    /* copy tree hights */
    for (size_t row = 0; row < treesRow; row++)
    {
        for (size_t colum = 0; colum < treesColum; colum++)
        {
            treeMapflipped.at(colum).at(row).first = treeMap.at(row).at(colum).first;
            treeMapflipped.at(colum).at(row).second = treeMap.at(row).at(colum).second;
        }
    }

    /* top to bottom */
    for (auto rowIt = treeMapflipped.begin(); rowIt != treeMapflipped.end(); rowIt++)
    {
        for (auto colIt = rowIt->begin(); colIt != rowIt->end(); colIt++)
        {
            if (colIt->second)
                continue;

            if (colIt->first > std::max_element(rowIt->begin(), colIt)->first)
                colIt->second = true;
        }
    }

    /* bottom to top */
    for (auto rowIt = treeMapflipped.begin(); rowIt != treeMapflipped.end(); rowIt++)
    {
        for (auto colIt = rowIt->rbegin(); colIt != rowIt->rend(); colIt++)
        {
            if (colIt->second)
                continue;

            if (colIt->first > std::max_element(rowIt->rbegin(), colIt)->first)
                colIt->second = true;
        }
    }

    /* sum up all visible trees */
    int cnt = 0;
    for (auto row : treeMapflipped)
    {
        for (auto colum : row)
        {
            if (colum.second)
                cnt++;
        }
    }

    std::cout << "the result is: " << cnt << std::endl;
    return 0;
}