#include <iostream>
#include "player.hpp"

using std::cout;
using std::endl;

int main()
{
    Player p;
    //p.init_board("board_1.txt");
    p.load_board("board_1.txt");
    for (size_t i = 0; i < 8; i++)
    {
        for (size_t j = 0; j < 8; j++)
        {
            switch (p(i, j, 0))
            {
                case Player::piece::e : cout << 'e'; break;
                case Player::piece::x : cout << 'x'; break;
                case Player::piece::o : cout << 'o'; break;
                default: cout << "bruh";
                // Ci rinuncio, buonanotte
            }
        }
        cout << endl;
        
    }
    
}