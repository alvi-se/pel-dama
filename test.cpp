#include <iostream>
#include "player.cpp"

using std::cout;
using std::endl;

int main()
{
    ifstream str("board.txt");
    Board b(str);
    b.print(cout);
    cout << endl;
    Move m {
        { 5, 6 },
        { 6, 7 }
    };
    cout << "Nuova board" << endl;
    Board newone;
    try
    {
        newone = b.applyMove(m);
        newone.print(cout);
        cout << endl;
    } catch(player_exception p)
    {
        cout << p.msg << endl;
    }
}