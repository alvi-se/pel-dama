#include <iostream>
#include "player.cpp"

using std::cout;
using std::endl;

int main()
{
    ifstream file{ "board_1.txt" };
    Board b{ file };
    Position p{ 2, 3 };
    List<Move> moves = b.getPossibleMoves(p);
    for (Move m : moves)
    {
        m.from.print(cout);
        cout << ' ';
        m.to.print(cout);
        cout << endl;
    }
}