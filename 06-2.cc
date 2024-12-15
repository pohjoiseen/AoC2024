#include <iostream>
#include <string>
#include <vector>
#include <tuple>
#include <cstring>

std::pair<int, int> move(int x, int y, char dir)
{
    switch (dir)
    {
        case '^':
            y -= 1;
            break;

        case '<':
            x -= 1;
            break;

        case '>':
            x += 1;
            break;

        case 'v':
            y += 1;
            break;
    }
    return std::pair<int, int>(x, y);
}

int walk(std::vector<std::string> &field, int x, int y, char dir)
{
    int result = 1;
    const char *dirChars = "<^>v";
    std::vector<std::tuple<int, int, char>> turns;
    int height = field.size(), width = field[0].size();

    for (;;)
    {
        auto [x1, y1] = move(x, y, dir);
        if (x1 < 0 || x1 >= width || y1 < 0 || y1 >= height)
        {
            break;
        }

        if (field[y1][x1] == '#')
        {
            for (auto turn : turns)
            {
                if (std::get<0>(turn) == x &&
                    std::get<1>(turn) == y &&
                    std::get<2>(turn) == dir)
                {
                    // loop detected
                    return 0;
                }
            }
            turns.emplace_back(x, y, dir);

            const char *next = std::strchr(dirChars, dir) + 1;
            if (!*next)
            {
                next = dirChars;
            }
            dir = *next;
            continue;
        }

        if (field[y1][x1] == '.')
        {
            field[y1][x1] = 'X';
            result++;
        }

        x = x1;
        y = y1;
    }

    return result;
}

int main(int argc, char **argv)
{
    std::vector<std::string> field;
    int xstart = -1, ystart = -1;
    char dirstart;
    const char *dirChars = "<^>v";

    // read in field
    int width, height = 0;
    while (!std::cin.eof())
    {
        std::string line;
        std::getline(std::cin, line);
        width = line.size();
        
        for (int i = 0; i < 4; i++)
        {
            size_t pos = line.find(dirChars[i]);
            if (pos != std::string::npos)
            {
                if (xstart != -1)
                {
                    std::cerr << "More than one guard position found" << std::endl;
                    return 1;
                }

                ystart = height;
                xstart = pos;
                dirstart = dirChars[i];
                line[pos] = 'X';
            }
        }

        height++;
        field.push_back(line);
    }
    if (xstart == -1)
    {
        std::cerr << "Field read in but no guard position found" << std::endl;
        return 1;
    }

    // perform initial walk which will mark visited squares with X
    walk(field, xstart, ystart, dirstart);

    // make a copy of the field for the rest of the checks
    std::vector<std::string> field1 = field;

    // loop through the field, examining visited squares only
    int result = 0;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // original starting coordinates cannot be used
            if (i == ystart && j == xstart)
            {
                continue;
            }

            // only visited squares are interesting
            if (field[i][j] != 'X')
            {
                continue;
            }

            // try to replace position with an obstacle
            field1[i][j] = '#';

            // test field and note if loop detected
            // we just interested in loop or no loop, not exact
            // result, so don't care about field1 getting more
            // and more clobbered with X's
            if (!walk(field1, xstart, ystart, dirstart))
            {
                result++;
            }

            // restore to empty space
            field1[i][j] = 'X';
        }
    }

    std::cout << result << std::endl;

    return 0;
}