#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <climits>
using namespace std;

// AI difficulty levels
// Needed because AIPlayer's constructor and Game::setupPvC() both take a
// Difficulty parameter, and the menu offers "Easy" and "Hard" AI modes.
enum class Difficulty {
    EASY,
    HARD
};

// ============================================================
// Board Class
// Responsibilities: Manage game board state and validation
// ============================================================
class Board {
private:
    vector<vector<char>> grid;
    const int size;

public:
    Board(int size = 3) : size(size) {
        grid = vector<vector<char>>(size, vector<char>(size, ' '));
    }

    void display() const {
        cout << " ";
        for (int j = 0; j < size; j++) {
            cout << " " << (j + 1);
        }
        cout << "\n";

        for (int i = 0; i < size; i++) {
            cout << (i + 1);

            for (int j = 0; j < size; j++) {
                cout << " " << grid[i][j];

                if (j != size - 1)
                    cout << " |";
            }

            cout << "\n";

            if (i != size - 1) {
                for (int j = 0; j < size; j++) {
                    cout << "---";

                    if (j != size - 1)
                        cout << "+";
                }
                cout << "\n";
            }
        }
    }
    // Input: row (0-based), col (0-based), player symbol (X/O)
    // Output: Boolean indicating move success
    // Function: Places symbol if move is valid, returns success status
   bool makeMove(int row, int col, char symbol) {
    if (!isValidMove(row, col)) {
        return false;
    }
    grid[row][col] = symbol;
    return true;
}

    // Input: row (0-based), col (0-based)
    // Output: Boolean indicating move validity
    // Function: Checks if coordinates are valid and cell is empty
bool isValidMove(int row, int col) const {
    if (row < 0 || row >= size || col < 0 || col >= size) {
        return false;
    }
    return grid[row][col] == ' ';
}

    // Input: Player symbol (X/O)
    // Output: Boolean indicating win condition
    // Function: Checks all win conditions (rows, columns, diagonals)
    bool checkWin(char symbol) const {
        // Check all rows
        for (int i = 0; i < size; i++) {
            bool rowWin = true;
            for (int j = 0; j < size; j++) {
                if (grid[i][j] != symbol) {
                    rowWin = false;
                }
            }
            if (rowWin == true) {
                return true;
            }
        }

        // Check all columns
        for (int j = 0; j < size; j++) {
            bool colWin = true;
            for (int i = 0; i < size; i++) {
                if (grid[i][j] != symbol) {
                    colWin = false;
                }
            }
            if (colWin == true) {
                return true;
            }
        }

        // Check top-left to bottom-right diagonal
        bool diag1Win = true;
        for (int i = 0; i < size; i++) {
            if (grid[i][i] != symbol) {
                diag1Win = false;
            }
        }
        if (diag1Win == true) {
            return true;
        }

        // Check top-right to bottom-left diagonal
        bool diag2Win = true;
        for (int i = 0; i < size; i++) {
            if (grid[i][size - 1 - i] != symbol) {
                diag2Win = false;
            }
        }
        if (diag2Win == true) {
            return true;
        }

        // If no win was found
        return false;
    }

    // Input: None
    // Output: Boolean indicating board full status
    // Function: Checks if all cells are occupied
    bool isFull() const {
        // Check every cell in the grid
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                if (grid[i][j] == ' ') {
                    // Found an empty space, so the board is not full
                    return false;
                }
            }
        }
        // If the loops finish and no empty spaces were found, the board is full
        return true;
    }

    // Input: row (0-based), col (0-based)
    // Output: Character representing cell content
    // Function: Returns the symbol at specified coordinates
   char getCell(int row, int col) const {
    return grid[row][col];
}

    // Input: None
    // Output: None
    // Function: Clears all cells to empty state
    void reset() {
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                grid[i][j] = ' ';
            }
        }
    }

    int getSize() const {
        return size;
    }
};

// ============================================================
// Player Class (Abstract)
// Responsibilities: Base class for player entities
// ============================================================
class Player {
protected:
    string name;
    char symbol;

public:
    // Input: Player name string, symbol character
    // Output: Constructs Player object
    // Function: Initializes player with name and symbol
 Player(const string& name, char symbol) {
    this->name = name;
    this->symbol = symbol;
}
string getName() const {
    return name;
}
char getSymbol() const {
    return symbol;
}
void setName(const string& name) {
    this->name = name;
}

    virtual ~Player() {}

    // Input: References to row and column variables
    // Output: None (pure virtual)
    // Function: Abstract method to be implemented by derived classes
    virtual void getMove(int& row, int& col, const Board& board) = 0;

   
};

// ============================================================
// HumanPlayer Class (Derived from Player)
// Responsibilities: Concrete, instantiable player driven by console input.
// Required because Player is abstract (pure virtual getMove) and PvP mode
// needs two concrete human players; Game::handleHumanMove(Player*) is the
// method that actually collects the move via console input, so this class
// simply forwards getMove to that flow.
// ============================================================
class HumanPlayer : public Player {
public:
    HumanPlayer(const string& name, char symbol) : Player(name, symbol) {}

    // Input: References to row and column variables
    // Output: None
    // Function: Not used directly — Game::handleHumanMove() collects and
    // validates console input for human players. Kept as a no-op override
    // so HumanPlayer is a valid, instantiable concrete class.
   void getMove(int& row, int& col, const Board& board) override {
    // Intentionally empty — Game::handleHumanMove() handles human input directly.
}
};

// ============================================================
// AIPlayer Class (Derived from Player)
// Responsibilities: Computer opponent implementation
// ============================================================
class AIPlayer : public Player {
private:
    Difficulty difficulty;

    int minimax(Board board, bool isMaximizing) const {
        char mySymbol = getSymbol();
        char oppSymbol = (mySymbol == 'X') ? 'O' : 'X';

        // Terminal state check — stop recursing and score the board
        if (board.checkWin(mySymbol) || board.checkWin(oppSymbol) || board.isFull()) {
            return evaluateBoard(board);
        }

        int size = board.getSize();

        if (isMaximizing) {
            int bestScore = INT_MIN;
            for (int r = 0; r < size; r++) {
                for (int c = 0; c < size; c++) {
                    if (board.isValidMove(r, c)) {
                        Board simulated = board;
                        simulated.makeMove(r, c, mySymbol);
                        int score = minimax(simulated, false);
                        bestScore = max(bestScore, score);
                    }
                }
            }
            return bestScore;
        } else {
            int bestScore = INT_MAX;
            for (int r = 0; r < size; r++) {
                for (int c = 0; c < size; c++) {
                    if (board.isValidMove(r, c)) {
                        Board simulated = board;
                        simulated.makeMove(r, c, oppSymbol);
                        int score = minimax(simulated, true);
                        bestScore = min(bestScore, score);
                    }
                }
            }
            return bestScore;
        }
    }


public:
    // Input: AI name, symbol, difficulty level
    // Output: Constructs AIPlayer object
    // Function: Initializes AI player with specified parameters
   AIPlayer(const string& name, char symbol, Difficulty difficulty)
    : Player(name, symbol), difficulty(difficulty) {
}

    // Input: References to row and column variables
    // Output: None
    // Function: Determines AI move based on difficulty level
  void getMove(int& row, int& col, const Board& board) override {
    if (difficulty == Difficulty::EASY) {
        getRandomMove(board, row, col);
    } else {
        getBestMove(board, row, col);
    }
}

    // Input: New difficulty level
    // Output: None
    // Function: Changes AI difficulty setting
  void setDifficulty(Difficulty newDifficulty) {
    difficulty = newDifficulty;
}

    // Input: Board reference, references to row and column variables
    // Output: None (modifies row/col references)
    // Function: Selects random valid move for easy difficulty
  void getRandomMove(const Board& board, int& row, int& col) const {
    vector<pair<int, int>> validMoves;
    int size = board.getSize();
    for (int r = 0; r < size; r++) {
        for (int c = 0; c < size; c++) {
            if (board.isValidMove(r, c)) {
                validMoves.push_back({r, c});
            }
        }
    }
    int index = rand() % validMoves.size();
    row = validMoves[index].first;
    col = validMoves[index].second;
}

    // Input: Board reference, references to row and column variables
    // Output: None (modifies row/col references)
    // Function: Finds optimal move (research minimax algorithm for implementation)
    void getBestMove(const Board& board, int& row, int& col) const {
        int bestScore = INT_MIN;
        int bestRow = -1;
        int bestCol = -1;
        int size = board.getSize();

        for (int r = 0; r < size; r++) {
            for (int c = 0; c < size; c++) {
                if (board.isValidMove(r, c)) {
                    Board simulated = board;
                    simulated.makeMove(r, c, getSymbol());
                    int score = minimax(simulated, false); // opponent moves next

                    if (score > bestScore) {
                        bestScore = score;
                        bestRow = r;
                        bestCol = c;
                    }
                }
            }
        }

        row = bestRow;
        col = bestCol;
    }

    // Input: Board reference
    // Output: Integer score (-10, 0, +10)
    // Function: Evaluates board state for scoring (win/loss/draw)
    int evaluateBoard(const Board& board) const {
        char mySymbol = getSymbol();
        char oppSymbol = (mySymbol == 'X') ? 'O' : 'X';

        if (board.checkWin(mySymbol)) {
            return 10;
        }
        if (board.checkWin(oppSymbol)) {
            return -10;
        }
        return 0;
    }
};

// ============================================================
// Game Class
// Responsibilities: Manage game flow and coordination
// ============================================================
class Game {
private:
    Board board;
    Player* player1;
    Player* player2;
    Player* currentPlayer;

public:
    // Input: None
    // Output: Constructs Game object
    // Function: Initializes game with empty board and null players
    Game() : player1(nullptr), player2(nullptr), currentPlayer(nullptr) {
        ////////
    }

    // Input: None
    // Output: None
    // Function: Main game entry point, controls overall flow
    void start() {
        // TODO: Implement this function
        bool playAgain = true;
        while (playAgain) {
            showMenu();

            bool gameOver = false;
            while (!gameOver) {
                board.display();

                AIPlayer* ai = dynamic_cast<AIPlayer*>(currentPlayer);
                if (ai != nullptr) {
                    handleAIMove(ai);
                } else {
                    handleHumanMove(currentPlayer);
                }

                if (checkGameEnd()) {
                    board.display();
                    displayResult();
                    gameOver = true;
                } else {
                    switchPlayer();
                }
            }

            cout << "Do you want to play again? (y/n): ";
            char choice;
            cin >> choice;
            if (choice == 'y' || choice == 'Y') {
                reset();
            } else {
                playAgain = false;
            }
        }
    }

    // Input: None
    // Output: None
    // Function: Displays mode selection menu and handles user choice
    void showMenu() {
        int choice = -1;
        while (true) {
            cout << "TIC-TAC-TOE GAME\n";
            cout << "===================\n";
            cout << "1. Player vs Player\n";
            cout << "2. Player vs Computer (Easy)\n";
            cout << "3. Player vs Computer (Hard)\n";
            cout << "4. Exit\n";
            cout << "Select game mode: ";
            string input;
            cin >> input;
            bool isNumeric = !input.empty() && all_of(input.begin(), input.end(), ::isdigit);
            if (isNumeric) {
                choice = stoi(input);
            }
            if (!isNumeric || choice < 1 || choice > 4) {
                cout << "Invalid selection. Please choose 1-4.\n";
                continue;
            }
            break;
        }
        switch (choice) {
            case 1:
                setupPvP();
                break;
            case 2:
                setupPvC(Difficulty::EASY);
                break;
            case 3:
                setupPvC(Difficulty::HARD);
                break;
            case 4:
                cout << "Thanks for playing! Goodbye.\n";
                delete player1;
                delete player2;
                exit(0);
        }
    }

    // Input: None
    // Output: None
    // Function: Configures player vs player mode with user input
    void setupPvP() {
        delete player1;
        delete player2;
        string name1, name2;
        cout << "Enter name for Player 1 (X): ";
        cin >> name1;
        cout << "Enter name for Player 2 (O): ";
        cin >> name2;
        player1 = new HumanPlayer(name1, 'X');
        player2 = new HumanPlayer(name2, 'O');
        currentPlayer = player1;
    }

    // Input: AI difficulty level
    // Output: None
    // Function: Configures player vs computer mode with user input
    void setupPvC(Difficulty difficulty) {
        delete player1;
        delete player2;
        string name;
        cout << "Enter your name: ";
        cin >> name;
        player1 = new HumanPlayer(name, 'X');
        player2 = new AIPlayer("Computer", 'O', difficulty);
        currentPlayer = player1;
    }

    // Input: None
    // Output: None
    // Function: Alternates current player between players
    void switchPlayer() {
        currentPlayer = (currentPlayer == player1) ? player2 : player1;
    }

    // Input: Pointer to human player
    // Output: None
    // Function: Processes human player input and validates moves
    void handleHumanMove(Player* player) {
        // TODO: Implement this function
        int row, col;
        while (true) {
            cout << player->getName() << " (" << player->getSymbol()
                 << "), enter row and column (1-" << board.getSize() << "): ";
            if (!(cin >> row >> col)) {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "Invalid input. Please enter numbers.\n";
                continue;
            }

            int zeroRow = row - 1;
            int zeroCol = col - 1;

            if (board.isValidMove(zeroRow, zeroCol)) {
                board.makeMove(zeroRow, zeroCol, player->getSymbol());
                break;
            } else {
                cout << "Invalid move! Cell is either occupied or out of range. Try again.\n";
            }
        }
    }

    // Input: Pointer to AI player
    // Output: None
    // Function: Executes AI move calculation and placement
    void handleAIMove(AIPlayer* aiPlayer) {
        // TODO: Implement this function
        int row = -1, col = -1;
        cout << aiPlayer->getName() << " is calculating move...\n";
        aiPlayer->getMove(row, col, board);
        board.makeMove(row, col, aiPlayer->getSymbol());
    }

    // Input: None
    // Output: Boolean indicating game termination
    // Function: Checks win conditions and board full status
    bool checkGameEnd() {
        // TODO: Implement this function
        if (currentPlayer != nullptr && board.checkWin(currentPlayer->getSymbol())) {
            return true;
        }
        if (board.isFull()) {
            return true;
        }
        return false; // placeholder
    }

    // Input: None
    // Output: None
    // Function: Shows game outcome message
    void displayResult() const {
        // TODO: Implement this function
        if (currentPlayer != nullptr && board.checkWin(currentPlayer->getSymbol())) {
            cout << "Congratulations! Player " << currentPlayer->getName() << " wins!\n";
        } else if (board.isFull()) {
            cout << "The game ended in a draw!\n";
        }
    }

    // Input: None
    // Output: None
    // Function: Prepares game for new round
    void reset() {
        // TODO: Implement this function
        board.reset();
        delete player1;
        delete player2;
        player1 = nullptr;
        player2 = nullptr;
        currentPlayer = nullptr;
    }
};

// ============================================================
// Main function
// ============================================================
int main() {
    srand(static_cast<unsigned int>(time(nullptr)));

    Game game;
    game.start();

    return 0;
}
