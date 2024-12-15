#include <iostream>
#include <string>
#include <sstream>
#include <cmath>

int main(int argc, char **argv)
{
    std::string line;
    int count = 0;

    while (std::getline(std::cin, line))
    {
        std::stringstream linestr(line);
        int last = -1, dir = 0, n;
        bool good = true;
        while (!linestr.eof())
        {
            linestr >> n;
            if (last != -1)
            {
                int diff = std::abs(n - last);
                if (diff < 1 || diff > 3)
                {
                    good = false;
                    break;
                }
            }
            if (last != -1 && dir == 0)
            {
                dir = n > last ? 1 : -1;
            }
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
        if (good)
        {
            count++;
        }
    }

    std::cout << count << std::endl;

    return 0;
}