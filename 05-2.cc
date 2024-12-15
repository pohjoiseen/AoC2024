#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <sstream>

// global vector of a|b rules
std::vector<std::pair<int, int>> rules;

// comparison function that goes through rules
bool comp(const int n1, const int n2)
{
    for (auto [a, b] : rules)
    {
        if (n1 == a && n2 == b)
        {
            return true;
        }
    }
    return false;
}

int main(int argc, char **argv)
{
    std::string line;

    // read in rules
    for (;;)
    {
        int n1, n2;
        char c;
        std::getline(std::cin, line);
        // empty line means this part is ending
        if (!line.size())
        {
            break;
        }

        std::istringstream istr(line);
        istr >> n1;
        if (istr.fail())
        {
            std::cerr << "Failed to read rule string (first number): " << line << std::endl;
            return 1;
        }

        istr >> c;
        if (!c || c != '|')
        {
            std::cerr << "Failed to read rule string (separator): " << line << std::endl;
            return 1;
        }

        istr >> n2;
        if (istr.fail())
        {
            std::cerr << "Failed to read rule string (second number): " << line << std::endl;
            return 1;
        }

        rules.emplace_back(n1, n2);
    }

    // read in rows
    int result = 0;
    while (!std::cin.eof())
    {
        std::vector<int> v;

        // row
        std::getline(std::cin, line);
        std::istringstream istr(line);
        while (!istr.eof())
        {
            int n;
            char c = 'x';
            istr >> n;
            v.push_back(n);
            istr >> c;
            if (!istr.eof() && c != ',')
            {
                std::cerr << "Failed to parse input line: " << line << std::endl;
                return 1;
            }
        }

        if (v.size() % 2 == 0)
        {
                std::cerr << "Non-odd size of line: " << line << std::endl;
                return 1;
        }

        // go through pairs of numbers in a row
        bool good = true;
        for (auto it = v.begin(); it != v.end() - 1; it++)
        {
            int n1 = *it, n2 = *(it + 1);
            // this is true if n2 should go before n1,
            // that is, order is incorrect
            if (comp(n2, n1))
            {
                good = false;
                break;
            }
        }

        // not interested in rows that were good to begin with
        if (good)
        {
            continue;
        }

        // re-sort row
        std::sort(v.begin(), v.end(), comp);

        // add middle number from the sorted row
        result += v[v.size() / 2];
    }

    std::cout << result << std::endl;

    return 0;
}