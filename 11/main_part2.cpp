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
    monkey(uint32_t name);
    ~monkey() = default;

    /* methods */
    void getItemPassed(uint64_t item);
    void playRound();

    uint32_t monkeyNumber;
    std::list<uint64_t> items;
    std::function<uint64_t(uint64_t, uint32_t)> worryLevel;
    uint32_t worryFactor;
    uint32_t manageWorryLevel;
    uint32_t testDivisibility;
    uint64_t inspectedItems;
    monkey *monkeyReferenceTrue;
    monkey *monkeyReferenceFalse;
};

monkey::monkey(uint32_t name)
    : monkeyNumber(name)
{
    inspectedItems = 0;
}

void monkey::getItemPassed(uint64_t item)
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
        uint64_t currentItemOfInterest = item;

        /* calculate new worry level after started playing */
        currentItemOfInterest = worryLevel(currentItemOfInterest, this->worryFactor);

        /* manage my worries - thanks to reddit */
        currentItemOfInterest = currentItemOfInterest % this->manageWorryLevel;

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
    std::map<uint32_t, std::pair<uint32_t, uint32_t>> monkeyReferences;

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
            uint32_t monkeyName;
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
                    uint64_t item = std::stoi(itemlist.substr(i, (idx - i)));
                    currentMonkey->getItemPassed(item);
                }
                else if (idx == std::string::npos && i != (itemlist.length() - 1)) // for last or single element in list
                {
                    uint64_t item = std::stoi(itemlist.substr(i));
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
            uint32_t argVal = 0;
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
                    currentMonkey->worryLevel = [](uint64_t old, uint32_t arg2)
                    { return old + arg2; };
                else
                    currentMonkey->worryLevel = [](uint64_t old, uint32_t arg2)
                    { return old + old; };
                break;

            case '*':
                if (argVal)
                    currentMonkey->worryLevel = [](uint64_t old, uint32_t arg2)
                    { return old * arg2; };
                else
                    currentMonkey->worryLevel = [](uint64_t old, uint32_t arg2)
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
            uint32_t testD;
            sscanf(line_buf, "  Test: divisible by %d", &testD);
            currentMonkey->testDivisibility = testD;
            phase = inputParsePhase::monkeyRefTrue;
            break;
        }
        case inputParsePhase::monkeyRefTrue:
        {
            uint32_t monkeyRef;
            sscanf(line_buf, "    If true: throw to monkey %d", &monkeyRef);
            monkeyReferences.emplace(currentMonkey->monkeyNumber, std::make_pair(monkeyRef, -1));
            phase = inputParsePhase::monkeyRefFalse;
            break;
        }
        case inputParsePhase::monkeyRefFalse:
        {
            uint32_t monkeyRef;
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

    /* calculate kgv & assign the referenced monkey to each other */
    uint32_t kgv = 1;
    for (auto monkey : monkeys)
    {
        kgv *= monkey->testDivisibility;
        for (auto i : monkeys)
        {
            if (i->monkeyNumber == monkeyReferences.at(monkey->monkeyNumber).first)
                monkey->monkeyReferenceTrue = i;
            if (i->monkeyNumber == monkeyReferences.at(monkey->monkeyNumber).second)
                monkey->monkeyReferenceFalse = i;
        }
    }

    /* assign calculated kgv to every monkey */
    for (auto monkey : monkeys)
        monkey->manageWorryLevel = kgv;

    /* let the monkeys play */
    for (size_t i = 0; i < 10000; i++)
    {
        for (auto monkey : monkeys)
        {
            monkey->playRound();
        }
    }

    /* find the two monkeys inspected the most */
    uint64_t highstMonkeyBusiness = 0;
    uint64_t secondHighstMonkeyBusiness = 0;
    for (auto monkey : monkeys)
    {
        if (monkey->inspectedItems > highstMonkeyBusiness)
        {
            secondHighstMonkeyBusiness = highstMonkeyBusiness;
            highstMonkeyBusiness = monkey->inspectedItems;
        }
        else if (monkey->inspectedItems > secondHighstMonkeyBusiness)
            secondHighstMonkeyBusiness = monkey->inspectedItems;
        std::cout << "Monkey " << monkey->monkeyNumber << " inspected " << monkey->inspectedItems << " items" << std::endl;
    }

    /* calculate the monkey business */
    uint64_t result = highstMonkeyBusiness * secondHighstMonkeyBusiness;
    std::cout << "the result is: " << result << " " << highstMonkeyBusiness << " & " << secondHighstMonkeyBusiness << std::endl;

    return 0;
}
