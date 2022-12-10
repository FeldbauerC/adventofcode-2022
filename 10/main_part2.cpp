#include <iostream>
#include <fstream>
#include <vector>

void setSprite(std::vector<char> *sprite, int posNew, int posOld)
{
    std::vector<int> postionsDelete;
    std::vector<int> postionsDraw;
    
    for (size_t i = 0; i < 3; i++)
    {
        postionsDelete.push_back(posOld + (i - 1));
        postionsDraw.push_back(posNew + (i - 1));
    }

    /* remove old sprite */
    for (auto pos : postionsDelete)
    {
        if (pos >= 0 && pos < 40)
            sprite->at(pos) = '.';
    }

    /* draw new sprite */
    for (auto pos : postionsDraw)
    {
        if (pos >= 0 && pos < 40)
            sprite->at(pos) = '#';
    }
}

int main(int argc, char **argv)
{
    std::cout << "called: " << argv[0] << " with " << (argc - 1) << " arguments" << std::endl;
    if (argc != 2)
        return -1;

    int cycle = 1;
    int position = 1;
    int registerX = 1;
    std::vector<char> sprite;

    /* fill sprite with dark pixels -->'.' */
    for (size_t i = 0; i < 40; i++)
        sprite.push_back('.');

    /* set first sprite and draw first pixel of sprite*/
    setSprite(&sprite, registerX, 0);
    std::cout << sprite.at(position);

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

        /* check if line break is needed */
        if ((position % 40) == 0)
        {
            position = 0;
            std::cout << std::endl;
        }
        /* draw pixel */
        std::cout << sprite.at(position);

        /* process input line */
        if (line.substr(0, 4) == "noop")
        {
            cycle++;
            position++;
        }
        else if (line.substr(0, 4) == "addx")
        {
            /* process addx command */
            int num = 0;
            sscanf(line_buf, "addx %d", &num);

            /* update cycle and position */
            cycle++;
            position++;

            /* check if line break is needed */
            if ((position % 40) == 0)
            {
                position = 0;
                std::cout << std::endl;
            }

            int oldRegisterX = registerX;                // save old value of register X
            registerX += num;                            // update register X
            setSprite(&sprite, registerX, oldRegisterX); // update sprite

            /* draw pixel */
            std::cout << sprite.at(position);

            /* update cycle and position */
            cycle++;
            position++;
        }
        else
            std::cerr << "this line shouldn't be printed" << std::endl;

        /* end when needed cycles done */
        if (cycle >= 240)
            break;
    }
    input.close();

    std::cout << std::endl;

    return 0;
}