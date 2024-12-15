#include <iostream>
#include <vector>

using Input = std::vector<int>;

Input readInput()
{
    Input input;
    int k = 0;
    while (!std::cin.eof())
    {
        char blocks, empty;
        std::cin >> blocks >> empty;
        for (char i = '0'; i < blocks; i++)
        {
            input.push_back(k);
        }
        for (char i = '0'; i < empty; i++)
        {
            input.push_back(-1);
        }
        k++;
    }
    return input;
}

void process(Input &input)
{
    auto it1 = input.begin(), it2 = input.end();
    do
    {
        it2--;
        if (*it2 != -1)
        {
            while (*it1 != -1) it1++;
            if (it1 >= it2)
            {
                break;
            }
            *it1 = *it2;
            *it2 = -1;
        }
    } while (it2 > input.begin());
}

long long summarize(const Input &input)
{
    long long result = 0;
    for (int i = 0; i < input.size(); i++)
    {
        if (input[i] == -1)
        {
            break;
        }
        result += i * input[i];
    }
    return result;
}

int main(int argc, char **argv)
{
    Input input = readInput();
    process(input);
    std::cout << summarize(input) << std::endl;
    return 0;
}

