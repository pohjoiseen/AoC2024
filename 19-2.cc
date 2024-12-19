#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>

struct Input
{
    std::vector<std::string> words;
    int maxLength = 0;
};

Input readInput()
{
    Input input;
    std::string line, word;
    std::getline(std::cin, line);
    std::stringstream ss(line);
    // split on spaces
    while (std::getline(ss, word, ' '))
    {
        // remove also commas
        if (word[word.size() - 1] == ',') word = word.substr(0, word.size() - 1);
        input.words.push_back(word);
        // remember maximum possible word length
        if (word.size() > input.maxLength)
            input.maxLength = word.size();
    }
    std::getline(std::cin, line);  // skip empty line

    // pre-sort words for binary search
    std::sort(input.words.begin(), input.words.end());
    return input;
}

long long testSequence(const Input &input, std::string_view seq)
{
    // cache results for subsequences
    static std::unordered_map<std::string_view, long long> cache;
    auto cached = cache.find(seq);
    if (cached != cache.end()) return cached->second;

    // test prefixes up to the max possible word length
    long long result = 0;
    for (int i = 1; i <= input.maxLength; i++)
    {
        if (i > seq.size()) continue;
        // if the prefix is a valid word, recurse
        if (std::binary_search(input.words.begin(), input.words.end(), seq.substr(0, i)))
            result += seq.size() > i ? testSequence(input, seq.substr(i)) : 1;
    }

    cache[seq] = result;
    return result;
}

int main(int argc, char **argv)
{
    Input input = readInput();
    long long result = 0;
    std::string seq;
    while (std::getline(std::cin, seq))
        result += testSequence(input, seq);
    std::cout << result << std::endl;
    return 0;
}