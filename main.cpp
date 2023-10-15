#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>

struct Automaton {
    int states;
    int startState;
    std::vector<int> acceptingStates;
    std::unordered_map<int, std::unordered_map<char, int>> transitions;
};

Automaton readFromFile(const std::string &fileName) {
    Automaton automaton;
    std::ifstream file(fileName);

    if (!file || file.peek() == std::ifstream::traits_type::eof()) {
        std::cerr << "Error while opening the file" << std::endl;
        exit(1);
    }

    file >> automaton.states;
    file >> automaton.startState;

    int finalStates;
    file >> finalStates;
    automaton.acceptingStates.resize(finalStates);
    for (int i = 0; i < finalStates; ++i) {
        file >> automaton.acceptingStates[i];
    }

    int from, to;
    char transitionSymbol;
    while (file >> from >> transitionSymbol >> to) {
        automaton.transitions[from][transitionSymbol] = to;
    }

    file.close();
    return automaton;
}

bool acceptWord(Automaton fa, const std::string word) {
    int currentState = fa.startState;
    for (size_t i = 0; i < word.size(); ++i) {
        char ch = word[i];

        if (fa.transitions[currentState].find(ch) == fa.transitions[currentState].end()) {
            return false;
        }

        currentState = fa.transitions[currentState][ch];
    }

    for (int state : fa.acceptingStates) {
        if (currentState == state)
        {
            return true;
        }
    }
    return false;
}

bool checkForWords(const Automaton &automaton, const std::string &w0) {
    for (int len = 1; len <= 10; ++len) {
        for (int i = 0; i < (1 << len); ++i) {
            std::string w1;
            for (int j = 0; j < len; ++j) {
                if (i & (1 << j)) w1 += 'b';
                else w1 += 'a';
            }
            if (acceptWord(automaton, w0 + w1)) return true;
        }
    }
    return false;
}

int main() {
    Automaton automaton = readFromFile("test.txt");

    std::string w0;

    std::cout << "Enter w0: ";
    std::cin >> w0;

    if (checkForWords(automaton, w0)) {
        std::cout << "Form w0 w1 accepted by the automaton" << std::endl;
    } else {
        std::cout << "Form w0 w1 does not accepted by the automaton" << std::endl;
    }

    return 0;
}
