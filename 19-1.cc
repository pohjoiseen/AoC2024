#include <iostream>
#include <sstream>
#include <vector>
#include <string>
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

bool testSequence(const Input &input, std::string seq, std::vector<std::string> usedWords)
{
    if (!seq.size())
    {
        //std::cout << "found:";
        //for (auto w : usedWords) std::cout << ' ' << w;
        //std::cout << std::endl;
        return true;
    }

    for (int i = 1; i <= input.maxLength; i++)
    {
        if (i > seq.size()) continue;
        std::string subseq = seq.substr(0, i);
        if (std::binary_search(input.words.begin(), input.words.end(), seq.substr(0, i)))
        {
            usedWords.push_back(subseq);
            if (testSequence(input, seq.substr(i), usedWords)) return true;
            usedWords.pop_back();
        }
    }

    //if (!usedWords.size()) std::cout << "not found: " << seq << std::endl;
    return false;
}

int main(int argc, char **argv)
{
    Input input = readInput();
    int result = 0;
    std::string seq;
    while (std::getline(std::cin, seq))
        if (testSequence(input, seq, {})) result++;
    std::cout << result << std::endl;
    return 0;
}