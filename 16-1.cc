#include <iostream>
#include <vector>
#include <queue>
#include <map>

struct Input
{
    std::vector<std::string> map;
    int xs = -1, ys, xe = -1, ye;
};

enum Dir { U = 'U', D = 'D', L = 'L', R = 'R' };

struct SearchStep
{
    int x, y;
    Dir dir;
    int cost;
    int distEstimate;
};

Input readInput()
{
    Input input;
    int i = 0;
    while (!std::cin.eof())
    {
        std::string &line = input.map.emplace_back();
        std::getline(std::cin, line);
        size_t pos;
        if ((pos = line.find('S')) != std::string::npos)
        {
            if (input.xs != -1)
            {
                std::cerr << "More than one start position!" << std::endl;
                std::exit(1);
            }
            input.xs = pos;
            input.ys = i;
        }
        if ((pos = line.find('E')) != std::string::npos)
        {
            if (input.xe != -1)
            {
                std::cerr << "More than one end position!" << std::endl;
                std::exit(1);
            }
            input.xe = pos;
            input.ye = i;
        }
        i++;
    }
    if (input.xs == -1 || input.xe == -1)
    {
        std::cerr << "Start and/or end position missing!" << std::endl;
        std::exit(1);
    }
    return input;
}

void visualize(const Input &input, std::map<std::pair<int, int>, std::pair<int, int>> &pathMap, int x, int y)
{
    std::vector<std::string> map = input.map;
    while (x != input.xs || y != input.ys)
    {
        map[y][x] = '+';
        auto [x1, y1] = pathMap[std::make_pair(x, y)];
        x = x1;
        y = y1;
    }
    for (auto s : map) std::cout << s << std::endl;
}

// find route from S (input.xs/ys) to E (input.xe/ye) according to the rules
// using A* algorithm
int pathfind(const Input &input)
{
    // function to order search steps for priority queue
    // sign must be inverted, as the queue picks the top item, but it must be one with
    // the lowest cost
    auto searchStepCmp = [](SearchStep &a, SearchStep &b) {
        return (a.cost + a.distEstimate) > (b.cost + b.distEstimate);
    };
    // function to estimate distance to the end, by just taking horizontal+vertical difference
    auto calcDistEstimate = [](int x, int y, const Input &input) {
        return std::abs(input.xe - x) + std::abs(input.ye - y);
    };

    // priority queue for A* 
    std::priority_queue<SearchStep, std::vector<SearchStep>, decltype(searchStepCmp)> searchQueue(searchStepCmp);
    // minimal cost for found nodes
    std::map<std::pair<int, int>, int> minCost;
    // for tracking the path (only for visualization), node -> previous node;
    std::map<std::pair<int, int>, std::pair<int, int>> pathMap;

    // allowed moves and their costs (initial dir, x/y offset, resulting dir, cost)
    const std::tuple<Dir, int, int, Dir, int> moves[] = {
        { R, 1, 0, R, 1 },
        { U, 1, 0, R, 1001 },
        { D, 1, 0, R, 1001 },
        { L, -1, 0, L, 1 },
        { U, -1, 0, L, 1001 },
        { D, -1, 0, L, 1001 },
        { D, 0, 1, D, 1 },
        { L, 0, 1, D, 1001 },
        { R, 0, 1, D, 1001 },
        { U, 0, -1, U, 1 },
        { L, 0, -1, U, 1001 },
        { R, 0, -1, U, 1001 },
    };

    int width = input.map[0].size(), height = input.map.size();

    // start with initial square
    searchQueue.emplace(SearchStep { input.xs, input.ys, Dir::R, 0, calcDistEstimate(input.xs, input.ys, input) });

    while (!searchQueue.empty())
    {
        // get the square with the lowest cost + remaining distance estimate
        // copy here, not reference, as searchQueue.emplace() would invalidate the reference
        const SearchStep cur = searchQueue.top();

        // examine possible moves from the current square
        for (const auto &[dir, dx, dy, dirNext, cost] : moves)
        {
            int x = cur.x + dx, y = cur.y + dy;
            
            // direction for the move must match our current, move must not
            // lead beyond map limits or into a wall
            if (dir != cur.dir) continue;
            if (x < 0 || y < 0 || x >= width || y >= height) continue;
            if (input.map[y][x] == '#') continue;

            // calculate cost for the candidate square and estimate until the end
            int newCost = cur.cost + cost;
            int distEstimate = calcDistEstimate(x, y, input);

            // it's actually the destination!
            if (!distEstimate)
            {
                //visualize(input, pathMap, cur.x, cur.y);
                return newCost;
            }

            // check that this square was not previously already found with lower cost
            auto coords = std::make_pair(x, y);
            if (minCost.find(coords) == minCost.end() || newCost < minCost[coords])
            {
                // enqueue
                minCost[coords] = newCost;
                pathMap[coords] = std::make_pair(cur.x, cur.y);
                searchQueue.emplace(SearchStep { x, y, dirNext, newCost, distEstimate });
            }
        }

        searchQueue.pop();
    }

    // queue exhausted and no possible path found
    return -1;
}

int main(int argc, char **argv)
{
    Input input = readInput();
    int result = pathfind(input);
    std::cout << result << std::endl;
    return 0;
}