#include <iostream>
#include <time.h>
#include "player.cpp"

using std::cout;
using std::endl;

int main()
{
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

    /*
    Player p2(2);
    p2.load_board("board_1.txt");
    p2.move();
    */
}