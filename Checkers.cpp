#include <iostream>
#include <cstdlib>
#include <string>
#include <sstream>
using namespace std;

string poslist[30];

char board[8][8];
int counter=0;
bool newKing = false;

bool validSelection(int, int, char);
int Yinput(char);
char Youtput(int);

class Piece
{
private:
    char Player;
public:
    int LocationX;
    int LocationY;
    bool IsKing;

    Piece(char P, int LX, int LY, bool King) : Player(P), LocationX(LX), LocationY(LY), IsKing(King) {}
    Piece(char P, int LX, int LY) : Player(P), LocationX(LX), LocationY(LY) {}
    Piece() {}


    char getPlayer() const
    {
        return Player;
    }

    int getLocationX() const
    {
        return LocationX;
    }

    int getLocationY() const
    {
        return LocationY;
    }

    bool getIsKing() const
    {
        return IsKing;
    }


    void setBoard()
    {
        if(IsKing){
            board[LocationX][LocationY] = 'K';
        }
        else{
            board[LocationX][LocationY] = Player;
        }
    }
    void delPiece()
    {
        Player = ' ';
        setBoard();
    }
    int space(int x, int y) {

        if (board[LocationX][LocationY] == ' ' ) { return 0; }
        if (board[LocationX][LocationY] != Player && board[LocationX][LocationY] != 'K' ) { return 0; }
        if (x < 0 || x > 7 || y < 0 || y > 7 || board[x][y] != ' ')
        {
            return 0;
        }
        stringstream ss;
        ss << Youtput(LocationY + 1) << LocationX + 1 << Youtput(y + 1) << x + 1;
        string pos = ss.str();
        for (int i = 0; i < 30; i++) {
            if (i == counter) {
                i++;
            }
            if (poslist[i] == pos){
                return 0;
            }
        }
        poslist[counter] = pos;
        counter++;
        cout << Player << ": " << Youtput(LocationY + 1) << " " << LocationX + 1 << " ---> " << Youtput(y + 1) << " " << x + 1 << endl;
        return 1;
    }

    int jumpC(int x, int y) {
        if (board[LocationX][LocationY] == ' ') { return 0;}
        if (board[LocationX][LocationY] != Player && board[LocationX][LocationY] != 'K' ){return 0;}
        if (newKing){return 0;}
        if (x < 0 || x > 7 || y < 0 || y > 7 || board[x][y] != ' ') { return 0; }

        int midX = (LocationX + x) / 2;
        int midY = (LocationY + y) / 2;
        if (board[midX][midY] == ' ' || board[midX][midY] == Player)
        {
            return 0;
        }
        
        cout << "Jump is possible at ---> " << Youtput(y + 1) << " " << x + 1 << endl;
        return 1;
    }
    bool validateMove(int x, int y, int turn, char piece)
    {
        //Checking in range or if move is on top of another piece
        if (x < 0 || x > 7 || y < 0 || y > 7 || board[x][y] != ' ')
        {
            cout << "invalid move, move coordinate points onto another piece or outside of board..." << endl;
            return true;
        }
        // cout<<LocationX<<" "<<x<<" "<<LocationY<<" "<<y<<endl;


        int deltaX = abs(LocationX - x);
        int deltaY = abs(LocationY - y);

        // check if move is a jump
        if (deltaX > 2) {
            cout << "invalid move, a jump can only be over one row..." << endl;
            return true;
        }

        // check if move is diagonal
        if (deltaX != deltaY)
        {
            cout << "invalid move, move is not diagonal..." << endl;
            return true;
        }

        // check if move is valid for a king
        if (IsKing)
        {
            return false;
        }
        if (board[LocationX][LocationY] == 'K'){
            return false;
        }

        // check if move is valid for a regular piece -- move's only forward

        if (Player == piece)
        {
            if (turn % 2 == 1) {
                if (x < LocationX)
                {
                    cout << "invalid move, the piece can not move backwards!" << endl;
                    return true;
                }
            }
            else {
                if (x > LocationX)
                {
                    cout << "invalid move, the piece can not move backwards!" << endl;
                    return true;
                }
            }
        }


        if (deltaX == 2 && deltaY == 2)
        {
            // check if there is an opponent's piece in between
            int midX = (LocationX + x) / 2;
            int midY = (LocationY + y) / 2;
            if (board[midX][midY] == ' ' || board[midX][midY] == Player)
            {
                cout << "invalid jump, there isnt an opponent's piece between the jump!! (" << board[midX][midY] << ")" << endl;
                return true;
            }

        }

        cout << "Move is valid" << endl;
        return false;
    }

    void takeMove(int A, int B)
    {

        board[LocationX][LocationY] = ' ';

        LocationX = A;
        LocationY = B;

        // check if piece should be promoted to king
        if (LocationX == 0 || LocationX == 7)
        {     
            if (IsKing == false){
                newKing = true;
                IsKing = true;
            }     
            
        }
        setBoard();
    }
};

//bool MakeMove(int, int, const Piece *, char);
int PosMoves(int, int, Piece*, Piece*);
int Exist(int, int, const Piece*);
void delPiece(Piece);
void Jump(int, int, int, Piece*, Piece*);

void in_Board()
{

    for (int i = 0; i < 8; i++)
    {
        bool freeRow = (i == 3 || i == 4);

        for (int j = 0; j < 8; j++)
        {
            if (freeRow)
            {
                board[i][j] = ' ';
            }
            else if ((i % 2 == 1 && j % 2 == 0) || (i % 2 == 0 && j % 2 == 1)) {}
            else
            {
                board[i][j] = ' ';
            }
        }
    }
}
void printBoard()
{
    char rowLabel = '1';

    cout << endl;
    cout << "  +---+---+---+---+---+---+---+---+" << endl;

    for (int i = 0; i < 8; i++)
    {
        cout << rowLabel << " | ";
        for (int j = 0; j < 8; j++)
        {
            cout << board[i][j] << " | ";
        }
        cout << endl;
        cout << "  +---+---+---+---+---+---+---+---+" << endl;
        rowLabel++;
    }
    cout << "    A   B   C   D   E   F   G   H" << endl;
}

bool MakeMove(int index, int turn, Piece* Player, char piece, Piece* Player2)
{
    int X, Y;
    char Yc;
    bool status;

    cout << "Player " << turn << " please enter move coordinates: ";
    cin >> Yc >> X;
    Y = Yinput(Yc);
    X--;
    Y--;
    status = Player[index].validateMove(X, Y, turn, piece);

    if (status)
    {
        return true;
    }

    //checking for jump 
    Jump(X, Y, index, Player, Player2);
    
    //Placing new positions of pieces
    Player[index].takeMove(X, Y);
    return false;

}
int main()
{

    int Side = 0, turn = 0;
    char piece, p1, p2;
    in_Board();

    // Player1 Piece Objects and instances declaration
    cout << "Player 1, please enter a character for your pieces: ";
    cin >> p1;
    Piece Player1[12];
    piece = p1;
    int index = 0;
    for (int i = Side; i < 3; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if ((i % 2 == 1 && j % 2 == 0) || (i % 2 == 0 && j % 2 == 1))
            {
                Player1[index] = Piece(piece, i, j, false);
                index++;
            }
        }
    }

    // Player2 Piece Objects and instances declaration
    Side = 5;
    cout << "Player 2, please enter a character for your pieces: ";
    cin >> p2;
    Piece Player2[12];
    piece = p2;
    index = 0;
    for (int i = Side; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if ((i % 2 == 1 && j % 2 == 0) || (i % 2 == 0 && j % 2 == 1))
            {
                Player2[index] = Piece(piece, i, j, false);
                index++;
            }
        }
    }

    // Piece arragment on board
    for (int i = 0; i < 12; i++)
    {
        Player1[i].setBoard();
        Player2[i].setBoard();
    }



    // Selection
    int X, Y;
    char Yc;
    bool Repeat;
    int isJump = 0;
    do
    {

        do
        {
            if (turn % 2 == 0)
            {
                piece = p1; isJump = PosMoves((turn % 2) + 1, isJump, Player1, Player2);
            }
            else { piece = p2; isJump = PosMoves((turn % 2) + 1, isJump, Player2, Player1); }

            if (isJump == 2) {

                //cout << "Is Jump is 2"; cin >> isJump;
            }
            else if (isJump == 3) {
                fill(poslist, poslist + 30, ' ');
                counter = 0;
                newKing = false;
                turn++;
            }
            else if (isJump == 1) {
                return 0;
            }
            // Displaying board
            printBoard();
        } while (isJump == 2 || isJump == 3);



        do
        {
            do
            {
                cout << "Player " << (turn % 2) + 1 << " please select your piece coordinates: ";
                cin >> Yc >> X;
                Y = Yinput(Yc);
                X--;
                Y--;
            } while (validSelection(X, Y, piece));

            if (turn % 2 == 0)
            {
                index = Exist(X, Y, Player1);
                Repeat = MakeMove(index, (turn % 2) + 1, Player1, piece, Player2);
            }
            else
            {
                index = Exist(X, Y, Player2);
                Repeat = MakeMove(index, (turn % 2) + 1, Player2, piece, Player1);
            }

            // Displaying board
            printBoard();

        } while (Repeat);

        fill(poslist, poslist+30, ' ');
        counter = 0;
        newKing = false;
        turn++;
    } while (true);

    return 0;
}

int Yinput(char X) {
    switch (X)
    {
    case 'A': return 1;
    case 'B': return 2;
    case 'C': return 3;
    case 'D': return 4;
    case 'E': return 5;
    case 'F': return 6;
    case 'G': return 7;
    case 'H': return 8;

    default:
        return 9;
        break;
    }
}
char Youtput(int X) {
    switch (X)
    {

    case 1: return 'A';
    case 2: return 'B';
    case 3: return 'C';
    case 4: return 'D';
    case 5: return 'E';
    case 6: return 'F';
    case 7: return 'G';
    case 8: return 'H';
    default:return 'X';

    }
}
bool validSelection(int X, int Y, char piece)
{
    if (X < 0 || X > 7 || Y < 0 || Y > 7 || board[X][Y] == ' '||  board[X][Y] != piece)
    {
        if (board[X][Y] == 'K'){
            return false;
        }
        cout << "invalid selection try again...(" << board[X][Y] << " )" << endl;
        return true;
    }
    return false;
}

int Exist(int X, int Y, const Piece* Player)
{
    for (int i = 0; i < 12; i++)
    {
        //cout << Player[i].LocationX << " " << Player[i].LocationY << endl;
        if (Player[i].LocationX == X && Player[i].LocationY == Y)
        {

            cout << "Piece found at: " << Youtput(Player[i].LocationY + 1) << " " << Player[i].LocationX + 1 << endl;
            return i;
        }
    }
    cout << " Piece Not found... " << endl;
    return 1;
}
void delPiece(Piece Player) {
    //cout << "before: " << Player.getPlayer() << endl;
    Player.IsKing = false;
    Player.delPiece();
    Player.LocationX = 0;
    Player.LocationY = 0;
    cout << "Jumped piece has been captured and removed..." << endl;
    //cout << "after: " << Player.getPlayer() << endl;


}
void Jump(int x, int y, int index, Piece* player, Piece* player2) {
    int deltaX = abs(player[index].LocationX - x);
    int deltaY = abs(player[index].LocationY - y);

    if (deltaX == 2) {
        int midX = (player[index].LocationX + x) / 2;
        int midY = (player[index].LocationY + y) / 2;
        //cout << midY << " " << midX << endl;
        cout << "Jump ";
        int Nindex = Exist(midX, midY, player2);
        delPiece(player2[Nindex]);
    }
    else {
        cout << "No Capture made...";
    }
}

int PosMoves(int turn, int Pjump, Piece* Player, Piece* Player2) {
    int x = 2, moves = 0, Jmoves = 0;
    bool Jumps = false;
    char Q;
    if (turn == 2) { x = -2; }
    for (int i = 0; i < 12; i++)
    {
        Jmoves = Player[i].jumpC(Player[i].LocationX + x, Player[i].LocationY + 2);
        if (Jmoves != 0) {
            cout << "Player " << turn << " You must make a jump!" << endl << "Do you want to make this Jump? y/n: "; cin >> Q;
            if (Q == 'y') {
                Jump(Player[i].LocationX + x, Player[i].LocationY + 2, i, Player, Player2);
                Player[i].takeMove(Player[i].LocationX + x, Player[i].LocationY + 2);
                return 2;
            }
            Jumps = true;
            Jmoves = 0;
        }
        Jmoves = Player[i].jumpC(Player[i].LocationX + x, Player[i].LocationY - 2);
        if (Jmoves != 0) {
            cout << "Player " << turn << " You must make a jump!" << endl << "Do you want to make this Jump? y/n: "; cin >> Q;
            if (Q == 'y') {
                Jump(Player[i].LocationX + x, Player[i].LocationY - 2, i, Player, Player2);
                Player[i].takeMove(Player[i].LocationX + x, Player[i].LocationY - 2);
                return 2;
            }
            Jumps = true;
            Jmoves = 0;
        }

        //King Jumps
        if (Player[i].IsKing) {
            Jmoves = Player[i].jumpC(Player[i].LocationX - x, Player[i].LocationY + 2);
            if (Jmoves != 0) {
                cout << "Player " << turn << " You must make a jump!" << endl << "Do you want to make this Jump? y/n: "; cin >> Q;
                if (Q == 'y') {
                    Jump(Player[i].LocationX - x, Player[i].LocationY + 2, i, Player, Player2);
                    Player[i].takeMove(Player[i].LocationX - x, Player[i].LocationY + 2);
                    return 2;
                }
                Jumps = true;
                Jmoves = 0;
            }
            Jmoves = Player[i].jumpC(Player[i].LocationX - x, Player[i].LocationY - 2);
            if (Jmoves != 0) {
                cout << "Player " << turn << " You must make a jump!" << endl << "Do you want to make this Jump? y/n: "; cin >> Q;
                if (Q == 'y') {
                    Jump(Player[i].LocationX - x, Player[i].LocationY - 2, i, Player, Player2);
                    Player[i].takeMove(Player[i].LocationX - x, Player[i].LocationY - 2);
                    return 2;
                }
                Jumps = true;
                Jmoves = 0;
            }

        }


    }
    if (Jumps) {
        return 2;
    }
    if (Pjump == 2) {
        return 3;
    }
    if (turn == 2) { x++; }
    else { x--; }

    // for (int i = 0; i < 12; i++)
    // {
    //    cout<< Player[i].getPlayer()<<endl;
    // }

    // //Regular moves
    // cout<<" ---------- "<<endl;
    for (int i = 0; i < 12; i++)
    {
        moves += Player[i].space(Player[i].LocationX + x, Player[i].LocationY + 1); //up right
        moves += Player[i].space(Player[i].LocationX + x, Player[i].LocationY - 1); //up left

        //cout<< Player[i].getPlayer()<<endl;
        //cout<<Player[i].LocationX + x<<" " << Player[i].LocationY +1<<endl;
        //cout<<Player[i].LocationX + x<<" " << Player[i].LocationY -1<<endl;
    }

    cout << "King only moves: " << endl;
    for (int i = 0; i < 12; i++)
    { 
        if (Player[i].IsKing)
        {
            moves += Player[i].space(Player[i].LocationX - x, Player[i].LocationY + 1); //down right
            moves += Player[i].space(Player[i].LocationX - x, Player[i].LocationY - 1); //down left
        }
    
    } 
   


    cout << "Player " << turn << " You have " << moves << " possible moves..." << endl;
    if (moves == 0) {
        cout << " Player " << turn << " You have lost!!" << endl << "Congratulations Player " << turn << endl;
        return 1;
    }
    return 0;

}
