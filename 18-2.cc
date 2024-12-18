#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <cstdio>

struct Input
{
    std::vector<std::pair<int, int>> obstacles;
    int width, height;
    int take;
};

struct SearchStep
{
    int x, y;
    int cost;
    int distEstimate;
};

Input readInput(int argc, char **argv)
{
    if (argc < 3)
    {
        std::cerr << "Not enough arguments!  Use: 18-2 <width> <height>" << std::endl;
        std::exit(1);
    }

    Input input;
    input.width = std::atoi(argv[1]);
    input.height = std::atoi(argv[2]);
    if (!input.width || !input.height || !input.take)
    {
        std::cerr << "One or more arguments invalid!  Use: 18-1 <width> <height>" << std::endl;
        std::exit(1);
    }

    while (!std::cin.eof())
    {
        auto &coords = input.obstacles.emplace_back();
        std::string line;
        std::getline(std::cin, line);
        if (2 != std::sscanf(line.c_str(), "%d,%d", &coords.first, &coords.second))
        {
            std::cerr << "Failed to read coords!" << std::endl;
            std::exit(1);
        }
    }

    return input;
}

void visualize(std::vector<std::string> &map, std::map<std::pair<int, int>, std::pair<int, int>> &pathMap)
{
    int x = map[0].size() - 1, y = map.size() - 1;
    do
    {
        map[y][x] = 'O';
        auto [x1, y1] = pathMap[std::make_pair(x, y)];
        x = x1;
        y = y1;
    } while (x != 0 || y != 0);
    for (auto s : map) std::cout << s << std::endl;
}

int pathfind(const Input &input)
{
    // build the actual map
    std::vector<std::string> map(input.height);
    for (int i = 0; i < input.height; i++) map[i].resize(input.width, '.');
    for (int i = 0; i < input.take; i++)
    {
        auto [x, y] = input.obstacles[i];
        map[y][x] = '#';
    }

    // function to order search steps for priority queue
    // sign must be inverted, as the queue picks the top item, but it must be one with
    // the lowest cost
    auto searchStepCmp = [](SearchStep &a, SearchStep &b) {
        return (a.cost + a.distEstimate) > (b.cost + b.distEstimate);
    };
    // function to estimate distance to the end, by just taking horizontal+vertical difference
    auto calcDistEstimate = [&](int x, int y) {
        return std::abs(input.width - 1 - x) + std::abs(input.height - 1 - y);
    };

    // priority queue for A* 
    std::priority_queue<SearchStep, std::vector<SearchStep>, decltype(searchStepCmp)> searchQueue(searchStepCmp);
    // minimal cost for found nodes
    std::map<std::pair<int, int>, int> minCost;
    // for tracking the path (only for visualization), node -> previous node;
    std::map<std::pair<int, int>, std::pair<int, int>> pathMap;

    // start with initial square
    searchQueue.emplace(SearchStep { 0, 0, 0, calcDistEstimate(input.width, input.height) });

    std::pair<int, int> moves[] = { { -1, 0 }, { 0, -1 }, { 1, 0 }, { 0, 1 } };

    while (!searchQueue.empty())
    {
        // get the square with the lowest cost + remaining distance estimate
        // copy here, not reference, as searchQueue.emplace() would invalidate the reference
        const SearchStep cur = searchQueue.top();

        // examine possible moves from the current square
        for (const auto &[dx, dy] : moves)
        {
            int x = cur.x + dx, y = cur.y + dy;
            auto coords = std::make_pair(x, y);
            
            // move must not lead beyond map limits or into a wall
            if (x < 0 || y < 0 || x >= input.width || y >= input.height) continue;
            if (map[y][x] == '#') continue;

            // estimate cost until the end
            int distEstimate = calcDistEstimate(x, y);

            // it's actually the destination!
            if (!distEstimate)
            {
                pathMap[coords] = std::make_pair(cur.x, cur.y);
                //visualize(map, pathMap);
                return cur.cost + 1;
            }

            // check that this square was not previously already found with lower cost
            if (minCost.find(coords) == minCost.end() || cur.cost + 1 < minCost[coords])
            {
                // enqueue
                minCost[coords] = cur.cost + 1;
                pathMap[coords] = std::make_pair(cur.x, cur.y);
                searchQueue.emplace(SearchStep { x, y, cur.cost + 1, distEstimate });
            }
        }

        searchQueue.pop();
    }

    // queue exhausted and no possible path found
    return -1;
}

int searchForSolution(Input &input)
{
    int result, n1 = 0, n2 = input.obstacles.size() - 1;

    // there is always a solution for n = 0 (empty field)
    // check if there is always a solution also for n = max,
    // if so, then don't need to do anything else
    input.take = n2;
    result = pathfind(input);
    if (result != -1) return -1;

    // do a binary search
    while (n2 - n1 > 1)
    {
        int n3 = (n1 + n2) / 2;

        input.take = n3;
        result = pathfind(input);

        if (result == -1)
            n2 = n3;
        else
            n1 = n3;
    }

    return n1;  // the last coordinates for which there is a solution
}

int main(int argc, char **argv)
{
    Input input = readInput(argc, argv);
    int result = searchForSolution(input);
    if (result > -1)
    {
        std::cout << input.obstacles[result].first << ',' <<
            input.obstacles[result].second << std::endl;
    }
    else
    {
        std::cout << "solution is always possible" << std::endl;
    }
    return 0;
}