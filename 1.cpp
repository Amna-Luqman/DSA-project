#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <map>
#include <algorithm>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <cctype>
using namespace std;


class Room {
public:
    string name;
    string description;
    bool locked;
    vector<string> connections;

    Room(string n="", string d="", bool l=false)
        : name(n), description(d), locked(l) {}
};