#include <iostream>
#include "player.cpp"
#include <vector>

using std::cout;
using std::endl;

int main()
{
    ifstream file{ "board.txt" };
    Board b{ file };
    file.close();
    Position p{ 5, 6 };
    List<Move> moves = b.getPossibleMoves(p);
    for (Move m : moves)
    {
        m.from.print(cout);
        cout << " -> ";
        m.to.print(cout);
        cout << endl;
    }
}