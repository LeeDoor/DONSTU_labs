#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <fstream>
#include <cstdlib>
#include <tuple>

using namespace std;
typedef set<int> StateSet;

void render(const string& name, const string& dot) {
    ofstream(name + ".dot") << dot;
    system(("dot -Tpng " + name + ".dot -o " + name + ".png").c_str());
}

int main() {
    int n;
    if (!(cin >> n)) return 0;

    // ---- конечные состояния ----
    vector<int> finals;
    string line;
    getline(cin, line);          // остаток строки с n
    getline(cin, line);
    {
        istringstream iss(line);
        int fs;
        while (iss >> fs) finals.push_back(fs);
    }

    // ---- рёбра ----
    struct Trans { int from, to; string sym; };
    vector<Trans> trans;
    set<string> symbolsSet;

    while (getline(cin, line)) {
        if (line.find_first_not_of(" \t\r\n") == string::npos) continue;
        istringstream is(line);
        int f, t;
        if (!(is >> f >> t)) continue;
        string s;
        if (is >> s) { trans.push_back({f, t, s}); symbolsSet.insert(s); }
        else         { trans.push_back({f, t, ""}); }
    }

    // ---- ε-замыкания ----
    vector<StateSet> eclos(n);
    for (int i = 0; i < n; ++i) {
        set<int> visited; vector<int> stack = { i };
        visited.insert(i);
        while (!stack.empty()) {
            int cur = stack.back(); stack.pop_back();
            for (auto &t : trans)
                if (t.from == cur && t.sym.empty() && !visited.count(t.to)) {
                    visited.insert(t.to); stack.push_back(t.to);
                }
        }
        eclos[i] = visited;
    }

    auto printSetQ = [](const StateSet& S) {
        cout << "{"; bool first = true;
        for (int s : S) { if (!first) cout << ", "; cout << "q" << s; first = false; }
        cout << "}";
    };
    auto printSetS = [](const StateSet& S) {
        cout << "{"; bool first = true;
        for (int s : S) { if (!first) cout << ", "; cout << "S" << s; first = false; }
        cout << "}";
    };

    // ---- вывод ε-замыканий ----
    cout << "Epsilon closures:\n";
    for (int i = 0; i < n; ++i) {
        cout << "S" << i << " = ~(q" << i << ") = ";
        printSetQ(eclos[i]);
        cout << "\n";
    }
    cout << "\n";

    auto moveSet = [&](const StateSet& S, const string& sym) {
        StateSet r;
        for (int s : S) for (auto &t : trans)
            if (t.from == s && t.sym == sym) r.insert(t.to);
        return r;
    };
    auto eclose = [&](const StateSet& S) {
        StateSet r;
        for (int s : S) for (int e : eclos[s]) r.insert(e);
        return r;
    };

    vector<string> symList(symbolsSet.begin(), symbolsSet.end());

    // ширина колонки для таблиц
    size_t symWidth = 4;
    for (auto& s : symList) symWidth = max(symWidth, s.size() + 2);

    // ---- таблица переходов NFA (значения — через S) ----
    cout << "Transition table:\n";
    cout << left << setw(8) << "S[i]";
    for (auto& s : symList) cout << left << setw(symWidth) << s;
    cout << "\n";

    for (int i = 0; i < n; ++i) {
        cout << left << setw(8) << ("S" + to_string(i));
        for (auto& sym : symList) {
            StateSet r = eclose(moveSet(eclos[i], sym));
            string cell;
            if (r.empty()) cell = "{}";
            else {
                cell = "{";
                bool first = true;
                for (int s : r) {
                    if (!first) cell += ", ";
                    cell += "S" + to_string(s);
                    first = false;
                }
                cell += "}";
            }
            cout << left << setw(symWidth) << cell;
        }
        cout << "\n";
    }
    cout << "\n";

    // ---- детерминирование ----
    vector<StateSet> dfaStates;
    map<StateSet, int> dfaIndex;
    auto getIndex = [&](const StateSet& s) {
        auto it = dfaIndex.find(s);
        if (it != dfaIndex.end()) return it->second;
        int idx = (int)dfaStates.size();
        dfaIndex[s] = idx; dfaStates.push_back(s);
        return idx;
    };
    vector<map<string, int>> dfaTrans;

    getIndex(eclose(StateSet{0}));
    for (size_t i = 0; i < dfaStates.size(); ++i) {
        while (dfaTrans.size() <= i) dfaTrans.push_back({});
        for (auto &sym : symList) {
            StateSet r = eclose(moveSet(dfaStates[i], sym));
            if (r.empty()) continue;
            int j = getIndex(r);
            while (dfaTrans.size() <= (size_t)j) dfaTrans.push_back({});
            dfaTrans[i][sym] = j;
        }
    }

    // ---- вывод состояний DFA (через P) ----
    cout << "DFA states:\n";
    for (size_t i = 0; i < dfaStates.size(); ++i) {
        cout << "P" << i << " = ";
        printSetQ(dfaStates[i]);
        cout << "\n";
    }
    cout << "\nInitial state: P0\n";

    cout << "Final states:";
    for (size_t i = 0; i < dfaStates.size(); ++i)
        for (int s : dfaStates[i])
            if (find(finals.begin(), finals.end(), s) != finals.end()) {
                cout << " P" << i; break;
            }
    cout << "\n\n";

    // ---- таблица переходов DFA (значения — через P) ----
    cout << "DFA transition table:\n";
    cout << left << setw(8) << "P[i]";
    for (auto& s : symList) cout << left << setw(symWidth) << s;
    cout << "\n";

    for (size_t i = 0; i < dfaStates.size(); ++i) {
        cout << left << setw(8) << ("P" + to_string(i));
        for (auto& sym : symList) {
            auto it = dfaTrans[i].find(sym);
            string cell = (it == dfaTrans[i].end()) ? "-" : ("P" + to_string(it->second));
            cout << left << setw(symWidth) << cell;
        }
        cout << "\n";
    }
    cout << "\n";

    // ============================================================
    //  Отрисовка графов через Graphviz
    // ============================================================

    // 1. Исходный NFA (с ε-переходами)
    {
        string d = "digraph NFA{rankdir=LR;node[shape=circle];start[shape=point];start->q0;";
        for (int f : finals) d += "q" + to_string(f) + "[shape=doublecircle];";
        for (auto& t : trans)
            d += "q" + to_string(t.from) + "->q" + to_string(t.to)
               + "[label=\"" + (t.sym.empty() ? "ε" : t.sym) + "\"];";
        render("nfa", d + "}");
    }

    // 2. NFA без ε-переходов (ε «вшиты» в рёбра)
    {
        set<tuple<int,int,string>> e;
        for (int p = 0; p < n; ++p)
            for (int q : eclos[p])
                for (auto& t : trans)
                    if (t.from == q && !t.sym.empty())
                        for (int r : eclos[t.to])
                            e.insert({p, r, t.sym});
        string d = "digraph NFAeps{rankdir=LR;node[shape=circle];start[shape=point];start->q0;";
        for (int f : finals) d += "q" + to_string(f) + "[shape=doublecircle];";
        for (auto& [a,b,s] : e)
            d += "q" + to_string(a) + "->q" + to_string(b) + "[label=\"" + s + "\"];";
        render("nfa_no_eps", d + "}");
    }

    // 3. Детерминированный DFA
    {
        string d = "digraph DFA{rankdir=LR;node[shape=circle];start[shape=point];start->P0;";
        for (size_t i = 0; i < dfaStates.size(); ++i)
            for (int s : dfaStates[i])
                if (find(finals.begin(), finals.end(), s) != finals.end())
                    { d += "P" + to_string(i) + "[shape=doublecircle];"; break; }
        for (size_t i = 0; i < dfaTrans.size(); ++i)
            for (auto& kv : dfaTrans[i])
                d += "P" + to_string(i) + "->P" + to_string(kv.second)
                   + "[label=\"" + kv.first + "\"];";
        render("dfa", d + "}");
    }

    cout << "Graphs written: nfa.png, nfa_no_eps.png, dfa.png\n";
    return 0;
}
