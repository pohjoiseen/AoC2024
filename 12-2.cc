#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <algorithm>

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

// given a vector of all horizontal edges of individual tiles,
// calculate how many continuous edges possibly spanning several tiles they form
// this only handles horizontal edges, but in measure() coordinates in vertical edge
// vector are flipped, so same function can be used
// note, changes (sorts) argument
int countContinuousEdges(std::vector<std::tuple<int, int, bool>> &edges)
{
    if (!edges.size()) return 0;  // should not happen

    // sort all edges top to bottom, left to right
    std::sort(edges.begin(), edges.end(), [](auto a, auto b) {
        auto [x1, y2, dir1] = a;
        auto [x2, y1, dir2] = b;

        if (y1 < y2) return true;
        if (y1 > y2) return false;
        return x1 < x2;
    });

    // with the sorted vector, we only need to count all discontinuities
    // at least one side, then compare every edge with previous one
    int result = 1;
    for (int i = 1; i < edges.size(); i++)
    {
        auto [x, y, dir] = edges[i];
        auto [x1, y1, dir1] = edges[i - 1];
        // vertical discontinuity
        if (y != y1) result++;
        // horizontal discontinuity
        else if (x - 1 != x1) result++;
        // direction discontinuity
        else if (dir != dir1) result++;
    }

    return result;
}

int measure(const Input &input, VisitedState &visitedState, int width, int height, int x, int y)
{
    std::deque<std::pair<int, int>> deque{ { x, y } };
    int area = 0;
    // horizontal edges = coordinates of tile above the edge (y can be -1), + direction bit
    // vertical edges = SWAPPED coordinates of tile left of the edge (x can be -1)
    std::vector<std::tuple<int, int, bool>> hedges, vedges;
    char c = input[y][x];

    // iterate while there are still points
    while (deque.size() > 0)
    {
        // new point
        auto [x, y] = deque.front();
        deque.pop_front();

        // skip already visited
        if (visitedState[y][x]) continue;

        // mark as visited
        visitedState[y][x] = true;
        area++;

        // check adjacent points, add to edge vectors if this is an edge,
        // and enqueue if not
        if (x > 0 && input[y][x - 1] == c)
            deque.push_back({ x - 1, y });
        else
            vedges.push_back({ y, x - 1, false });
        if (y > 0 && input[y - 1][x] == c)
            deque.push_back({ x, y - 1 });
        else
            hedges.push_back({ x, y - 1, false });
        if (x < width - 1 && input[y][x + 1] == c)
            deque.push_back({ x + 1, y });
        else
            vedges.push_back({ y, x, true });
        if (y < height - 1 && input[y + 1][x] == c)
            deque.push_back({ x, y + 1 });
        else
            hedges.push_back({ x, y, true });
    }

    return area * (countContinuousEdges(hedges) + countContinuousEdges(vedges));
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
