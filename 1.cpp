#include <iostream>
#include <vector>
#include <stack>
#include <map>
#include <algorithm>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <cctype>
using namespace std;

/////////////////////////////////////////////////////////
// ROOM CLASS
/////////////////////////////////////////////////////////
class Room {
public:
    string name;
    string description;
    bool locked;

    Room(string n = "", string d = "", bool l = false)
        : name(n), description(d), locked(l) {}
};

/////////////////////////////////////////////////////////
// INVENTORY CLASS
/////////////////////////////////////////////////////////
class Inventory {
private:
    stack<string> items;

public:
    void addItem(string item) {
        items.push(item);
        cout << "Added '" << item << "' to inventory." << endl;
    }

    bool useItem(string item) {
        if (items.empty()) {
            cout << "Inventory is empty" << endl;
            return false;
        }

        stack<string> temp;
        bool found = false;

        while (!items.empty()) {
            string top = items.top();
            items.pop();
            if (top == item) {
                found = true;
                cout << "Used '" << item << "'." << endl;
                break;
            }
            temp.push(top);
        }

        while (!temp.empty()) {
            items.push(temp.top());
            temp.pop();
        }

        if (!found) cout << "Item not found" << endl;
        return found;
    }

    void display() {
        if (items.empty()) {
            cout << "Inventory empty" << endl;
            return;
        }

        stack<string> temp = items;
        vector<string> v;
        while (!temp.empty()) { v.push_back(temp.top()); temp.pop(); }

        cout << "Inventory: ";
        for (int i = 0; i < v.size(); i++)
            cout << v[i] << (i < v.size() - 1 ? ", " : "");
        cout << endl;
    }

    stack<string> getItems() { return items; }
    void setItems(stack<string> s) { items = s; }
};

/////////////////////////////////////////////////////////
// GAME LEVEL CLASS
/////////////////////////////////////////////////////////
class GameLevel {
private:
    map<string, Room*> rooms;
    map<string, vector<string>> adj;
    map<string, string> roomItems; // Room -> item

    vector<pair<string, string>> riddles = {
        {"I speak without a mouth and hear without ears. What am I?", "echo"},
        {"The more you take, the more you leave behind. What am I?", "footsteps"},
        {"What has keys but cannot open locks?", "piano"},
        {"What has to be broken before you can use it?", "egg"},
        {"I am tall when young and short when old. What am I?", "candle"}
    };

public:
    string currentRoom, startRoom, endRoom;
    int level;

    GameLevel(int lvl) : level(lvl) { init(); }

    void init() {
        if (level == 1) {
            rooms["entrance"] = new Room("entrance", "Castle entrance.", false);
            rooms["hall"] = new Room("hall", "Great castle hall.", false);
            rooms["armory"] = new Room("armory", "Old armory.", true);
            rooms["library"] = new Room("library", "Ancient library.", true);
            rooms["throne"] = new Room("throne", "Throne chamber.", false);

            connect("entrance", "hall");
            connect("hall", "armory");
            connect("hall", "library");
            connect("library", "throne");

            startRoom = "entrance";
            endRoom = "throne";

            roomItems["armory"] = "sword";
            roomItems["library"] = "ancient_book";
        }
        else if (level == 2) {
            rooms["garden"] = new Room("garden", "Mystic garden.", false);
            rooms["pond"] = new Room("pond", "Quiet pond.", false);
            rooms["grove"] = new Room("grove", "Sacred grove.", true);
            rooms["arch"] = new Room("arch", "Stone archway.", true);
            rooms["gate"] = new Room("gate", "Gate to next realm.", false);

            connect("garden", "pond");
            connect("pond", "grove");
            connect("grove", "arch");
            connect("arch", "gate");

            startRoom = "garden";
            endRoom = "gate";

            roomItems["grove"] = "magic_leaf";
            roomItems["arch"] = "silver_key";
        }
        else if (level == 3) {
            rooms["dungeon"] = new Room("dungeon", "Dark dungeon.", false);
            rooms["cells"] = new Room("cells", "Prison cells.", true);
            rooms["crypt"] = new Room("crypt", "Ancient crypt.", true);
            rooms["pit"] = new Room("pit", "Deep pit.", false);
            rooms["altar"] = new Room("altar", "Hidden altar.", false);

            connect("dungeon", "cells");
            connect("cells", "crypt");
            connect("crypt", "altar");
            connect("altar", "pit");

            startRoom = "dungeon";
            endRoom = "pit";

            roomItems["cells"] = "prison_key";
            roomItems["crypt"] = "crystal_orb";
        }
        else {
            rooms["tower"] = new Room("tower", "High tower.", false);
            rooms["hall2"] = new Room("hall2", "King's hall.", true);
            rooms["vault"] = new Room("vault", "Treasure vault.", true);
            rooms["chamber"] = new Room("chamber", "Final chamber.", false);
            rooms["crown"] = new Room("crown", "Crown room. Final prize.", false);

            connect("tower", "hall2");
            connect("hall2", "vault");
            connect("vault", "chamber");
            connect("chamber", "crown");

            startRoom = "tower";
            endRoom = "crown";

            roomItems["hall2"] = "golden_shield";
            roomItems["vault"] = "treasure_chest";
        }

        currentRoom = startRoom;
    }

    void connect(string a, string b) {
        adj[a].push_back(b);
        adj[b].push_back(a);
    }

    vector<string> getConnected(string r) { return adj[r]; }

    bool moveTo(string target) {
        string t = lower(target);
        for (string x : adj[currentRoom])
            if (lower(x) == t) { currentRoom = x; return true; }
        return false;
    }

    bool solveRiddle() {
        int idx = rand() % riddles.size();
        cout << "Riddle: " << riddles[idx].first << endl;
        cout << "Answer: ";
        string ans;
        getline(cin, ans);
        return lower(ans) == riddles[idx].second;
    }

    bool isLocked(string room) { return rooms[room]->locked; }
    void unlock(string room) { rooms[room]->locked = false; }
    Room* getRoom() { return rooms[currentRoom]; }
    bool complete() { return currentRoom == endRoom; }

    string getRoomItem(string room) {
        if (roomItems.find(room) != roomItems.end())
            return roomItems[room];
        return "";
    }

    string lower(string s) {
        transform(s.begin(), s.end(), s.begin(), ::tolower);
        return s;
    }
};

/////////////////////////////////////////////////////////
// GAME CLASS
/////////////////////////////////////////////////////////
class Game {
private:
    GameLevel* level;
    Inventory inv;
    int levelNum;
    int health;

public:
    Game() : levelNum(1), health(3) {
        srand(time(0));
        level = new GameLevel(levelNum);
    }

    void start() {
        cout << "RIPHAH INTERNATIONAL UNIVERSITY" << endl;
        cout << "ESCAPE PUZZLE GAME" << endl;
        cout << "Team: Areeba Noor, Amna Luqman, Arooj Fatima, Bisma Amin" << endl;
        cout << "Supervisor: Saleha Nasim" << endl;
        cout << "=====================================" << endl;
        cout << "Type 'help' to see available commands" << endl;
        help();

        string cmd;
        while (true) {
            if (health <= 0) {
                cout << "Health zero. Game Over" << endl;
                return;
            }
            cout << "\nHealth: " << health << endl;
            cout << "[" << level->currentRoom << "] > ";
            getline(cin, cmd);
            if (!process(cmd)) break;
        }
    }

    bool process(string c) {
        string s = lower(c);
        if (s == "quit") return false;
        if (s == "help") help();
        else if (s == "look") look();
        else if (s == "inventory") inv.display();
        else if (s.find("go ") == 0) go(c.substr(3));
        else if (s.find("use ") == 0) use(c.substr(4));
        else if (s == "save") save();
        else if (s == "load") load();
        else cout << "Invalid command." << endl;
        return true;
    }

    void go(string room) {
        if (!level->moveTo(room)) {
            cout << "Cannot go there" << endl;
            showConnected();
            return;
        }

        string cr = level->currentRoom;

        if (level->isLocked(cr)) {
            cout << "Room is locked. Solve riddle to unlock.\n";
            if (level->solveRiddle()) {
                level->unlock(cr);
                cout << "Room unlocked.\n";

                inv.addItem("mystic_token"); // Bonus token
                string item = level->getRoomItem(cr);
                if (!item.empty()) {
                    inv.addItem(item);
                    cout << "You obtained a '" << item << "' from " << cr << endl;
                }

                // Count mystic_tokens
                int count = 0;
                stack<string> temp = inv.getItems();
                while (!temp.empty()) {
                    if (temp.top() == "mystic_token") count++;
                    temp.pop();
                }
                cout << "You now have " << count << " mystic_token(s)" << endl;
            } else {
                cout << "Incorrect. You cannot enter" << endl;
                health--;
                level->currentRoom = level->startRoom;
                cout << "Returned to start of level" << endl;
                return;
            }
        }

        look();

        if (level->complete()) {
            cout << "LEVEL COMPLETE" << endl;
            if (levelNum < 4) {
                levelNum++;
                delete level;
                level = new GameLevel(levelNum);
                cout << "LEVEL " << levelNum << " STARTED" << endl;
                look();
            } else {
                cout << "GAME COMPLETED" << endl;
            }
        }
    }

    void use(string item) {
        if (inv.useItem(item)) {
            if (item == "mystic_token") {
                health++;
                cout << "Health increased to " << health << endl;
            } else if (item == "sword") {
                cout << "You swing the sword! Useful for battles." << endl;
            } else if (item == "ancient_book") {
                cout << "You read the ancient book. Knowledge increased!" << endl;
            } else if (item == "silver_key") {
                cout << "Silver key can unlock special doors." << endl;
            } else if (item == "magic_leaf") {
                cout << "Magic leaf glows. You feel refreshed!" << endl;
            } else if (item == "prison_key") {
                cout << "Prison key might open locked cells." << endl;
            } else if (item == "crystal_orb") {
                cout << "Crystal orb radiates energy." << endl;
            } else if (item == "golden_shield") {
                cout << "Golden shield can protect you." << endl;
            } else if (item == "treasure_chest") {
                cout << "You found treasure inside the chest!" << endl;
            }
        }
    }

    void look() {
        Room* r = level->getRoom();
        cout << r->name << ": " << r->description << endl;
        showConnected();
    }

    void showConnected() {
        auto c = level->getConnected(level->currentRoom);
        cout << "Connected rooms: ";
        for (int i = 0; i < c.size(); i++)
            cout << c[i] << (i < c.size() - 1 ? ", " : "");
        cout << endl;
    }

    void help() {
        cout << "====================== COMMANDS ======================" << endl;
        cout << "go <room>       - Move to a connected room" << endl;
        cout << "look            - Describe the current room" << endl;
        cout << "inventory       - Show items in your inventory" << endl;
        cout << "use <item>      - Use an item (e.g., mystic_token to heal)" << endl;
        cout << "save            - Save current game state" << endl;
        cout << "load            - Load saved game state" << endl;
        cout << "help            - Show this help message" << endl;
        cout << "quit            - Exit the game" << endl;
        cout << "=====================================================" << endl;
    }

    void save() {
        ofstream f("save.txt");
        f << levelNum << endl;
        f << level->currentRoom << endl;
        f << health << endl;

        stack<string> s = inv.getItems();
        vector<string> v;
        while (!s.empty()) { v.push_back(s.top()); s.pop(); }
        f << v.size() << endl;
        for (string x : v) f << x << endl;

        f.close();
        cout << "Game saved" << endl;
    }

    void load() {
        ifstream f("save.txt");
        if (!f) { cout << "No save file." << endl; return; }

        f >> levelNum;
        f.ignore();
        string room;
        getline(f, room);
        f >> health;
        f.ignore();

        int n; f >> n; f.ignore();
        stack<string> st;
        for (int i = 0; i < n; i++) {
            string x; getline(f, x);
            st.push(x);
        }

        delete level;
        level = new GameLevel(levelNum);
        level->currentRoom = room;
        inv.setItems(st);

        cout << "Game loaded" << endl;
        look();
    }

    string lower(string s) {
        transform(s.begin(), s.end(), s.begin(), ::tolower);
        return s;
    }
};

/////////////////////////////////////////////////////////
// MAIN FUNCTION
/////////////////////////////////////////////////////////
int main() {
    Game g;
    g.start();
    return 0;
}
