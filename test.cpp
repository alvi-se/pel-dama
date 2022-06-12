#include <iostream>
#include <time.h>
#include "player.cpp"

using std::cout;
using std::endl;

int main()
{
    Board b1, b2;
    ifstream input;
    input.open("board_21.txt");
    b1 = Board{input};
    input.close();
    input.open("board_22.txt");
    b2 = Board{input};
    input.close();
    Move m = b2.extractMove(b1);
    m.print(cout);
    cout << endl;

    /*
    Player p1(1);
    Player p2(2);

    p1.init_board("board_0.txt");

    for (int i = 0; i < 100; ++i)
    {
        if (i % 2 == 0)
        {
            p1.load_board("board_" + std::to_string(i) + ".txt");
            p1.move();
            p1.store_board("board_" + std::to_string(i + 1) + ".txt");
        }
        else
        {
            p2.load_board("board_" + std::to_string(i) + ".txt");
            p2.move();
            p2.store_board("board_" + std::to_string(i + 1) + ".txt");
        }
    }
    */
    /*
    Player p2(2);
    p2.load_board("board_1.txt");
    p2.move();
    */
}