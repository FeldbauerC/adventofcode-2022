#include <iostream>
#include <fstream>

int main(int argc, char **argv)
{
    std::cout << "called: " << argv[0] << " with " << (argc - 1) << " arguments" << std::endl;
    if (argc != 2)
        return -1;

    int cycle = 1;
    int registerX = 1;
    int signalstrengthSum = 0;

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

        /* check if signal strength should be calculated and summed up */
        if (((cycle + 20) % 40) == 0)
            signalstrengthSum += registerX * cycle;

        /* process input line */
        if (line.substr(0, 4) == "noop")
            cycle++;
        else if (line.substr(0, 4) == "addx")
        {
            /* process addx command */
            int num = 0;
            sscanf(line_buf, "addx %d", &num);
            cycle++; // cycle one of addx command

            /* check if signal strength should be calculated and summed up */
            if (((cycle + 20) % 40) == 0)
                signalstrengthSum += registerX * cycle;

            registerX += num; // update register X
            cycle++;          // cycle two of addx command
        }
        else
            std::cerr << "this line shouldn't be printed" << std::endl;

        /* end when needed cycles done */
        if (cycle > 220)
            break;
    }
    input.close();

    std::cout << "the result is: " << signalstrengthSum << std::endl;

    return 0;
}