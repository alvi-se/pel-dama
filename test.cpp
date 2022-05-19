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
    int c = 0;
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            if (b.promotion(i, j))
                c++;
        }
    }
    cout << c << endl;
}