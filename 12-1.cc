#include <iostream>
#include <string>
#include <vector>
#include <deque>

using Input = std::vector<std::string>;
using VisitedState = std::vector<std::vector<bool>>;

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

int measure(const Input &input, VisitedState &visitedState, int width, int height, int x, int y)
{
    std::deque<std::pair<int, int>> deque{ { x, y } };
    int area = 0, perimeter = 0;
    char c = input[y][x];
    //std::cout << c << std::endl;

    // iterate while there are still points
    while (deque.size() > 0)
    {
        // new point
        auto [x, y] = deque.front();
        deque.pop_front();

        //std::cout << '(' << x << ',' << y << ')' << std::endl;

        // skip already visited
        if (visitedState[y][x]) continue;

        // mark as visited
        visitedState[y][x] = true;
        area++;

        // check adjacent points, add to perimeter if this is an edge,
        // and enqueue if not
        if (x > 0 && input[y][x - 1] == c)
            deque.push_back({ x - 1, y });
        else
            perimeter++;
        if (y > 0 && input[y - 1][x] == c)
            deque.push_back({ x, y - 1 });
        else
            perimeter++;
        if (x < width - 1 && input[y][x + 1] == c)
            deque.push_back({ x + 1, y });
        else
            perimeter++;
        if (y < height - 1 && input[y + 1][x] == c)
            deque.push_back({ x, y + 1 });
        else
            perimeter++;
    }

    //std::cout << "area = " << area << ", perimeter = " << perimeter << std::endl;

    return area * perimeter;
}

int process(const Input &input)
{
    int width = input[0].size(), height = input.size();  // do not check if actually rectangular

    // create 2D grid to track visited locations
    VisitedState visitedState;
    for (auto line : input)
    {
        visitedState.emplace_back(std::vector<bool>(line.size()));
    }

    int result = 0;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (!visitedState[i][j])
            {
                result += measure(input, visitedState, width, height, j, i);
            }
        }
    }

    return result;
}

int main(int argc, char **argv)
{
    Input input = readInput();
    auto result = process(input);
    std::cout << result << std::endl;
    return 0;
}
