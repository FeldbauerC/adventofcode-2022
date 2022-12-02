#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <exception>

int main(int argc, char **argv)
{
    if (argc != 2)
        return -1;

    std::ifstream input;
    input.open(argv[1]);

    int tmp_nr = 0;
    std::vector<int> elf_calories;

    while (!input.eof())
    {
        char line_buf[80];
        std::string line;
        int nr_of_line;
        input.getline(&line_buf[0], sizeof(line_buf));
        line = line_buf;

        try
        {
            if (line.length() >= 1)
            {
                nr_of_line = std::stoi(line);
                tmp_nr += nr_of_line;
            }
            else
            {
                elf_calories.emplace_back(tmp_nr);
                tmp_nr = 0;
            }
        }
        catch (const std::exception &e)
        {
            std::cout << "error: " << e.what() << '\n';
            return -1;
        }
    }
    input.close();

    /* assign the last sum of calories to the last elf */
    elf_calories.emplace_back(tmp_nr);

    tmp_nr = 0;
    /* search for biggest amount of calories among elves */
    for (int elf : elf_calories)
    {
        if (elf > tmp_nr)
            tmp_nr = elf;
    }
    std::cout << "the highest amount of calories among elves is: " << tmp_nr << std::endl;
    return 0;
}