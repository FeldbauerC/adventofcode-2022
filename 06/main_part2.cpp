#include <iostream>
#include <fstream>

int main(int argc, char **argv)
{
    std::cout << "called: " << argv[0] << " with " << (argc - 1) << " arguments" << std::endl;
    if (argc != 2)
        return -1;

    std::ifstream input;
    input.open(argv[1]);
    std::streampos curPos = 0;

    while (!input.eof())
    {
        char buffer[14];
        std::string charaterCheck;

        input.seekg(curPos);
        input.read(&buffer[0], sizeof(buffer));
        charaterCheck = buffer;

        if (charaterCheck.length() == 0)
        {
            std::cerr << "charater check is empty" << std::endl;
            continue;
        }
        bool isUnique = true;
        for (size_t i = 0; i < charaterCheck.length(); i++)
        {
            if (int idx = charaterCheck.find(charaterCheck.at(i), i + 1) != std::string::npos)
            {
                curPos += idx;
                isUnique = false;
                break;
            }
            else
            {
                curPos += 1;
            }
        }
        if (isUnique)
            break;
    }
    input.close();

    std::cout << "the result is: " << curPos << std::endl;

    return 0;
}