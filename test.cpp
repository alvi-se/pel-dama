#include <iostream>
#include "player.cpp"

using std::cout;
using std::endl;

int main()
{
    ifstream str("board_1.txt");
    Board b(str);
    b.print(cout);
    cout << endl;
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            cout << b.canEat(i, j) << endl;
        }
    }
}