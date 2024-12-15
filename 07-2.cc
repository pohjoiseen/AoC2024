#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <numeric>
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

long long calculate(Input input)
{
    long long result = 0;

    for (auto [total, numbers] : input)
    {
        // total number of signs between numbers
        int signsLength = numbers.size() - 1;
        // special case 0 and 1 numbers (though doesn't occur in real input)
        if (signsLength < 0)
        {
            continue;
        }
        if (signsLength == 0)
        {
            result += total;
            continue;
        }

        // there are now three kinds of signs but we
        // still are going to try to represent them with bits,
        // now using two bits for one sign (00 = +, 01 = *, 10 = ||,
        // 11 = ignored), so loop through numbers
        // of (signsLength * 2) bits.
        // int (32 bits) should be enough for original input
        int n = 2 << (signsLength * 2);

        bool found = false;
        for (int i = 0; i < n; i++)
        {
            // go through numbers array, multiplying/adding
            // depending on bit value.  Go always left to right
            // as the task requires
            long long total1 = numbers[0];
            bool skip = false;
            for (int k = 0; k < signsLength; k++)
            {
                char sign = 
                    ((i & (1 << (k * 2))) ? 1 : 0) +
                    ((i & (1 << (k * 2 + 1))) ? 2 : 0);
                switch (sign)
                {
                    case 0:
                        total1 *= numbers[k + 1];
                        break;

                    case 1:
                        total1 += numbers[k + 1];
                        break;

                    case 2:
                    {
                        int numdigits = 1 + std::floor(std::log10f(numbers[k + 1]));
                        total1 = total1 * std::pow(10, numdigits) + numbers[k + 1];
                    }
                    break;

                    default:
                        // 3 value is meaningless and skip all combinations with it
                        skip = true;
                }

                if (skip || total1 > total)
                {
                    break;
                }
            }
            if (skip)
            {
                continue;
            }

            // and then it's supposed to match the total
            if (total1 == total)
            {
                result += total;
                found = true;
                break;  // enough if just one option is found
            }
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