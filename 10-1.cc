#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <deque>

using Input = std::vector<std::string>;

Input readInput()
{
    Input input;
    std::string line;
    while (!std::cin.eof())
    {
        std::getline(std::cin, line);
        input.push_back(line);
    }
    return input;
}

std::set<std::pair<int, int>> getTrailheadsForMountain(const Input &input, int x, int y)
{
    std::set<std::pair<int, int>> result;
    std::deque<std::pair<int, int>> deque;
    deque.push_back(std::make_pair(x, y));
    while (deque.size() > 0)
    {
        auto [x, y] = deque.front();
        deque.pop_front();
        char c = input[y][x];
        if (c == '0')
        {
            result.insert(std::make_pair(x, y));
        }
        else
        {
            if (x > 0 && input[y][x - 1] == c - 1)
            {
                deque.push_back(std::make_pair(x - 1, y));
            }
            if (y > 0 && input[y - 1][x] == c - 1)
            {
                deque.push_back(std::make_pair(x, y - 1));
            }
            if (x < input[y].size() && input[y][x + 1] == c - 1)
            {
                deque.push_back(std::make_pair(x + 1, y));
            }
            if (y < input.size() - 1 && input[y + 1][x] == c - 1)
            {
                deque.push_back(std::make_pair(x, y + 1));
            }
        }
    }
    return result;
}

int calculate(const Input &input)
{
    std::map<std::pair<int, int>, int> trailheads;

    for (int i = 0; i < input.size(); i++)
    {
        for (int j = 0; j < input.size(); j++)
        {
            if (input[i][j] == '9')
            {
                auto trailheadsForMountain = getTrailheadsForMountain(input, j, i);
                for (auto coords : trailheadsForMountain)
                {
                    trailheads[coords]++;
                }
            }
        }
    }

    int result = 0;
    for (auto [coords, num] : trailheads)
    {
        result += num;
    }
    return result;
}

int main(int argc, char **argv)
{
    Input input = readInput();
    int result = calculate(input);
    std::cout << result << std::endl;
    return 0;
}