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
    for (int i = 0; i < input.size(); i++)
    {
        for (int j = 0; j < input[i].size(); j++)
        {
            if (input[i][j] == 'X')
            {
                if (j <= input[i].size() - 4 &&
                    input[i][j + 1] == 'M' &&
                    input[i][j + 2] == 'A' &&
                    input[i][j + 3] == 'S')
                {
                    result++;
                    //std::cerr << i << j << "R" << std::endl;
                }

                if (j >= 3 &&
                    input[i][j - 1] == 'M' &&
                    input[i][j - 2] == 'A' &&
                    input[i][j - 3] == 'S')
                {
                    result++;
                    //std::cerr << i << j << "L" << std::endl;
                }

                if (i <= input.size() - 4 &&
                    input[i + 1][j] == 'M' &&
                    input[i + 2][j] == 'A' &&
                    input[i + 3][j] == 'S')
                {
                    result++;
                    //std::cerr << i << j << "D" << std::endl;
                }

                if (i >= 3 &&
                    input[i - 1][j] == 'M' &&
                    input[i - 2][j] == 'A' &&
                    input[i - 3][j] == 'S')
                {
                    result++;
                    //std::cerr << i << j << "U" << std::endl;
                }

                if (j <= input[i].size() - 4 &&
                    i <= input.size() - 4 &&
                    input[i + 1][j + 1] == 'M' &&
                    input[i + 2][j + 2] == 'A' &&
                    input[i + 3][j + 3] == 'S')
                {
                    result++;
                    //std::cerr << i << j << "RD" << std::endl;
                }

                if (j >= 3 &&
                    i <= input.size() - 4 &&
                    input[i + 1][j - 1] == 'M' &&
                    input[i + 2][j - 2] == 'A' &&
                    input[i + 3][j - 3] == 'S')
                {
                    result++;
                    //std::cerr << i << j << "LD" << std::endl;
                }

                if (j <= input[i].size() - 4 &&
                    i >= 3  &&
                    input[i - 1][j + 1] == 'M' &&
                    input[i - 2][j + 2] == 'A' &&
                    input[i - 3][j + 3] == 'S')
                {
                    result++;
                    //std::cerr << i << j << "RU" << std::endl;
                }

                if (j >= 3 &&
                    i >= 3 &&
                    input[i - 1][j - 1] == 'M' &&
                    input[i - 2][j - 2] == 'A' &&
                    input[i - 3][j - 3] == 'S')
                {
                    result++;
                    //std::cerr << i << j << "LU" << std::endl;
                }
            }
        }
    }

    std::cout << result << std::endl;

    return 0;
}