// straightforward version

#include <iostream>
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

void process(Input &input, int nsteps)
{
    //for (auto i : input) std::cout << i << ' ';
    //std::cout << std::endl;

    for (int i = 0; i < nsteps; i++)
    {
        for (auto it = input.begin(); it != input.end(); it++)
        {
            if (*it == 0)
            {
                *it = 1;
                continue;
            }
            
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

int main(int argc, char **argv)
{
    int nsteps = 25;
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
    process(input, nsteps);
    std::cout << input.size() << std::endl;
    return 0;
}