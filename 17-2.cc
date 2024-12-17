#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdio>

struct State
{
    long long a, b, c;
    int pc = 0;
    std::vector<int> instr;
    std::vector<int> out;
};

State readInput()
{
    State state;

    if (1 != std::scanf("Register A: %d\n", &state.a))
    {
        std::cerr << "Failed to read initial register A value!" << std::endl;
        std::exit(1);
    }
    if (1 != std::scanf("Register B: %d\n", &state.b))
    {
        std::cerr << "Failed to read initial register B value!" << std::endl;
        std::exit(1);
    }
    if (1 != std::scanf("Register C: %d\n", &state.c))
    {
        std::cerr << "Failed to read initial register C value!" << std::endl;
        std::exit(1);
    }

    std::string line;
    std::getline(std::cin, line);  // skip empty string
    std::getline(std::cin, line);
    if (line.find("Program: ") != 0)
    {
        std::cerr << "Failed to read instructions stream!" << std::endl;
        std::exit(1);
    }
    std::stringstream sstr(line.substr(9));
    while (!sstr.eof())
    {
        int n;
        char c;
        sstr >> n;
        if (sstr.fail())
        {
            std::cerr << "Failed to read instructions stream!" << std::endl;
            std::exit(1);
        }
        sstr >> c;  // skip comma
        state.instr.push_back(n);
    }

    return state;
}

long long comboAddr(const State &state, int val)
{
    if (val >= 0 && val <= 3) return val;
    if (val == 4) return state.a;
    if (val == 5) return state.b;
    if (val == 6) return state.c;
    std::cerr << "Invalid combo operand: " << val << std::endl;
    std::exit(1);
}

void runVM(State &state)
{
    while (state.pc < state.instr.size())
    {
        int instr = state.instr[state.pc], operand = state.instr[state.pc + 1];
        state.pc += 2;

        switch (instr)
        {
            case 0: // adv
                state.a /= (1 << comboAddr(state, operand));
                break;

            case 1: // bxl
                state.b ^= operand;
                break;

            case 2: // bst
                state.b = comboAddr(state, operand) % 8;
                break;

            case 3: // jnz
                if (state.a) state.pc = operand;
                break;

            case 4: // bxc
                state.b ^= state.c;
                break;

            case 5: // out
                state.out.push_back(comboAddr(state, operand) % 8);
                break;

            case 6: // bdv
                state.b = state.a / (1 << comboAddr(state, operand));
                break;

            case 7: // cdv
                state.c = state.a / (1 << comboAddr(state, operand));
                break;
        }
    }
}

std::vector<int> test(const State &input, long long n)
{
    State state = input;
    state.a = n;
    runVM(state);
    return state.out;
}

long long solve(const State &input, long long subresult)
{
    subresult <<= 3;
    for (int j = 0; j < 8; j++)
    {
        long long n = subresult | j;
        auto out = test(input, n);
        if (out == std::vector<int>(input.instr.end() - out.size(), input.instr.end()))
        {
            //std::cout << std::oct << n << std::dec << " ->";
            //for (auto n : out) std::cout << ' ' << n;
            //std::cout << std::endl;
            if (out.size() == input.instr.size()) return n;

            long long result = solve(input, n);
            if (result >= 0) return result;
        }
    }

    return -1;
}

int main(int argc, char **argv)
{
    State state = readInput();
    long long result = solve(state, 0);
    std::cout << result << std::endl;
    return 0;
}