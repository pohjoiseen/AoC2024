#include <iostream>
#include <string>
#include <vector>

struct Input
{
    std::vector<std::string> field;
    std::string moves;
    int x = -1, y = -1;
};

Input readInput()
{
    Input input;
    std::string line;
    for (int i = 0;; i++)
    {
        std::getline(std::cin, line);
        if (!line.size()) break;

        auto startpos = line.find('@');
        if (startpos != std::string::npos)
        {
            line[startpos] = '.';
            if (input.x != -1)
            {
                std::cerr << "More than one starting position!" << std::endl;
                std::exit(1);
            }
            input.x = startpos;
            input.y = i;
        }

        input.field.push_back(line);
    }

    if (input.x == -1)
    {
        std::cerr << "No starting position!" << std::endl;
        std::exit(1);
    }

    while (!std::cin.eof())
    {
        std::getline(std::cin, line);
        input.moves.append(line);
    }

    return input;
}

void move(int &x, int &y, char m)
{
    switch (m)
    {
        case '<': x--; break;
        case '^': y--; break;
        case '>': x++; break;
        case 'v': y++; break;
        default:
            std::cerr << "Unknown move: " << m << std::endl;
            std::exit(1);
    }
}

void visualize(const Input &input)
{
    for (int i = 0; i < input.field.size(); i++)
    {
        std::string s = input.field[i];
        if (i == input.y)
        {
            s[input.x] = '@';
        }
        std::cout << s << std::endl;
    }
    std::cout << std::endl;
}

void walk(Input &input)
{
    int width = input.field[0].size(), height = input.field.size();

    for (char m : input.moves)
    {
        //visualize(input);

        // get new coordinates and verify them
        int x = input.x, y = input.y;
        move(x, y, m);
        if (x < 0 || y < 0 || x >= width || y >= height)
        {
            std::cerr << "Left the field!" << std::endl;
            std::exit(1);
        }

        // hit wall, don't do anything
        if (input.field[y][x] == '#') continue;

        // hit empty space, move
        if (input.field[y][x] == '.')
        {
            input.x = x;
            input.y = y;
            continue;
        }

        // hit box, try to move a row of boxes (possibly just one box)
        if (input.field[y][x] == 'O')
        {
            // skip over a row
            int x1 = x, y1 = y;
            while (input.field[y1][x1] == 'O')
            {
                move(x1, y1, m);
                if (x1 < 0 || y1 < 0 || x1 >= width || y1 >= height)
                {
                    std::cerr << "Pushed boxes outside the field!" << std::endl;
                    std::exit(1);
                }
            }

            if (input.field[y1][x1] == '.')
            {
                // move the box
                input.field[y1][x1] = 'O';
                input.field[y][x] = '.';

                // move ourselves into position of the box
                input.x = x;
                input.y = y;
                continue;
            }

            if (input.field[y1][x1] == '#')
            {
                // pushed against wall, do nothing
                continue;
            }

            std::cerr << "Pushed box(es) against unknown block: " << input.field[y1][x1] << std::endl;
            std::exit(-1);
        }

        std::cerr << "Moved into unknown block: " << input.field[y][x] << std::endl;
        std::exit(-1);
    }

    //visualize(input);
}

int summarize(const Input &input)
{
    int result = 0;
    for (int i = 0; i < input.field.size(); i++)
    {
        for (int j = 0; j < input.field[j].size(); j++)
        {
            if (input.field[i][j] == 'O')
            {
                result += 100 * i + j;
            }
        }
    }
    return result;
}

int main(int argc, char **argv)
{
    Input input = readInput();
    walk(input);
    std::cout << summarize(input) << std::endl;
    return 0;
}