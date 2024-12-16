#include <iostream>
#include <vector>
#include <queue>
#include <deque>
#include <map>
#include <set>

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
};

struct IntermediateResult
{
    std::map<std::tuple<int, int, Dir>, int> pathsCost;
    int minCostToFinish = 1000000000;
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

void visualize(const Input &input)
{
    for (auto s : input.map) std::cout << s << std::endl;
}

// find shortest paths from S (input.xs/ys) to all ponts of the map, according to the rules,
// using Dijkstra algorithm.  Return map of (coords, direction) -> minimal cost to get to position
IntermediateResult pathfind(const Input &input)
{
    IntermediateResult result;

    // function to order search steps for priority queue
    // sign must be inverted, as the queue picks the top item, but it must be one with
    // the lowest cost
    auto searchStepCmp = [](SearchStep &a, SearchStep &b) {
        return a.cost > b.cost;
    };

    // priority queue for Dijkstra
    std::priority_queue<SearchStep, std::vector<SearchStep>, decltype(searchStepCmp)> searchQueue(searchStepCmp);

    // allowed moves and their costs (initial dir, x/y offset, resulting dir, cost)
    // do not handle turn and move at once, unlike 16-1
    const std::tuple<Dir, int, int, Dir, int> moves[] = {
        { R, 1, 0, R, 1 },
        { R, 0, 0, U, 1000 },
        { R, 0, 0, D, 1000 },
        { L, -1, 0, L, 1 },
        { L, 0, 0, U, 1000 },
        { L, 0, 0, D, 1000 },
        { D, 0, 1, D, 1 },
        { D, 0, 0, L, 1000 },
        { D, 0, 0, R, 1000 },
        { U, 0, -1, U, 1 },
        { U, 0, 0, L, 1000 },
        { U, 0, 0, R, 1000 },
    };

    int width = input.map[0].size(), height = input.map.size();

    // start with initial square
    searchQueue.emplace(SearchStep { input.xs, input.ys, Dir::R, 0 });

    // loop until examined all free squares, do not stop at destination
    while (!searchQueue.empty())
    {
        // get the square with the lowest cost
        // copy here, not reference, as searchQueue.emplace() would invalidate the reference
        const SearchStep cur = searchQueue.top();
        searchQueue.pop();

        // examine possible moves from the current square
        for (const auto &[dir, dx, dy, dirNext, cost] : moves)
        {
            int x = cur.x + dx, y = cur.y + dy;
            
            // direction for the move must match our current, move must not
            // lead beyond map limits or into a wall
            if (dir != cur.dir) continue;
            if (x < 0 || y < 0 || x >= width || y >= height) continue;
            if (input.map[y][x] == '#') continue;

            // calculate cost for the candidate square
            int newCost = cur.cost + cost;

            // if we already know cost to the finish and this is longer than that,
            // can skip
            if (newCost > result.minCostToFinish) continue;

            // if we reached the finish, note the distance
            if (x == input.xe && y == input.ye && newCost < result.minCostToFinish)
            {
                result.minCostToFinish = newCost;
            }

            // check that this square was not previously already found with lower cost
            auto coords = std::make_tuple(x, y, dirNext);
            if (result.pathsCost.find(coords) == result.pathsCost.end() || 
                newCost < result.pathsCost[coords])
            {
                // enqueue
                result.pathsCost[coords] = newCost;
                searchQueue.emplace(SearchStep { x, y, dirNext, newCost });
            }
        }
    }

    return result;
}

// count all squares along all possible shortest paths to the destination
// (so paths may be different but must have same, shortest length).
// Accepts pathCost map precalculated by pathfind().  Changes map in input.
// This is more complicated than it would seem to be because we need to track
// also direction, not just coordinates
int countSquaresAlongShortestPaths(Input &input, IntermediateResult intermediateResult)
{
    auto [pathsCost, minCostToFinish] = intermediateResult;
    int width = input.map[0].size(), height = input.map.size();

    // start from the end, but the end square may be reached in any possible orientaion,
    // so enqueue all final positions which have the minimal cost
    std::deque<std::tuple<int, int, Dir>> deque;
    for (auto dir : { L, U, R, D })
    {
        if (pathsCost[std::make_tuple(input.xe, input.ye, dir)] == minCostToFinish)
        {
            deque.emplace_back(input.xe, input.ye, dir);
        }
    }

    // flood-fill from the ending points, backtracking only on valid paths
    std::set<std::tuple<int, int, Dir>> seen;
    int result = 0;
    while (deque.size())
    {
        auto coords = deque.front();
        auto [x, y, dir] = coords;
        deque.pop_front();

        // do not reexamine already seen positions, this would massively
        // blow up running time
        if (seen.find(coords) != seen.end()) continue;
        seen.insert(coords);

        // note visited position, only once regardless of orientation
        if (input.map[y][x] != 'O')
        {
            input.map[y][x] = 'O';
            result++;
        }

        // enqueue all valid and previously found positions that can lead to this one
        int cost = pathsCost[coords];
        if (x > 0 && pathsCost[std::make_tuple(x - 1, y, R)] == cost - 1)
            deque.emplace_back(x - 1, y, R);
        if (y > 0 && pathsCost[std::make_tuple(x, y - 1, D)] == cost - 1)
            deque.emplace_back(x, y - 1, D);
        if (x < width - 1 && pathsCost[std::make_tuple(x + 1, y, L)] == cost - 1)
            deque.emplace_back(x + 1, y, L);
        if (y < height - 1 && pathsCost[std::make_tuple(x, y + 1, U)] == cost - 1)
            deque.emplace_back(x, y + 1, U);
        if (pathsCost[std::make_tuple(x, y, L)] == cost - 1000)
            deque.emplace_back(x, y, L);
        if (pathsCost[std::make_tuple(x, y, R)] == cost - 1000)
            deque.emplace_back(x, y, R);
        if (pathsCost[std::make_tuple(x, y, U)] == cost - 1000)
            deque.emplace_back(x, y, U);
        if (pathsCost[std::make_tuple(x, y, D)] == cost - 1000)
            deque.emplace_back(x, y, D);
    }

    // ensure the beginning square is covered
    if (input.map[input.ys][input.xs] != 'O')
    {
        input.map[input.ys][input.xs] = 'O';
        result++;
    }

    //visualize(input);
    return result;
}

int main(int argc, char **argv)
{
    Input input = readInput();
    auto pathCost = pathfind(input);
    int result = countSquaresAlongShortestPaths(input, pathCost);
    std::cout << result << std::endl;
    return 0;
}