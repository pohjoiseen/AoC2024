// recursive version
// massively faster than previous ones
// runs in ~4 ms vs 1130-1300 ms in 07-2-par.cc
// (no benefit from further parallelization)

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>

typedef std::vector<std::pair<long long, std::vector<int>>> Input;

Input read(std::istream &istr)
{
    Input result;

    while (!istr.eof())
    {
        std::string line;
        std::getline(istr, line);
        std::stringstream sstr(line);

        long long total;
        sstr >> total;
        if (sstr.fail())
        {
            std::cerr << "Failed to read total in line: " << line << std::endl;
            std::exit(1);
        }

        char c;
        sstr >> c;
        if (c != ':')
        {
            std::cerr << "Missling colon in line: " << line << std::endl;
            std::exit(1);
        }

        std::vector<int> numbers;
        while (!sstr.eof())
        {
            int n;
            sstr >> n;
            if (sstr.fail())
            {
                std::cerr << "Failed to read one of numbers in line: " << line << std::endl;
                std::exit(1);
            }

            numbers.push_back(n);
        }

        result.emplace_back(total, numbers);
    }

    return result;
}

bool check(std::vector<int> &numbers, long long total, int k)
{
    if (k == 0)
    {
        return total == numbers[0];
    }

    if (total < numbers[k])
    {
        return false;
    }

    if (check(numbers, total - numbers[k], k - 1))
    {
        return true;
    }

    if (total % numbers[k] == 0 &&
        check(numbers, total / numbers[k], k - 1))
    {
        return true;
    }

    int ndigits = 1 + std::floor(std::log10(numbers[k]));
    int d = std::pow(10, ndigits);
    if ((total - numbers[k]) % d == 0)
    {
        if (check(numbers, (total - numbers[k]) / d, k - 1))
        {
            return true;
        }
    }

    return false;
}

long long calculate(Input input)
{
    long long result = 0;
    for (auto [total, numbers] : input)
    {
        if (check(numbers, total, numbers.size() - 1))
        {
            result += total;
        }
    }

    return result;
}

int main(int argc, char **argv)
{
    Input input = read(std::cin);
    long long result = calculate(input);
    std::cout << result << std::endl;
    return 0;
}