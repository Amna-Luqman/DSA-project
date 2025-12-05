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

/////////////////////////////////////////////////////////
// ROOM CLASS
// Each room is a "node" in our game graph
/////////////////////////////////////////////////////////
class Room {
public:
    string name;                   // Room name
    string description;            // Room description shown to player
    bool locked;                   // If true, room is locked until riddle is solved
    vector<string> connections;    // Names of connected rooms (edges in graph)

    // Constructor to initialize room
    Room(string n="", string d="", bool l=false)
        : name(n), description(d), locked(l) {}
};

/////////////////////////////////////////////////////////
// INVENTORY CLASS (STACK BASED)
// Stack is used to store items because LIFO is suitable for usage
/////////////////////////////////////////////////////////
class Inventory {
private:
    stack<string> items; // Stack to store items

public:
    // Add an item to inventory
    void addItem(string item) {
        items.push(item);
        cout << "Added '" << item << "' to inventory." << endl;
    }

    // Use an item from inventory
    bool useItem(string item) {
        if (items.empty()) {
            cout << "Inventory is empty" << endl;
            return false;
        }

        stack<string> temp; // Temporary stack to search for item
        bool found = false;

        // Search for item
        while (!items.empty()) {
            string top = items.top();
            items.pop();
            if (top == item) {
                found = true;
                cout << "Used '" << item << endl;
                break;
            }
            temp.push(top);
        }

        // Restore remaining items back to main stack
        while (!temp.empty()) {
            items.push(temp.top());
            temp.pop();
        }

        if (!found) cout << "Item not found" << endl;
        return found;
    }

    // Display inventory
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
            cout << v[i] << (i < v.size()-1 ? ", " : "");
        cout << endl;
    }

    // Get/Set inventory (for save/load)
    stack<string> getItems() { return items; }
    void setItems(stack<string> s) { items = s; }
};

/////////////////////////////////////////////////////////
// GAME LEVEL CLASS
// Manages rooms, connections (graph), riddles, and level logic
/////////////////////////////////////////////////////////
class GameLevel {
private:
    map<string, Room*> rooms;               // Map of room name to Room object (fast lookup)
    map<string, vector<string>> adj;        // Graph adjacency list: room -> connected rooms

    // Riddles used to unlock locked rooms
    vector<pair<string,string>> riddles = {
        {"I speak without a mouth and hear without ears. What am I?", "echo"},
        {"The more you take, the more you leave behind. What am I?", "footsteps"},
        {"What has keys but cannot open locks?", "piano"},
        {"What has to be broken before you can use it?", "egg"},
        {"I am tall when young and short when old. What am I?", "candle"}
    };

public:
    string currentRoom, startRoom, endRoom; // Track current, start, and end rooms
    int level;                               // Current level number

    // Constructor: initialize level
    GameLevel(int lvl) : level(lvl) { init(); }

    // Initialize rooms and connections for each level
    void init() {
        if (level == 1) {
            // Creating rooms for Level 1
            rooms["entrance"] = new Room("entrance","Castle entrance.",false);
            rooms["hall"] = new Room("hall","Great castle hall.",false);
            rooms["armory"] = new Room("armory","Old armory.",true); // Locked
            rooms["library"] = new Room("library","Ancient library.",true); // Locked
            rooms["throne"] = new Room("throne","Throne chamber.",false);

            // Connecting rooms (graph edges)
            connect("entrance","hall");
            connect("hall","armory");
            connect("hall","library");
            connect("library","throne");

            startRoom = "entrance";
            endRoom = "throne";
        }
        // Level 2 rooms and connections
        else if (level == 2) {
            rooms["garden"] = new Room("garden","Mystic garden.",false);
            rooms["pond"] = new Room("pond","Quiet pond.",false);
            rooms["grove"] = new Room("grove","Sacred grove.",true);
            rooms["arch"] = new Room("arch","Stone archway.",true);
            rooms["gate"] = new Room("gate","Gate to next realm.",false);

            connect("garden","pond");
            connect("pond","grove");
            connect("grove","arch");
            connect("arch","gate");

            startRoom = "garden";
            endRoom = "gate";
        }
        // Level 3 rooms and connections
        else if (level == 3) {
            rooms["dungeon"] = new Room("dungeon","Dark dungeon.",false);
            rooms["cells"] = new Room("cells","Prison cells.",true);
            rooms["crypt"] = new Room("crypt","Ancient crypt.",true);
            rooms["pit"] = new Room("pit","Deep pit.",false);
            rooms["altar"] = new Room("altar","Hidden altar.",false);

            connect("dungeon","cells");
            connect("cells","crypt");
            connect("crypt","altar");
            connect("altar","pit");

            startRoom = "dungeon";
            endRoom = "pit";
        }
        // Level 4 rooms and connections
        else {
            rooms["tower"] = new Room("tower","High tower.",false);
            rooms["hall2"] = new Room("hall2","King's hall.",true);
            rooms["vault"] = new Room("vault","Treasure vault.",true);
            rooms["chamber"] = new Room("chamber","Final chamber.",false);
            rooms["crown"] = new Room("crown","Crown room. Final prize.",false);

            connect("tower","hall2");
            connect("hall2","vault");
            connect("vault","chamber");
            connect("chamber","crown");

            startRoom = "tower";
            endRoom = "crown";
        }

        currentRoom = startRoom;
    }

    // Connect two rooms (bidirectional)
    void connect(string a, string b) {
        adj[a].push_back(b);
        adj[b].push_back(a);
    }

    // Get rooms connected to a given room
    vector<string> getConnected(string r) { return adj[r]; }

    // Move player to target room if connected
    bool moveTo(string target) {
        string t = lower(target);
        for (string x : adj[currentRoom])
            if (lower(x) == t) { currentRoom = x; return true; }
        return false;
    }

    // Solve a random riddle to unlock room
    bool solveRiddle() {
        int idx = rand() % riddles.size();
        cout << "Riddle: " << riddles[idx].first << endl;
        cout << "Answer: ";
        string ans;
        getline(cin, ans);
        return lower(ans) == riddles[idx].second;
    }

    bool isLocked(string room) { return rooms[room]->locked; } // Check if room is locked
    void unlock(string room) { rooms[room]->locked = false; }  // Unlock a room

    Room* getRoom() { return rooms[currentRoom]; }  // Get current room object
    bool complete() { return currentRoom == endRoom; } // Check if level complete

    // Convert string to lowercase (for case-insensitive comparison)
    string lower(string s) {
        transform(s.begin(), s.end(), s.begin(), ::tolower);
        return s;
    }
};

/////////////////////////////////////////////////////////
// MAIN GAME CLASS
// Handles gameplay, commands, inventory, and levels
/////////////////////////////////////////////////////////
class Game {
private:
    GameLevel* level;   // Current level object
    Inventory inv;      // Player inventory
    int levelNum;       // Current level number
    int health;         // Player health

public:
    // Constructor
    Game() : levelNum(1), health(3) {
        srand(time(0));  // Random seed for riddles
        level = new GameLevel(levelNum);
    }

    // Start the game
    void start() {
        cout << "RIPHAH INTERNATIONAL UNIVERSITY"<<endl;
        cout << "ESCAPE PUZZLE GAME"<<endl;
        cout << "Team: Areeba Noor, Amna Luqman, Arooj Fatima, Bisma Amin"<<endl;
        cout << "Supervisor: Saleha Nasim"<<endl;
        cout << "====================================="<<endl;
        cout << "Type 'help' to see available commands"<<endl;
        help(); // Show commands at start

        string cmd;
        while (true) {
            if (health <= 0) {  // Check game over
                cout << "Health zero. Game Over"<<endl;
                return;
            }
            cout << "\nHealth: " << health <<endl;
            cout << "[" << level->currentRoom << "] > "; // Show current room prompt
            getline(cin, cmd);
            if (!process(cmd)) break; // Process command, exit if false (quit)
        }
    }

    // Process player commands
    bool process(string c) {
        string s = lower(c); // Make lowercase for easy comparison

        if (s == "quit") return false;      // Quit game
        if (s == "help") help();            // Show help
        else if (s == "look") look();       // Look around
        else if (s == "inventory") inv.display(); // Show inventory
        else if (s.find("go ") == 0) go(c.substr(3)); // Move to room
        else if (s.find("use ") == 0) use(c.substr(4)); // Use item
        else if (s == "save") save();       // Save game
        else if (s == "load") load();       // Load game
        else cout << "Invalid command."<<endl;

        return true;
    }

    // Move to another room
    void go(string room) {
        if (!level->moveTo(room)) {
            cout << "Cannot go there"<<endl;
            showConnected(); // Show connected rooms
            return;
        }

        string cr = level->currentRoom;

        // Check if room is locked
        if (level->isLocked(cr)) {
            cout << "Room is locked. Solve riddle to unlock.\n";
            if (level->solveRiddle()) {        // Solve riddle
                level->unlock(cr);             // Unlock room
                cout << "Room unlocked.\n";
                inv.addItem("mystic_token");   // Reward token

                // Count how many mystic_tokens player has
                int count = 0;
                stack<string> temp = inv.getItems();
                while (!temp.empty()) { 
                    if (temp.top() == "mystic_token"){
                        count++; 
                    }
                    temp.pop(); 
                }
                cout << "You now have " << count << " mystic_token"<<endl;
            } else {
                cout << "Incorrect. You cannot enter"<<endl;
                health--;                      // Lose health
                level->currentRoom = level->startRoom; // Return to start
                cout << "Returned to start of level"<<endl;
                return;
            }
        }

        look(); // Show room description

        // Check if level is complete
        if (level->complete()) {
            cout << "LEVEL COMPLETE"<<endl;
            if (levelNum < 4) {
                levelNum++;
                delete level;
                level = new GameLevel(levelNum); // Start new level
                cout << "LEVEL " << levelNum << " STARTED"<<endl;
                look();
            } else {
                cout << "GAME COMPLETED"<<endl;
            }
        }
    }

    // Use an item
    void use(string item) {
        if (inv.useItem(item)) {
            if (item == "mystic_token") { // Mystic token increases health
                health++;
                cout << "Health increased to " << health << endl;
            }
        }
    }

    // Look around current room
    void look() {
        Room* r = level->getRoom();
        cout << r->name << ": " << r->description << endl;
        showConnected(); // Show connected rooms
    }

    // Show connected rooms
    void showConnected() {
        auto c = level->getConnected(level->currentRoom);
        cout << "Connected rooms: ";
        for (int i = 0; i < c.size(); i++)
            cout << c[i] << (i < c.size()-1 ? ", " : "");
        cout << endl;
    }

    // Display available commands
    void help() {
        cout << "====================== COMMANDS ======================"<<endl;
        cout << "go <room>       - Move to a connected room"<<endl;
        cout << "look            - Describe the current room"<<endl;
        cout << "inventory       - Show items in your inventory\n";
        cout << "use <item>      - Use an item (e.g., mystic_token to heal)"<<endl;
        cout << "save            - Save current game state"<<endl;
        cout << "load            - Load saved game state"<<endl;
        cout << "help            - Show this help message"<<endl;
        cout << "quit            - Exit the game"<<endl;
        cout << "=====================================================\n";
    }

    // Save game state to file
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
        cout << "Game saved"<<endl;
    }

    // Load game state from file
    void load() {
        ifstream f("save.txt");
        if (!f) { cout << "No save file."<<endl; return; }

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

        cout << "Game loaded"<<endl;
        look();
    }

    // Convert string to lowercase
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
    g.start(); // Start the game
    return 0;
}
