#include <iostream>
#include <vector>
#include <map>
#include <ctime>
#include <cstdlib>

using namespace std;

class Player {
    string name;
    int position;
public:
    Player(string name) : name(name), position(0) {}
    string getName() { return name; }
    int getPosition() { return position; }
    void setPosition(int pos) { position = pos; }
};

class MoveStrategy {
public:
    virtual int move(int position) = 0;
};

class NormalMove : public MoveStrategy {
public:
    int move(int position) override {
        return position;
    }
};

class SnakeBite : public MoveStrategy {
    int end;
public:
    SnakeBite(int end) : end(end) {}
    int move(int position) override {
        cout << "Snake bite! Moving down to " << end << endl;
        return end;
    }
};

class LadderClimb : public MoveStrategy {
    int end;
public:
    LadderClimb(int end) : end(end) {}
    int move(int position) override {
        cout << "Ladder climb! Moving up to " << end << endl;
        return end;
    }
};

class GameBoard {
    map<int, MoveStrategy*> board;
public:
    GameBoard() {
        // Example snakes and ladders
        board[16] = new SnakeBite(6);
        board[47] = new SnakeBite(26);
        board[98] = new SnakeBite(78);
        board[1] = new LadderClimb(38);
        board[4] = new LadderClimb(14);
        board[9] = new LadderClimb(31); 
    }

    MoveStrategy* getMoveStrategy(int position) {
        if (board.find(position) != board.end())
            return board[position];
        return new NormalMove();
    }
};

class Game {
    GameBoard* gameBoard;
    vector<Player*> players;
    int currentPlayerIndex;
    
    static Game* instance;

    Game() : gameBoard(new GameBoard()), currentPlayerIndex(0) {}

public:
    static Game* getInstance() {
        if (!instance)
            instance = new Game();
        return instance;
    }

    void addPlayer(Player* player) {
        players.push_back(player);
    }

    void play() {
        while (true) {
            Player* currentPlayer = players[currentPlayerIndex];
            int diceRoll = (rand() % 6) + 1;
            cout << currentPlayer->getName() << " rolled a " << diceRoll << endl;

            int newPosition = currentPlayer->getPosition() + diceRoll;
            if (newPosition > 100) {
                cout << currentPlayer->getName() << " needs to roll the exact number to reach 100." << endl;
            } else if (newPosition == 100) {
                cout << currentPlayer->getName() << " wins!" << endl;
                break;
            } else {
                MoveStrategy* moveStrategy = gameBoard->getMoveStrategy(newPosition);
                newPosition = moveStrategy->move(newPosition);
                currentPlayer->setPosition(newPosition);
                cout << currentPlayer->getName() << " is now at position " << newPosition << endl;
            }
            currentPlayerIndex = (currentPlayerIndex + 1) % players.size();
        }
    }
};

Game* Game::instance = nullptr;

int main() {
    srand(time(0));
    
    Game* game = Game::getInstance();
    game->addPlayer(new Player("Player 1"));
    game->addPlayer(new Player("Player 2"));

    game->play();
    return 0;
}
