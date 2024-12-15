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

        auto &mapline = input.field.emplace_back();
        for (int j = 0; j < line.size(); j++)
        {
            switch (line[j])
            {
                case '.': mapline += ".."; break;
                case '#': mapline += "##"; break;
                case 'O': mapline += "[]"; break;
                case '@':
                {
                    if (input.x != -1)
                    {
                        std::cerr << "More than one starting position!" << std::endl;
                        std::exit(1);
                    }

                    mapline += "..";
                    input.x = j * 2;
                    input.y = i;
                    break;
                }
                default:
                {
                    std::cerr << "Unknown character found: " << line[j] << std::endl;
                    std::exit(1);
                }
            }
        }
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

// horizontal move is easy enough, only need to examine one line
bool tryPushHorizontal(std::string &line, int x0, bool toRight)
{
    // where to move?
    int sign = toRight ? 1 : -1;

    // skip over a row of boxes
    int x = x0 + sign;
    while (line[x] == '[' || line[x] == ']') x += sign;

    // if pushed against wall, cannot move
    if (line[x] == '#') return false;

    // otherwise must be an empty space
    if (line[x] != '.') std::abort();

    // shift a row of characters with boxes
    for (int i = x; i != x0; i -= sign) line[i] = line[i - sign];

    return true;
}

// vertical move can move a big number of boxes interlocked with each other in
// various ways, so needs to be considerably more involved
bool tryPushVertical(std::vector<std::string> &field, int x0, int y0, bool toDown)
{
    int width = field[0].size(), height = field.size();

    // where to move?
    int sign = toDown ? 1 : -1;

    // coordinates of left halves of all boxes to be moved
    std::vector<std::pair<int, int>> boxesToMove;

    // put the box immediately next to the "robot" there
    int x1 = x0;
    int y1 = y0 + sign;
    if (field[y1][x1] == ']') x1--;  // coordinates are always of the left half of the box
    boxesToMove.emplace_back(x1, y1);

    // loop through boxes until found all
    for (int i = 0; i < boxesToMove.size(); i++)
    {
        // coordinates of space to which the current box needs to be moved
        auto [x, y] = boxesToMove[i];
        y += sign;
        if (x < 0 || y < 0 || x >= width || y >= height)
        {
            std::cerr << "Pushed boxes outside the field!" << std::endl;
            std::exit(1);
        }

        // if hit a wall at any point, that's an immediate bailout
        if (field[y][x] == '#' || field[y][x + 1] == '#') return false;

        // check for a box halfway to the left
        if (field[y][x] == ']') boxesToMove.emplace_back(x - 1, y);
        // check for a box aligned with this one
        if (field[y][x] == '[') boxesToMove.emplace_back(x, y);
        // check for a box halfway to the right
        if (field[y][x + 1] == '[') boxesToMove.emplace_back(x + 1, y);
    }

    // actually move boxes on the map, starting from the end ensures correct order
    for (auto it = boxesToMove.end() - 1; it >= boxesToMove.begin(); it--)
    {
        auto [x, y] = *it;
        field[y + sign][x] = '[';
        field[y + sign][x + 1] = ']';
        field[y][x] = '.';
        field[y][x + 1] = '.';
    }

    return true;
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

        // hit box, try to move one or more boxes
        if (input.field[y][x] == '[' || input.field[y][x] == ']')
        {
            // different functions for horizontal and vertical move
            bool pushed;
            switch (m)
            {
                case '<': pushed = tryPushHorizontal(input.field[y], input.x, false); break;
                case '^': pushed = tryPushVertical(input.field, input.x, input.y, false); break;
                case '>': pushed = tryPushHorizontal(input.field[y], input.x, true); break;
                case 'v': pushed = tryPushVertical(input.field, input.x, input.y, true); break;
                default: std::abort();
            }

            // move ourselves if succeeded to push any boxes
            if (pushed)
            {
                input.x = x;
                input.y = y;
            }

            continue;
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
        for (int j = 0; j < input.field[i].size(); j++)
        {
            if (input.field[i][j] == '[') result += 100 * i + j;
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