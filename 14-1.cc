#include <iostream>
#include <vector>
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

void iterate(Input &input, int width, int height, int iters)
{
    for (int i = 0; i < iters; i++)
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
    }
}

long long summarize(const Input &input, int width, int height)
{
    long long a = 0, b = 0, c = 0, d = 0;

    int x1, y1, x2, y2;
    x1 = width / 2 - 1;
    y1 = height / 2 - 1;
    x2 = (width % 2) ? width / 2 + 1 : width / 2;
    y2 = (height % 2) ? height / 2 + 1 : height / 2;

    for (auto &[x, y, p, q] : input)
    {
        if      (x <= x1 && y <= y1) a++;
        else if (x >= x2 && y <= y1) b++;
        else if (x <= x1 && y >= y2) c++;
        else if (x >= x2 && y >= y2) d++;
    }

    return a * b * c * d;
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
    iterate(input, width, height, 100);
    std::cout << summarize(input, width, height) << std::endl;
    return 0;
}