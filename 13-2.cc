#include <iostream>
#include <string>
#include <vector>
#include <cstdio>
#include <cmath>

using Input = std::vector<std::tuple<int, int, int, int, int, int>>;

Input readInput()
{
    Input input;
    std::string line;
    while (!std::cin.eof())
    {
        auto &tuple = input.emplace_back();

        std::getline(std::cin, line);
        if (2 != std::sscanf(line.c_str(), "Button A: X+%d, Y+%d",
            &std::get<0>(tuple), &std::get<3>(tuple)))
        {
            std::cerr << "Failed to parse line: " << line << std::endl;
            std::exit(1);
        }

        std::getline(std::cin, line);
        if (2 != std::sscanf(line.c_str(), "Button B: X+%d, Y+%d",
            &std::get<1>(tuple), &std::get<4>(tuple)))
        {
            std::cerr << "Failed to parse line: " << line << std::endl;
            std::exit(1);
        }

        std::getline(std::cin, line);
        if (2 != std::sscanf(line.c_str(), "Prize: X=%d, Y=%d",
            &std::get<2>(tuple), &std::get<5>(tuple)))
        {
            std::cerr << "Failed to parse line: " << line << std::endl;
            std::exit(1);
        }

        std::getline(std::cin, line);  // skip empty line
    }
    return input;
}

long long calculate(const Input &input)
{
    long long result = 0;
    for (auto [a, b, c, d, e, f] : input)
    {
        int n = a * e - b * d;
        if (!n) continue;

        double c1 = c * 1.0 + 10000000000000;
        double f1 = f * 1.0 + 10000000000000;

        double x = (a * f1 - c1 * d) * 1.0 / n;
        double y = (c1 * e - b * f1) * 1.0 / n;
        if (x != std::floor(x) || y != std::floor(y)) continue;

        result += y * 3 + x;
    }
    return result;
}

int main(int argc, char **argv)
{
    Input input = readInput();
    long long result = calculate(input);
    std::cout << result << std::endl;
    return 0;
}