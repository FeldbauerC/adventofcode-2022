#include <iostream>
#include <fstream>

enum class RockPaperScissors
{
    ROCK = 1,
    PAPER = 2,
    SCISSORS = 3,
    INVALID = 255
};

enum class ReturnType
{
    returnInvalidOption = -2,
    returnGenericError = -1,
    returnOk = 0
};

struct solutionRPC
{
    int scoreRound;
    ReturnType returnCode;
};

std::pair<RockPaperScissors, RockPaperScissors> classifyCombination(char player1, char player2)
{
    RockPaperScissors rpcPlayer1;
    RockPaperScissors rpcPlayer2;

    switch (std::toupper(player1))
    {
    case 'A':
        rpcPlayer1 = RockPaperScissors::ROCK;
        break;
    case 'B':
        rpcPlayer1 = RockPaperScissors::PAPER;
        break;
    case 'C':
        rpcPlayer1 = RockPaperScissors::SCISSORS;
        break;

    default:
        rpcPlayer1 = RockPaperScissors::INVALID;
        break;
    }

    switch (std::toupper(player2))
    {
    case 'X':
        rpcPlayer2 = RockPaperScissors::ROCK;
        break;
    case 'Y':
        rpcPlayer2 = RockPaperScissors::PAPER;
        break;
    case 'Z':
        rpcPlayer2 = RockPaperScissors::SCISSORS;
        break;

    default:
        rpcPlayer1 = RockPaperScissors::INVALID;
        break;
    }
    return std::make_pair(rpcPlayer1, rpcPlayer2);
}
solutionRPC getScorePlayer2(char player1, char player2)
{
    struct solutionRPC ret;
    bool draw = false;
    bool winner = false;
    int score = 0;
    auto rpcMode = classifyCombination(player1, player2);

    if (rpcMode.first == RockPaperScissors::INVALID || rpcMode.second == RockPaperScissors::INVALID)
    {
        ret.returnCode = ReturnType::returnInvalidOption;
        return ret;
    }

    if (rpcMode.first == RockPaperScissors::ROCK && rpcMode.second == RockPaperScissors::ROCK)
        draw = true;
    else if (rpcMode.first == RockPaperScissors::ROCK && rpcMode.second == RockPaperScissors::PAPER)
        winner = true;
    else if (rpcMode.first == RockPaperScissors::PAPER && rpcMode.second == RockPaperScissors::PAPER)
        draw = true;
    else if (rpcMode.first == RockPaperScissors::PAPER && rpcMode.second == RockPaperScissors::SCISSORS)
        winner = true;
    else if (rpcMode.first == RockPaperScissors::SCISSORS && rpcMode.second == RockPaperScissors::ROCK)
        winner = true;
    else if (rpcMode.first == RockPaperScissors::SCISSORS && rpcMode.second == RockPaperScissors::SCISSORS)
        draw = true;

    score = static_cast<int>(rpcMode.second);
    if (draw)
        ret.scoreRound = score + 3;
    else if (winner)
        ret.scoreRound = score + 6;
    else
        ret.scoreRound = score;

    ret.returnCode = ReturnType::returnOk;
    return ret;
}

int main(int argc, char **argv)
{
    std::cout << "called: " << argv[0] << " with " << (argc - 1) << " arguments" << std::endl;
    if (argc != 2)
        return static_cast<int>(ReturnType::returnGenericError);

    int score = 0;
    std::ifstream input;
    input.open(argv[1]);

    while (!input.eof())
    {
        char line_buf[80];
        std::string line;

        input.getline(&line_buf[0], sizeof(line_buf));
        line = line_buf;

        if (line.length() == 3)
        {
            struct solutionRPC ret;

            ret = getScorePlayer2(line.at(0), line.at(2));
            if (ret.returnCode == ReturnType::returnOk)
                score += ret.scoreRound;
            else
                std::cerr << "invalid combination" << std::endl;
        }
        else if (line.length() == 0)
        {
            std::cerr << "parsed empty line" << std::endl;
            continue;
        }
        else
        {
            std::cerr << "invalid line" << std::endl;
            return static_cast<int>(ReturnType::returnGenericError);
        }
    }
    input.close();

    std::cout << "the score is: " << score << std::endl;
    return static_cast<int>(ReturnType::returnOk);
}