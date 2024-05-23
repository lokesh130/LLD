#include<bits/stdc++.h> 
using namespace std; 

class Command {
    public:
        virtual void execute() = 0;
};


class HelpCommand : public Command {
    public: 
        void execute() override {
            cout<<"Available commands: help, exit"<<endl;
        }
};

class ExitCommand : public Command {
    public: 
        void execute() override {
            cout<<"exiting from the terminal..."<<endl;
            exit(0);
        }
};


class CommandFactory {
    private:
        unordered_map<string, Command*> commandMap; 
        void registerCommand(string commandName, Command* command) {
            commandMap.insert({commandName, command});
        }

    public:
        CommandFactory() {
            registerCommand("help", new HelpCommand());
            registerCommand("exit", new ExitCommand());
        }

        Command* getCommand(string commandName) {
            auto itr = commandMap.find(commandName);
            if(itr != commandMap.end()) {
                return itr->second;
            } else {
                return nullptr;
            }
        }
};


class Terminal {
    private: 
        CommandFactory* commandFactory; 

    public:
        Terminal() {
            this->commandFactory = new CommandFactory;
        }

        void run() {
            while(true) {
                string input; 
                cout<<"> ";
                getline(cin, input); 
                Command* command = commandFactory->getCommand(input);
                if(command != nullptr) {
                    command->execute();
                } else {
                    cout<<"No such command : "<<input<<endl;
                }
            }
        }
};


int main() { 
    Terminal* terminal = new Terminal(); 
    terminal->run();

    return 0;
}