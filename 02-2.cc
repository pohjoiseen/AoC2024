#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <cmath>

bool is_good(std::vector<int> &v)
{
    int last = -1, dir = 0;
    bool good = true;

    for (int n : v)
    {
        // removed number, do not count
        if (n == -1)
        {
            continue;
        }

        // check that the different with previous number
        // (if this is not the first in the row) is within 1..3
        if (last != -1)
        {
            int diff = std::abs(n - last);
            if (diff < 1 || diff > 3)
            {
                good = false;
                break;
            }
        }
        // determine direction of sequence in the beginning
        if (last != -1 && dir == 0)
        {
            dir = n > last ? 1 : -1;
        }
        // check that the current number matches previously
        // determined direction
        else
        {
            if ((dir > 0 && n < last) || (dir < 0 && n > last))
            {
                good = false;
                break;
            }
        }
        last = n;
    }

    return good;
}

int main(int argc, char **argv)
{
    std::string line;
    int count = 0;

    // read by lines
    while (std::getline(std::cin, line))
    {
        // read line into a vector
        std::stringstream linestr(line);
        std::vector<int> v;
        while (!linestr.eof())
        {
            int n;
            linestr >> n;
            v.push_back(n);
        }
        
        // check if good by itself
        if (is_good(v))
        {
            count++;
        }
        // check if good with skipping one number
        else
        {
            for (int i = 0; i < v.size(); i++)
            {
                int removed = v[i];
                v[i] = -1;
                if (is_good(v))
                {
                    count++;
                    break;
                }
                v[i] = removed;
            }
        }
    }

    std::cout << count << std::endl;

    return 0;
}