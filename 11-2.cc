// optimized version, as 11-1 would not work for input of default size (75),
// as it would require a list hundreds of trillions long.  Use a precalculated
// lookup table

#include <iostream>
#include <vector>
#include <list>
#include <cmath>

using Input = std::list<long long>;

Input readInput()
{
    Input input;
    long long n;
    while (!std::cin.eof())
    {
        std::cin >> n;
        if (!std::cin.fail())
        {
            input.push_back(n);
        }
    }
    return input;
}

std::vector<std::vector<long long>> table;

// build up a lookup table for all digits (0...9) for all possible numbers of iterations up to the max
// all )or almost all?) longer numbers eventually will fall apart to individual digits
void precalculateTable(int steps)
{
    // some seed data, 0 = need to calculate (except for row 0)
    table.push_back({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 });  // 0, unused
    table.push_back({ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 });  // 1
    table.push_back({ 1, 2, 2, 2, 2, 1, 1, 1, 1, 1 });  // 2
    table.push_back({ 2, 4, 4, 4, 4, 2, 2, 2, 2, 2 });  // 3
    table.push_back({ 4, 0, 0, 0, 0, 4, 4, 4, 4, 4 });  // 4
    table.push_back({ 0, 0, 0, 0, 0, 8, 8, 8, 7, 8 });  // 5
    table.push_back({ 0, 0, 0, 0, 0, 0, 0, 0, 7, 0 });  // 6

    // iterate onwards, using previous rows to calculate next ones
    for (int i = 1; i <= steps; i++)
    {
        if (table.size() >= i)
        {
            table.push_back({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 });
        }

        if (!table[i][0])
        {
            table[i][0] =
                table[i - 4][2] +
                table[i - 4][0] +
                table[i - 4][2] +
                table[i - 4][4];
        }

        if (!table[i][1])
        {
            table[i][1] =
                table[i - 3][2] +
                table[i - 3][0] +
                table[i - 3][2] +
                table[i - 3][4];
        }

        if (!table[i][2])
        {
            table[i][2] =
                table[i - 3][4] +
                table[i - 3][0] +
                table[i - 3][4] +
                table[i - 3][8];
        }

        if (!table[i][3])
        {
            table[i][3] =
                table[i - 3][6] +
                table[i - 3][0] +
                table[i - 3][7] +
                table[i - 3][2];
        }

        if (!table[i][4])
        {
            table[i][4] =
                table[i - 3][8] +
                table[i - 3][0] +
                table[i - 3][9] +
                table[i - 3][6];
        }

        if (!table[i][5])
        {
            table[i][5] =
                table[i - 5][2] +
                table[i - 5][0] +
                table[i - 5][4] +
                table[i - 5][8] +
                table[i - 5][2] +
                table[i - 5][8] +
                table[i - 5][8] +
                table[i - 5][0];
        }

        if (!table[i][6])
        {
            table[i][6] =
                table[i - 5][2] +
                table[i - 5][4] +
                table[i - 5][5] +
                table[i - 5][7] +
                table[i - 5][9] +
                table[i - 5][4] +
                table[i - 5][5] +
                table[i - 5][6];
        }

        if (!table[i][7])
        {
            table[i][7] =
                table[i - 5][2] +
                table[i - 5][8] +
                table[i - 5][6] +
                table[i - 5][7] +
                table[i - 5][6] +
                table[i - 5][0] +
                table[i - 5][3] +
                table[i - 5][2];
        }

        if (!table[i][8])
        {
            table[i][8] =
                table[i - 5][3] +
                table[i - 5][2] +
                table[i - 5][7] +
                table[i - 5][7] +
                table[i - 5][2] +
                table[i - 5][6] +
                table[i - 4][8];  // <-- not a mistake, 8 is awkward in this way
        }

        if (!table[i][9])
        {
            table[i][9] =
                table[i - 5][3] +
                table[i - 5][6] +
                table[i - 5][8] +
                table[i - 5][6] +
                table[i - 5][9] +
                table[i - 5][1] +
                table[i - 5][8] +
                table[i - 5][4];
        }

    }
}

void process(Input &input, int nsteps)
{
    //for (auto i : input) std::cout << i << ' ';
    //std::cout << std::endl;

    long long result = 0;
    for (int i = 0; i < nsteps; i++)
    {
        for (auto it = input.begin(); it != input.end(); it++)
        {
            // already precalculated
            if (*it < 0)
            {
                continue;
            }

            // use lookup table
            if (*it < 10)
            {
                *it = -table[nsteps - i][*it];
                continue;
            }
            
            // process numbers with at least two digits according to rules
            // possibly splitting in two (which may create more numbers <10 which can be looked up then)
            int numdigits = 1 + std::floor(std::log10f(*it));
            if (numdigits % 2 == 0)
            {
                long long d = std::pow(10, numdigits / 2);
                long long a = *it / d, b = *it % d;
                input.insert(it, a);
                *it = b;
            }
            else
            {
                *it *= 2024;
            }
        }

        //for (auto i : input) std::cout << i << ' ';
        //std::cout << std::endl;
    }
}

long long summarize(const Input &input)
{
    long long result = 0;
    for (auto n : input)
    {
        if (n < 0)
            result += -n;  // precalculated value
        else
            result++;
    }
    return result;
}

int main(int argc, char **argv)
{
    int nsteps = 75;
    if (argc >= 2)
    {
        nsteps = std::atoi(argv[1]);
        if (nsteps <= 0)
        {
            std::cerr << "need valid number of iterations greater than zero!" << std::endl;
            return 1;
        }
    }

    Input input = readInput();
    precalculateTable(nsteps);
    process(input, nsteps);
    std::cout << summarize(input) << std::endl;
    return 0;
}