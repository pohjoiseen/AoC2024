#include <iostream>
#include <string>
#include <vector>

int main(int argc, char **argv)
{
    std::vector<std::string> input;

    while (!std::cin.eof())
    {
        std::string line;
        std::getline(std::cin, line);
        input.push_back(line);
    }

    int result = 0;
    for (int i = 1; i < input.size() - 1; i++)
    {
        for (int j = 1; j < input[i].size() -1; j++)
        {
            if (input[i][j] == 'A')
            {
                if (input[i - 1][j - 1] == 'M' &&
                    input[i - 1][j + 1] == 'M' &&
                    input[i + 1][j - 1] == 'S' &&
                    input[i + 1][j + 1] == 'S')
                {
                    result++;
                }

                if (input[i - 1][j - 1] == 'M' &&
                    input[i - 1][j + 1] == 'S' &&
                    input[i + 1][j - 1] == 'M' &&
                    input[i + 1][j + 1] == 'S')
                {
                    result++;
                }

                if (input[i - 1][j - 1] == 'S' &&
                    input[i - 1][j + 1] == 'M' &&
                    input[i + 1][j - 1] == 'S' &&
                    input[i + 1][j + 1] == 'M')
                {
                    result++;
                }

                if (input[i - 1][j - 1] == 'S' &&
                    input[i - 1][j + 1] == 'S' &&
                    input[i + 1][j - 1] == 'M' &&
                    input[i + 1][j + 1] == 'M')
                {
                    result++;
                }
            }
        }
    }

    std::cout << result << std::endl;

    return 0;
}