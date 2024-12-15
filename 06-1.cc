#include <iostream>
#include <string>
#include <vector>
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

int main(int argc, char **argv)
{
    std::vector<std::string> field;
    int x = -1, y = -1;
    char dir;
    int result = 1;
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
                if (x != -1)
                {
                    std::cerr << "More than one guard position found" << std::endl;
                    return 1;
                }

                y = height;
                x = pos;
                dir = dirChars[i];
                line[pos] = 'X';
            }
        }

        height++;
        field.push_back(line);
    }
    if (x == -1)
    {
        std::cerr << "Field read in but no guard position found" << std::endl;
        return 1;
    }

    // do the walk
    for (;;)
    {
        auto [x1, y1] = move(x, y, dir);
        if (x1 < 0 || x1 >= width || y1 < 0 || y1 >= height)
        {
            break;
        }

        if (field[y1][x1] == '#')
        {
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

    for (auto line : field)
    {
        //std::cerr << line << std::endl;
    }
    std::cout << result << std::endl;

    return 0;
}