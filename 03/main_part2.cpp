#include <iostream>
#include <fstream>
#include <map>

constexpr int offsetLowercaseLetterItem = 'a' - 1;
constexpr int offsetUppercaseLetterItem = 'A' - 27;

/* get priority of item */
int getPriorityOfItem(char item)
{
    if (isupper(item))
        return (item - offsetUppercaseLetterItem);
    else
        return (item - offsetLowercaseLetterItem);
}

/* get character map of string */
std::map<char, int> getCharacterMap(std::string rucksack)
{
    std::map<char, int> characterMap;
    for (size_t i = 0; i < rucksack.length(); i++)
        characterMap.insert(std::make_pair(rucksack.at(i), NULL));

    return characterMap;
}

int main(int argc, char **argv)
{
    std::cout << "called: " << argv[0] << " with " << argc << " arguments" << std::endl;
    if (argc != 2)
        return -1;

    int badgePriorities = 0;
    std::string rucksackElf1;
    std::string rucksackElf2;
    std::string rucksackElf3;
    int groupMember = 1;

    std::ifstream input;
    input.open(argv[1]);

    while (!input.eof())
    {
        char line_buf[80];
        std::string line;

        input.getline(&line_buf[0], sizeof(line_buf));
        line = line_buf;
        if (line.length() == 0)
        {
            std::cerr << "parsed empty line" << std::endl; // ok if last line
            continue;
        }

        /* match rucksack to elf in group */
        switch (groupMember)
        {
        case 1:
            rucksackElf1 = line;
            break;
        case 2:
            rucksackElf2 = line;
            break;
        case 3:
            rucksackElf3 = line;
            groupMember = 0;
            break;

        default:
            break;
        }

        /* find common item per group and sum up priority of choosen badges */
        if (groupMember == 0)
        {
            std::map<char, int> rucksackMapElf2;
            std::map<char, int> rucksackMapElf3;

            /* find duplicate item */
            rucksackMapElf2 = getCharacterMap(rucksackElf2);
            rucksackMapElf3 = getCharacterMap(rucksackElf3);

            for (size_t i = 0; i < rucksackElf1.length(); i++)
            {
                auto itemElf2 = rucksackMapElf2.find(rucksackElf1.at(i));
                auto itemElf3 = rucksackMapElf3.find(rucksackElf1.at(i));
                if (itemElf2 == rucksackMapElf2.end() || itemElf3 == rucksackMapElf3.end())
                    continue;
                else
                {
                    badgePriorities += getPriorityOfItem(rucksackElf1.at(i));
                    break;
                }
            }
        }
        groupMember++;
    }
    input.close();

    std::cout << "the sum of priorities of all badges is: " << badgePriorities << std::endl;

    return 0;
}