#include <iostream>
#include <fstream>

/* split line */
std::pair<std::string, std::string> splitAssignment(std::string elvePair)
{
    std::string assignment1;
    std::string assignment2;

    auto splitIdx = elvePair.find_first_of(',');
    if (splitIdx != std::string::npos)
    {
        try
        {
            assignment1 = elvePair.substr(0, splitIdx);
            assignment2 = elvePair.substr(splitIdx + 1);
        }
        catch (const std::exception &error)
        {
            std::cerr << "splitting string caused error: " << error.what() << '\n';
        }
    }

    return std::make_pair(assignment1, assignment2);
}

/* assign lower and upper ID by string-range */
std::pair<int, int> getRangeOfAssignment(std::string assignment)
{
    /* assumption that all values are positive */
    int lowerID = -255;
    int upperID = -255;
    std::string lowerIDstring;
    std::string upperIDstring;

    auto splitIdx = assignment.find_first_of('-');
    if (splitIdx != std::string::npos)
    {
        try
        {
            lowerID = std::stoi(assignment.substr(0, splitIdx));
            upperID = std::stoi(assignment.substr(splitIdx + 1));
        }
        catch (const std::exception &error)
        {
            std::cerr << "assigning converted string caused error: " << error.what() << '\n';
        }
    }
    return std::make_pair(lowerID, upperID);
}

/* get true or false if one assignment if fully covered by the other elf */
bool fullyContainmentOfOne(std::string elfPair)
{
    std::pair<std::string, std::string> assignments = splitAssignment(elfPair);
    std::pair<int, int> rangeElf1;
    std::pair<int, int> rangeElf2;
    if (assignments.first.empty() || assignments.second.empty())
    {
        std::cerr << "splitting lines failed - assignment is empty" << std::endl;
        return false;
    }

    rangeElf1 = getRangeOfAssignment(assignments.first);
    rangeElf2 = getRangeOfAssignment(assignments.second);

    if (rangeElf1.first == -255 && rangeElf1.second == -255)
    {
        std::cerr << "assigning ID failed" << std::endl;
        return false;
    }
    if (rangeElf2.first == -255 && rangeElf2.second == -255)
    {
        std::cerr << "assigning ID failed" << std::endl;
        return false;
    }

    /* check ranges */
    if (rangeElf1.first <= rangeElf2.first && rangeElf1.second >= rangeElf2.second)
        return true;
    else if (rangeElf2.first <= rangeElf1.first && rangeElf2.second >= rangeElf1.second)
        return true;
    else
        return false;
}

int main(int argc, char **argv)
{
    std::cout << "called: " << argv[0] << " with " << (argc - 1) << " arguments" << std::endl;
    if (argc != 2)
        return -1;

    int assignmentCnt = 0;
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
        /* check assignments */
        if (fullyContainmentOfOne(line))
            assignmentCnt++;
    }
    input.close();

    std::cout << "the result is: " << assignmentCnt << std::endl;

    return 0;
}