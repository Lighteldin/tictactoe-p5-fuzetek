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
    // 3x3 grid representation
    vector<vector<char>> grid;

    // Board size constant (default: 3)
    const int size;

public:
    // Input: Optional size parameter (default 3)
    // Output: Constructs Board object
    // Function: Initializes empty grid of given size
    Board(int size = 3) : size(size) {
        // TODO: Implement this function
    }

    // Input: None
    // Output: Prints formatted board to console
    // Function: Displays current board state with coordinates and borders
    void display() const {
        // TODO: Implement this function
    }

    // Input: row (0-based), col (0-based), player symbol (X/O)
    // Output: Boolean indicating move success
    // Function: Places symbol if move is valid, returns success status
    bool makeMove(int row, int col, char symbol) {
        // TODO: Implement this function
        return false; // placeholder
    }

    // Input: row (0-based), col (0-based)
    // Output: Boolean indicating move validity
    // Function: Checks if coordinates are valid and cell is empty
    bool isValidMove(int row, int col) const {
        // TODO: Implement this function
        return false; // placeholder
    }

    // Input: Player symbol (X/O)
    // Output: Boolean indicating win condition
    // Function: Checks all win conditions (rows, columns, diagonals)
    bool checkWin(char symbol) const {
        // TODO: Implement this function
        return false; // placeholder
    }

    // Input: None
    // Output: Boolean indicating board full status
    // Function: Checks if all cells are occupied
    bool isFull() const {
        // TODO: Implement this function
        return false; // placeholder
    }

    // Input: row (0-based), col (0-based)
    // Output: Character representing cell content
    // Function: Returns the symbol at specified coordinates
    char getCell(int row, int col) const {
        // TODO: Implement this function
        return ' '; // placeholder
    }

    // Input: None
    // Output: None
    // Function: Clears all cells to empty state
    void reset() {
        // TODO: Implement this function
    }

    // Input: None
    // Output: Integer representing board size
    // Function: Returns the dimension of the board
    int getSize() const {
        // TODO: Implement this function
        return 0; // placeholder
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
        // TODO: Implement this function
    }

    virtual ~Player() {}

    // Input: References to row and column variables
    // Output: None (pure virtual)
    // Function: Abstract method to be implemented by derived classes
    virtual void getMove(int& row, int& col) = 0;

    // Input: None
    // Output: Player name string
    // Function: Returns player's name
    string getName() const {
        // TODO: Implement this function
        return ""; // placeholder
    }

    // Input: None
    // Output: Player symbol character
    // Function: Returns player's symbol
    char getSymbol() const {
        // TODO: Implement this function
        return ' '; // placeholder
    }

    // Input: New name string
    // Output: None
    // Function: Updates player's name
    void setName(const string& name) {
        // TODO: Implement this function
    }
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
    void getMove(int& row, int& col) override {
        // TODO: Implement this function (if needed)
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
        : Player(name, symbol) {
        // TODO: Implement this function
    }

    // Input: References to row and column variables
    // Output: None
    // Function: Determines AI move based on difficulty level
    void getMove(int& row, int& col) override {
        // TODO: Implement this function
    }

    // Input: New difficulty level
    // Output: None
    // Function: Changes AI difficulty setting
    void setDifficulty(Difficulty newDifficulty) {
        // TODO: Implement this function
    }

    // Input: Board reference, references to row and column variables
    // Output: None (modifies row/col references)
    // Function: Selects random valid move for easy difficulty
    void getRandomMove(const Board& board, int& row, int& col) const {
        // TODO: Implement this function
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
        // TODO: Implement this function
    }

    // Input: None
    // Output: None
    // Function: Main game entry point, controls overall flow
    void start() {
        // TODO: Implement this function
    }

    // Input: None
    // Output: None
    // Function: Displays mode selection menu and handles user choice
    void showMenu() {
        // TODO: Implement this function
    }

    // Input: None
    // Output: None
    // Function: Configures player vs player mode with user input
    void setupPvP() {
        // TODO: Implement this function
    }

    // Input: AI difficulty level
    // Output: None
    // Function: Configures player vs computer mode with user input
    void setupPvC(Difficulty difficulty) {
        // TODO: Implement this function
    }

    // Input: None
    // Output: None
    // Function: Alternates current player between players
    void switchPlayer() {
        // TODO: Implement this function
    }

    // Input: Pointer to human player
    // Output: None
    // Function: Processes human player input and validates moves
    void handleHumanMove(Player* player) {
        // TODO: Implement this function
    }

    // Input: Pointer to AI player
    // Output: None
    // Function: Executes AI move calculation and placement
    void handleAIMove(AIPlayer* aiPlayer) {
        // TODO: Implement this function
    }

    // Input: None
    // Output: Boolean indicating game termination
    // Function: Checks win conditions and board full status
    bool checkGameEnd() {
        // TODO: Implement this function
        return false; // placeholder
    }

    // Input: None
    // Output: None
    // Function: Shows game outcome message
    void displayResult() const {
        // TODO: Implement this function
    }

    // Input: None
    // Output: None
    // Function: Prepares game for new round
    void reset() {
        // TODO: Implement this function
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
