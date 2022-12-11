#include <iostream>
#include <fstream>
#include <list>
#include <vector>
#include <map>
#include <functional>
#include <math.h>

class monkey;

class monkey
{
public:
    /* CTOR & DTOR */
    monkey(int name);
    ~monkey() = default;

    /* methods */
    void getItemPassed(int item);
    void playRound();

    /* members */
    int monkeyNumber;
    std::list<int> items;
    std::function<int(int, int)> worryLevel;
    int worryFactor;
    int testDivisibility;
    int inspectedItems;
    monkey *monkeyReferenceTrue;
    monkey *monkeyReferenceFalse;
};

monkey::monkey(int name)
    : monkeyNumber(name)
{
    inspectedItems = 0;
}

void monkey::getItemPassed(int item)
{
    this->items.emplace_back(item);
}

void monkey::playRound()
{
    /* check if monkey has any items */
    if (items.empty())
        return;

    for (auto item : items)
    {
        /* get item */
        int currentItemOfInterest = item;

        /* calculate new worry level after started playing */
        currentItemOfInterest = worryLevel(currentItemOfInterest, this->worryFactor);

        /* playing with it ... loosing interest */

        /* recalculate worry level after you chillin' */
        currentItemOfInterest /= 3;

        /* thinks about which monkey to pass item next */
        if (currentItemOfInterest % testDivisibility == 0)
            monkeyReferenceTrue->getItemPassed(currentItemOfInterest);
        else
            monkeyReferenceFalse->getItemPassed(currentItemOfInterest);

        /* update number of inspected items */
        inspectedItems++;
    }
    items.clear();
}

enum class inputParsePhase
{
    Name,
    ItemList,
    worryLevelOperation,
    testDivisibility,
    monkeyRefTrue,
    monkeyRefFalse,
    finished
};

int main(int argc, char **argv)
{
    std::cout << "called: " << argv[0] << " with " << (argc - 1) << " arguments" << std::endl;
    if (argc != 2)
        return -1;

    std::vector<monkey *> monkeys;
    std::map<int, std::pair<int, int>> monkeyReferences;

    inputParsePhase phase = inputParsePhase::Name;
    monkey *currentMonkey;
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
        case inputParsePhase::Name:
        {
            int monkeyName;
            sscanf(line_buf, "Monkey %d:", &monkeyName);
            currentMonkey = new monkey(monkeyName);
            phase = inputParsePhase::ItemList;
            break;
        }
        case inputParsePhase::ItemList:
        {
            std::string itemlist = line.substr(17);

            for (size_t i = 0; i < itemlist.length();)
            {
                auto idx = itemlist.find(',', i);
                if (idx != std::string::npos)
                {
                    int item = std::stoi(itemlist.substr(i, (idx - i)));
                    currentMonkey->getItemPassed(item);
                }
                else if (idx == std::string::npos && i != (itemlist.length() - 1)) // for last or single element in list
                {
                    int item = std::stoi(itemlist.substr(i));
                    currentMonkey->getItemPassed(item);
                    break;
                }

                idx += 2; // skip ',' and ' '
                i = idx;
            }
            phase = inputParsePhase::worryLevelOperation;
            break;
        }
        case inputParsePhase::worryLevelOperation:
        {
            char operation;
            char buf[10];
            std::string arg;
            int argVal = 0;
            sscanf(line_buf, "  Operation: new = old %c %s", &operation, &buf[0]);
            arg = buf;

            if (arg != "old")
            {
                argVal = std::stoi(arg);
            }
            currentMonkey->worryFactor = argVal;

            switch (operation)
            {
            case '+':
                if (argVal)
                    currentMonkey->worryLevel = [](int old, int arg2)
                    { return old + arg2; };
                else
                    currentMonkey->worryLevel = [](int old, int arg2)
                    { return old + old; };
                break;

            case '*':
                if (argVal)
                    currentMonkey->worryLevel = [](int old, int arg2)
                    { return old * arg2; };
                else
                    currentMonkey->worryLevel = [](int old, int arg2)
                    { return old * old; };
                break;

            default:
                break;
            }

            phase = inputParsePhase::testDivisibility;
            break;
        }
        case inputParsePhase::testDivisibility:
        {
            int testD;
            sscanf(line_buf, "  Test: divisible by %d", &testD);
            currentMonkey->testDivisibility = testD;
            phase = inputParsePhase::monkeyRefTrue;
            break;
        }
        case inputParsePhase::monkeyRefTrue:
        {
            int monkeyRef;
            sscanf(line_buf, "    If true: throw to monkey %d", &monkeyRef);
            monkeyReferences.emplace(currentMonkey->monkeyNumber, std::make_pair(monkeyRef, -1));
            phase = inputParsePhase::monkeyRefFalse;
            break;
        }
        case inputParsePhase::monkeyRefFalse:
        {
            int monkeyRef;
            sscanf(line_buf, "    If false: throw to monkey %d", &monkeyRef);
            monkeyReferences.at(currentMonkey->monkeyNumber).second = monkeyRef;
            phase = inputParsePhase::finished;
            break;
        }
        case inputParsePhase::finished:
        {
            if (line.length() == 0)
            {
                monkeys.push_back(currentMonkey);
                phase = inputParsePhase::Name;
                currentMonkey = nullptr;
            }
        }

        default:
            break;
        }
    }
    input.close();

    /*  assign the referenced monkey to each other */
    for (auto monkey : monkeys)
    {
        for (auto i : monkeys)
        {
            if (i->monkeyNumber == monkeyReferences.at(monkey->monkeyNumber).first)
                monkey->monkeyReferenceTrue = i;
            if (i->monkeyNumber == monkeyReferences.at(monkey->monkeyNumber).second)
                monkey->monkeyReferenceFalse = i;
        }
    }

    /* let the monkeys play */
    for (size_t i = 0; i < 20; i++)
    {
        for (auto monkey : monkeys)
        {
            monkey->playRound();
        }
    }

    /* find the two monkeys inspected the most */
    int highstMB = 0;
    int secondHighstMB = 0;
    for (auto monkey : monkeys)
    {
        if (monkey->inspectedItems > highstMB)
        {
            secondHighstMB = highstMB;
            highstMB = monkey->inspectedItems;
        }
        else if (monkey->inspectedItems > secondHighstMB)
            secondHighstMB = monkey->inspectedItems;
        std::cout << "Monkey " << monkey->monkeyNumber << " inspected " << monkey->inspectedItems << " items" << std::endl;
    }

    /* calculate the monkey business and print */
    std::cout << "the result is: " << (highstMB * secondHighstMB) << std::endl;

    return 0;
}