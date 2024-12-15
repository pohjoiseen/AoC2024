#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <sstream>

int main(int argc, char **argv)
{
    std::vector<std::pair<int, int>> rules;
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

        // go through numbers in a row
        bool good = true;
        for (auto it = v.begin(); it != v.end(); it++)
        {
            // go through rules
            for (auto [a, b] : rules)
            {
                // a|b, number == a -> b must not be found BEFORE it
                if (*it == a && std::find(v.begin(), it, b) != it)
                {
                    //std::cerr << "in " << line << " number " << *it << " fails " << a << '|' << b << std::endl;
                    good = false;
                    break;
                }
                // a|b, number == b -> a must not be found AFTER it
                else if (*it == b && std::find(it, v.end(), a) != v.end())
                {
                    //std::cerr << "in " << line << " number " << *it << " fails " << a << '|' << b << std::endl;
                    good = false;
                    break;
                }
            }
            if (!good)
            {
                break;
            }
        }

        // add middle number from the row if all good
        if (good)
        {
            //std::cerr << "in " << line << " all good, result += " << v[v.size() / 2] << std::endl;
            result += v[v.size() / 2];
        }
    }

    std::cout << result << std::endl;

    return 0;
}