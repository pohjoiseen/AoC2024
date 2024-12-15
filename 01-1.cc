#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

int main(int argc, char **argv)
{
    std::vector<int> v1, v2;
    while (!std::cin.eof()) {
        int n1, n2;
        std::cin >> n1 >> n2;
        v1.push_back(n1);
        v2.push_back(n2);
    }

    std::sort(v1.begin(), v1.end());
    std::sort(v2.begin(), v2.end());

    int sum = 0;
    for (int i = 0; i < v1.size(); i++) {
        sum += std::abs(v1[i] - v2[i]);
    }

    std::cout << sum << std::endl;

    return 0;
}