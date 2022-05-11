#include <iostream>
#include "player.cpp"

using std::cout;
using std::endl;

int main()
{
    List<int> l;
    l.push_front(1);
    l.push_front(0);
    l.push_back(2);
    l.push_back(3);
    for (int i : l)
        cout << i << endl;
}