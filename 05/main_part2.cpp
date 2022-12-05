#include <iostream>
#include <fstream>
#include <vector>
#include <list>

#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"

enum class inputParsePhase
{
    crateStack,
    newLine,
    moveCommands,
    finished
};

int main(int argc, char **argv)
{
    std::cout << "called: " << argv[0] << " with " << (argc - 1) << " arguments" << std::endl;
    if (argc != 2)
        return -1;

    std::vector<std::list<char>> crateStacks;
    std::list<char> newEmptyStack;
    crateStacks.push_back(newEmptyStack);

    inputParsePhase phase = inputParsePhase::crateStack;
    std::ifstream input;
    input.open(argv[1]);

    while (!input.eof())
    {
        char line_buf[80];
        std::string line;

        input.getline(&line_buf[0], sizeof(line_buf));
        line = line_buf;

        switch (phase)
        {
        case inputParsePhase::crateStack:
            for (size_t i = 0; i < line.length();)
            {
                auto idx = line.find('[', i);
                if (idx != std::string::npos)
                {
                    idx++;

                    auto stackNr = (idx + 2) / 4 + 1;
                    while (crateStacks.size() < (stackNr + 1))
                        crateStacks.push_back(newEmptyStack);

                    crateStacks.at(stackNr).push_front(line.at(idx));
                    i = idx;
                }
                else
                {
                    if (i == 0)
                    {
                        phase = inputParsePhase::newLine;
                        std::cout << "phase changed to new line" << std::endl;
                    }
                    i = line.length();
                }
            }
            break;

        case inputParsePhase::newLine:
            if (line.length() == 0)
            {
                phase = inputParsePhase::moveCommands;
                std::cout << "phase changed to move commands" << std::endl;
            }
            break;

        case inputParsePhase::moveCommands:
            if (line.length() == 0)
            {
                phase = inputParsePhase::finished;
                std::cout << "phase changed to finished" << std::endl;
                continue;
            }
            int crateCnt;
            int crateOrigin;
            int crateDestination;
            sscanf(line.c_str(), "move %d from %d to %d", &crateCnt, &crateOrigin, &crateDestination);

            if (crateCnt == 1)
            {
                crateStacks.at(crateDestination).push_back(crateStacks.at(crateOrigin).back());
                crateStacks.at(crateOrigin).pop_back();
            }
            else
            {
                std::list<char> CrateMover9001;
                /* load CrateMover 9001 */
                for (int i = 0; i < crateCnt; i++)
                {
                    CrateMover9001.push_back(crateStacks.at(crateOrigin).back());
                    crateStacks.at(crateOrigin).pop_back();
                }

                /* unload CrateMover 9001 */
                for (int i = 0; i < crateCnt; i++)
                {
                    crateStacks.at(crateDestination).push_back(CrateMover9001.back());
                    CrateMover9001.pop_back();
                }
            }
            break;

        case inputParsePhase::finished:
            std::cout << "this shouldn't be printed" << std::endl;
            break;

        default:
            break;
        }
    }
    input.close();

    if (phase == inputParsePhase::finished)
    {
        std::cout << "the result is: " << std::endl;
        for (size_t i = 1; i < crateStacks.size(); i++)
            std::cout << crateStacks.at(i).back();
        std::cout << std::endl;
    }
    return 0;
}