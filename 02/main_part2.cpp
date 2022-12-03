#include <iostream>
#include <fstream>

enum class RockPaperScissors
{
    ROCK = 1,
    PAPER = 2,
    SCISSORS = 3,
    INVALID = 255
};
enum class GameSolution
{
    DRAW = 3,
    WIN = 6,
    LOOSE = 0,
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

std::pair<RockPaperScissors, GameSolution> classifyCombination(char player1, char solution)
{
    RockPaperScissors rpcPlayer1;
    GameSolution requiredSolution;

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

    switch (std::toupper(solution))
    {
    case 'X':
        requiredSolution = GameSolution::LOOSE;
        break;
    case 'Y':
        requiredSolution = GameSolution::DRAW;
        break;
    case 'Z':
        requiredSolution = GameSolution::WIN;
        break;

    default:
        requiredSolution = GameSolution::INVALID;
        break;
    }
    return std::make_pair(rpcPlayer1, requiredSolution);
}

RockPaperScissors getWinningMove(RockPaperScissors playedMove)
{
    switch (playedMove)
    {
    case RockPaperScissors::ROCK:
        return RockPaperScissors::PAPER;
    case RockPaperScissors::PAPER:
        return RockPaperScissors::SCISSORS;
    case RockPaperScissors::SCISSORS:
        return RockPaperScissors::ROCK;
    default:
        return RockPaperScissors::INVALID;
    }
}

RockPaperScissors getMoveToLoose(RockPaperScissors playedMove)
{
    switch (playedMove)
    {
    case RockPaperScissors::ROCK:
        return RockPaperScissors::SCISSORS;
    case RockPaperScissors::PAPER:
        return RockPaperScissors::ROCK;
    case RockPaperScissors::SCISSORS:
        return RockPaperScissors::PAPER;
    default:
        return RockPaperScissors::INVALID;
    }
}
solutionRPC getScorePlayer2(char player1, char requiredSolution)
{
    struct solutionRPC ret;
    auto rpcMode = classifyCombination(player1, requiredSolution);

    if (rpcMode.first == RockPaperScissors::INVALID || rpcMode.second == GameSolution::INVALID)
    {
        ret.returnCode = ReturnType::returnInvalidOption;
        return ret;
    }
    switch (rpcMode.second)
    {
    case GameSolution::WIN:
        ret.scoreRound = static_cast<int>(rpcMode.second) + static_cast<int>(getWinningMove(rpcMode.first));
        break;

    case GameSolution::LOOSE:
        ret.scoreRound = static_cast<int>(rpcMode.second) + static_cast<int>(getMoveToLoose(rpcMode.first));
        break;
    case GameSolution::DRAW:
        ret.scoreRound = static_cast<int>(rpcMode.second) + static_cast<int>(rpcMode.first);
        break;
    default:
        ret.returnCode = ReturnType::returnInvalidOption;
        break;
    }

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