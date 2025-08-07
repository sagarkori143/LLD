#include<bits/stdc++.h>
using namespace std;

enum class PieceType{
    X,
    O,
    EMPTY  
};

class Piece{
private:
    PieceType PlayingPiece;
    
public:
    Piece(PieceType piece = PieceType::EMPTY) : PlayingPiece(piece) {}
    
    PieceType getPieceType() const {
        return PlayingPiece;
    }
    
    bool isEmpty() const {
        return PlayingPiece == PieceType::EMPTY;
    }
};

class PieceX: public Piece{
public:
    PieceX(): Piece(PieceType::X){}
};

class PieceO: public Piece{
public:
    PieceO(): Piece(PieceType::O){}
};

class Player{
private:
    string PlayerName;
    Piece piece;
    
public:
    Player(string name, Piece p) : PlayerName(name), piece(p) {}
    
    string getPlayerName() const {
        return PlayerName;
    }
    
    Piece getPiece() const {
        return piece;
    }
};

class Board{
private:
    int size;
    vector<vector<Piece>> board;
    
public:
    Board(int size) : size(size) {
        board.resize(size, vector<Piece>(size, Piece()));  // Initialize with empty pieces
    }

    bool addPiece(int row, int col, Piece piece){
        if(row < 0 || row >= size || col < 0 || col >= size || !board[row][col].isEmpty()){
            return false;
        }
        board[row][col] = piece;
        return true;
    }

    vector<pair<int,int>> getFreeCells(){
        vector<pair<int,int>> temp;
        for(int i=0; i<size; i++){
            for(int j=0; j<size; j++){
                if(board[i][j].isEmpty()) {
                    temp.push_back({i,j});
                }
            }
        }
        return temp;
    }
    
    void printBoard(){
        for(int i=0; i<size; i++){
            for(int j=0; j<size; j++){
                if(!board[i][j].isEmpty()) {
                    char pieceChar = (board[i][j].getPieceType() == PieceType::X) ? 'X' : 'O';
                    cout << pieceChar << " ";
                } else {
                    cout << ". ";
                }
            }
            cout << endl;
        }
    }
};


int main() {
    // Create players and board
    Player* Player1 = new Player("Sagar", PieceX());
    Player* Player2 = new Player("Omkar", PieceO());
    Board* newBoard = new Board(3);

    // Queue for alternating turns
    queue<Player*> playerQueue;
    playerQueue.push(Player1);
    playerQueue.push(Player2);

    bool isWon = false;

    while (!isWon && !newBoard->getFreeCells().empty()) {
        Player* currentPlayer = playerQueue.front();
        playerQueue.pop();

        cout << currentPlayer->getPlayerName() << "'s turn:\n";

        // Dummy move: take first available cell
        vector<pair<int,int>> freeCells = newBoard->getFreeCells();
        if (freeCells.empty()) break;

        pair<int,int> move = freeCells[0];  // Replace with user input if needed
        newBoard->addPiece(move.first, move.second, currentPlayer->getPiece());

        newBoard->printBoard();

        // TODO: Add winning logic here (row/col/diag check)

        playerQueue.push(currentPlayer);  // Push back to end of queue
    }

    cout << "Game Over\n";
    newBoard->printBoard();

    // Cleanup
    delete Player1;
    delete Player2;
    delete newBoard;
}
