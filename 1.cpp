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

class Inventory {
private:
    stack<string> items;
public:
    void addItem(string item) {
        items.push(item);
        cout << "Added '" << item << "' to inventory.\n";
    }
    string top = items.top();
            items.pop();
            if (top == item) {
                found = true;
                cout << "Used '" << item << "'.\n";
                break;
            }
            temp.push(top);
        }
        while (!temp.empty()) { items.push(temp.top()); temp.pop(); }
        if (!found) cout << "Item not found.\n";
        return found;
    }   
    
    
    
    
    
class GameLevel {
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

    string lower(string s) {
        transform(s.begin(), s.end(), s.begin(), ::tolower);
        return s;
    }
};
    
    
    
    
  class Game{
  	
  	private:
  		Gamelevel* level;
  		Inventory inv;
  		int levelNum;
  		int health;
  		
  	public:
	  Game() : levelNum(1) , health(3){
	  	srand(time(0));
	  	level= new Gamelevel(levelNum);
	  }	
  	
  	void start(){
  		cout<<"Riphah International University"<<endl;
  		cout<<"Escape Puzzle Game"<<endl;
  		cout<<"Team members: Areeba noor , Amna luqman ,Bisma Amin ,Arooj fatima"<<endl;
  		cout<<"Supervisor: Saleha Nasim"<<endl;
  		
  		cout<<"=================================="<<endl;
  		
  		cout<<"Type 'help' to see avaible commands"<<endl;
  		help();	
  		
  		string cmd;
  		while(true){
  			if(health<=0){
  				cout<<"health zero.Game Over"<<endl;
  				return;
			  }
			  cout<<"Health"<<health<<endl;
			  cout<<"["<<level->currentRoom<<"]>";
			  getline(cin,cmd);
			  if(!process(cmd))
			  break;
		  }
	  }
	   bool process(string c) {
        string s = lower(c);

        if (s == "quit") return false;
        if (s == "help") help();
        
        
  	
  } 
    
    
    