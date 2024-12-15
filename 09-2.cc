#include <iostream>
#include <vector>

struct Extent
{
    int id;  // -1 = empty
    int pos;
    int len;
};
struct Input
{
    std::vector<Extent> files;
    std::vector<Extent> free;
};

Input readInput()
{
    Input input;
    int id = 0, pos = 0;
    while (!std::cin.eof())
    {
        char blocks, empty;
        std::cin >> blocks >> empty;
        if (blocks > '0')
        {
            input.files.emplace_back(Extent { id, pos, blocks - '0' });
        }
        pos += blocks - '0';
        if (empty > '0')
        {
            input.free.emplace_back(Extent { -1, pos, empty - '0' });
        }
        pos += empty - '0';
        id++;
    }
    return input;
}

void process(Input &input)
{
    for (auto it = input.files.end() - 1; it >= input.files.begin(); it--)
    {
        for (auto itFree = input.free.begin();
             itFree != input.free.end() && itFree->pos < it->pos;
             itFree++)
        {
            if (itFree->len >= it->len)
            {
                it->pos = itFree->pos;
                itFree->pos += it->len;
                itFree->len -= it->len;
                break;
            }
        }
    }
}

long long summarize(const Input &input)
{
    long long result = 0;
    for (auto extent : input.files)
    {
        for (int i = extent.pos; i < extent.pos + extent.len; i++)
        {
            result += i * extent.id;
        }
    }
    return result;
}

int main(int argc, char **argv)
{
    Input input = readInput();
    process(input);
    std::cout << summarize(input) << std::endl;
    return 0;
}

