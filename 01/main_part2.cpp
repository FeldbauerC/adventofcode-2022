#include <iostream>
#include <fstream>
#include <vector>
#include <string>

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
            std::cerr << "error: " << e.what() << '\n';
        }
    }
    input.close();

    /* assign the last sum of calories to the last elf */
    elf_calories.emplace_back(tmp_nr);

    int calories_first = 0;
    int calories_second = 0;
    int calories_third = 0;

    /* search for biggest amount of calories among elves */
    for (int elf : elf_calories)
    {
        if (elf >= calories_first)
        {
            calories_third = calories_second;
            calories_second = calories_first;
            calories_first = elf;
        }
        else if (elf >= calories_second)
        {
            calories_third = calories_second;
            calories_second = elf;
        }
        else if (elf >= calories_third)
        {
            calories_third = elf;
        }
    }
    std::cout << "the highest amount of calories among all elves is: " << calories_first << std::endl;
    std::cout << "the second most calories are: " << calories_second << " and the third most are: " << calories_third << std::endl;

    int sum_calories_first_three = calories_first + calories_second + calories_third;
    std::cout << "the sum of the three most carrying elves is " << sum_calories_first_three << " calories " << std::endl;
    return 0;
}