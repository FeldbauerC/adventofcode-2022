#include <iostream>
#include <map>
#include <fstream>

constexpr int offsetLowercaseLetterItem = 'a' - 1;
constexpr int offsetUppercaseLetterItem = 'A' - 27;

/* split line */
std::pair<std::string, std::string> splitRucksackToCompartmentList(std::string rucksack)
{
    int compartmentVolume = 0;
    std::string compartment1;
    std::string compartment2;

    if (rucksack.length() % 2 == 0)
        compartmentVolume = rucksack.length() / 2;
    else
    {
        compartmentVolume = (unsigned)(rucksack.length() / 2) + 1;
        std::cerr << "not even" << std::endl;
    }

    try
    {
        compartment2 = rucksack.substr(compartmentVolume, compartmentVolume);
        compartment1 = rucksack.substr(0, compartmentVolume);
    }
    catch (const std::out_of_range &error)
    {
        std::cerr << "splitting string caused error: " << error.what() << '\n';
    }

    return std::make_pair(compartment1, compartment2);
}

/* get priority of item */
int getPriorityOfItem(char item)
{
    if (isupper(item))
        return (item - offsetUppercaseLetterItem);
    else
        return (item - offsetLowercaseLetterItem);
}

/* get character map of string with priority of item*/
std::map<char, int> getCharacterMapWithPriority(std::string compartment)
{
    std::map<char, int> characterMap;
    for (size_t i = 0; i < compartment.length(); i++)
        characterMap.insert(std::make_pair(compartment.at(i), getPriorityOfItem(compartment.at(i))));

    return characterMap;
}

int main(int argc, char **argv)
{
    std::cout << "called: " << argv[0] << " with " << (argc - 1) << " arguments" << std::endl;
    if (argc != 2)
        return -1;

    int itemPriorities = 0;
    std::pair<std::string, std::string> rucksackCompartment;
    std::map<char, int> compartmentMap;
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

        /* find duplicate item */
        rucksackCompartment = splitRucksackToCompartmentList(line);
        compartmentMap = getCharacterMapWithPriority(rucksackCompartment.second);

        for (size_t i = 0; i < rucksackCompartment.first.length(); i++)
        {
            auto item = compartmentMap.find(rucksackCompartment.first.at(i));
            if (item == compartmentMap.end())
                continue;
            else
            {
                itemPriorities += item->second;
                break;
            }
        }
    }
    input.close();

    std::cout << "the sum of all priorities is: " << itemPriorities << std::endl;

    return 0;
}