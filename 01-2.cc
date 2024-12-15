#include <iostream>
#include <vector>
#include <unordered_map>

int main(int argc, char **argv)
{
    std::vector<int> v1;
    std::unordered_map<int, int> m2;
    while (!std::cin.eof()) {
        int n1, n2;
        std::cin >> n1 >> n2;
        v1.push_back(n1);
        m2[n2]++;
    }

    int sum = 0;
    for (int k : v1) {
        sum += k * m2[k];
    }

    std::cout << sum << std::endl;

    return 0;
}