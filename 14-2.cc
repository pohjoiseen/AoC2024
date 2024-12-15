// this looks for a pattern with at least 10 robots in a horizontal row
// (visualizing it to stderr), which in practice solves this task for the given input,
// but of course not guaranteed to solve for arbitrary input.  In practice
// I just found this pattern manually and adjusted the code so that it stops on it
#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdio>

using Input = std::vector<std::tuple<int, int, int, int>>;

Input readInput()
{
    Input input;
    std::string line;

    while (!std::cin.eof())
    {
        std::getline(std::cin, line);
        auto &[x, y, p, q] = input.emplace_back();
        if (4 != std::sscanf(line.c_str(), "p=%d,%d v=%d,%d", &x, &y, &p, &q))
        {
            std::cerr << "Failed to parse line: " << line << std::endl;
            std::exit(1);
        }
    }

    return input;
}

void visualize(const Input &input, int width, int height)
{
    std::vector<std::string> field(height);
    for (auto [x, y, p, q] : input)
    {
        if (!field[y].size())
        {
            field[y].resize(width, ' ');
        }
        if (field[y][x] == ' ')
        {
            field[y][x] = '1';
        }
        else
        {
            field[y][x]++;
        }
    }
    for (auto s : field) std::cerr << s << std::endl;
}

bool checkForPossiblePattern(Input &input)
{
    std::sort(input.begin(), input.end(), [](auto a, auto b) {
        auto [x1, y1, p1, q1] = a;
        auto [x2, y2, p2, q2] = b;

        if (y1 < y2) return true;
        if (y1 > y2) return false;
        return x1 < x2;
    });

    int len = 0;
    for (int i = 0; i < input.size() - 1; i++)
    {
        auto [x1, y1, p1, q1] = input[i];
        auto [x2, y2, p2, q2] = input[i + 1];
        if (x1 + 1 == x2 && y1 == y2)
        {
            len++;
            if (len >= 10) return true;
        }
        else
        {
            len = 0;
        }
    }

    return false;
}

int iterate(Input &input, int width, int height)
{
    for (int i = 1;; i++)
    {
        for (auto &[x, y, p, q] : input)
        {
            x += p;
            y += q;
            if (x < 0) x += width;
            if (y < 0) y += height;
            if (x >= width) x -= width;
            if (y >= height) y -= height;
        }

        if (checkForPossiblePattern(input))
        {
            visualize(input, width, height);
            return i;
        }
    }
}

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        std::cerr << "Provide field size (width height) as arguments!" << std::endl;
        return 1;
    }

    int width = std::atoi(argv[1]), height = std::atoi(argv[2]);
    if (width <= 0 || height <= 0)
    {
        std::cerr << "Invalid width or height!" << std::endl;
        return 1;
    }

    Input input = readInput();
    int result = iterate(input, width, height);
    std::cout << result << std::endl;
    return 0;
}