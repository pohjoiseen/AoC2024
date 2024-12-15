#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <set>

struct Input {
    std::map<char, std::vector<std::pair<int, int>>> antennas;
    int width = 0, height = 0;
};

Input read(std::istream &istr)
{
    Input input;
    std::string line;
    while (!istr.eof())
    {
        std::getline(istr, line);
        for (int i = 0; i < line.size(); i++)
        {
            if (line[i] != '.')
            {
                input.antennas[line[i]].push_back(std::make_pair(i, input.height));
            }
        }
        input.width = line.size();  // assume all lines have same length
        input.height++;
    }

    return input;
}

int calculate(const Input &input)
{
    std::set<std::pair<int, int>> result;

    for (auto [c, coords] : input.antennas)
    {
        for (int i = 0; i < coords.size(); i++)
        {
            auto [x1, y1] = coords[i];
            for (int j = i + 1; j < coords.size(); j++)
            {
                auto [x2, y2] = coords[j];
                int dx = x2 - x1, dy = y2 - y1;

                int x11 = x2, y11 = y2;
                for (;;)
                {
                    x11 -= dx;
                    y11 -= dy;
                    if (x11 >= 0 && x11 < input.width &&
                        y11 >= 0 && y11 < input.height)
                    {
                        result.insert(std::make_pair(x11, y11));
                    }
                    else
                    {
                        break;
                    }
                }

                int x21 = x1, y21 = y1;
                for (;;)
                {
                    x21 += dx;
                    y21 += dy;
                    if (x21 >= 0 && x21 < input.width &&
                        y21 >= 0 && y21 < input.height)
                    {
                        result.insert(std::make_pair(x21, y21));
                    }
                    else
                    {
                        break;
                    }
                }
            }
        }
    }

    return result.size();
}

int main(int argc, char **argv)
{
    Input input = read(std::cin);
    int result = calculate(input);
    std::cout << result << std::endl;
    return 0;
}