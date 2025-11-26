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
    
    bool useItem(string item) {
        if (items.empty()) {
            cout << "Inventory is empty.\n";
            return false;
        }
        stack<string> temp;
        bool found = false;
        while (!items.empty()) {
            string top = items.top();
            items.pop();
    
    
    
    
    
    
    
    
    
  class Game{
  	
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
  	
  } 
    
    
    