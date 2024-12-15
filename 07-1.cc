#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <numeric>

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

        // now, there are only two kinds of signs,
        // so a particular combination of them can be represented
        // by a binary number of (signsLength) bits.
        // We can then look at every number, and assume
        // set bits mean multiplications and unset bits mean additions
        int n = 2 << signsLength;

        for (int i = 0; i < n; i++)
        {
            // go through numbers array, multiplying/adding
            // depending on bit value.  Go always left to right
            // as the task requires
            long long total1 = numbers[0];
            for (int k = 0; k < signsLength; k++)
            {
                if (i & (1 << k))
                {
                    total1 *= numbers[k + 1];
                }
                else
                {
                    total1 += numbers[k + 1];
                }
            }

            // and then it's supposed to match the total
            if (total1 == total)
            {
                result += total;
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