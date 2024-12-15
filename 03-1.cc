#include <iostream>
#include <string>
#include <cstdio>
#include <cstring>

int main(int argc, char **argv)
{
    int sum = 0;
    // should only be one line but handle any number anyway
    while (!std::cin.eof())
    {
        int k = 0, num1, num2, result;
        std::string line;

        // get a line and loop through it
        std::getline(std::cin, line);
        while (k < line.size())
        {
            // look for "mul("
            if (k <= line.size() - 4 &&
                line[k] == 'm' &&
                line[k + 1] == 'u' &&
                line[k + 2] == 'l' &&
                line[k + 3] == '(')
            {
                // skip over it
                k += 4;
                // look for first integer number
                result = std::sscanf(line.c_str() + k, "%d", &num1);
                if (result)
                {
                    // skip over it
                    while (line[k] >= '0' && line[k] <= '9')
                    {
                        k++;
                    }

                    // look for ","
                    if (line[k] == ',')
                    {
                        // skip over it
                        k++;

                        // look for second integer number
                        result = std::sscanf(line.c_str() + k, "%d", &num2);
                        if (result)
                        {
                            // skip over it
                            while (line[k] >= '0' && line[k] <= '9')
                            {
                                k++;
                            }

                            // look for ")"
                            if (line[k] == ')')
                            {
                                // skip over it
                                k++;

                                // if all elements are found,
                                // actually add the product of these
                                // numbers
                                sum += num1 * num2;
                            }
                        }
                    }
                }
            }
            // nothing found, go to next character
            else
            {
                k++;
            }
        }
    }

    std::cout << sum << std::endl;

    return 0;
}