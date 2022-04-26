#include "player.hpp"

#pragma region IMPL

template<typename T>
struct Cell
{
    T data;
    Cell* next;
};

struct Board
{
    Player::piece pieces[8][8];
};


struct Impl
{
    struct Cell
    {
        int data;
    };
};

#pragma endregion

Player::Player(int player_nr)
{
    if (player_nr < 1 || player_nr > 2)
        throw player_exception
        {
            player_exception::index_out_of_bounds,
            "Player number not valid. Must be 1 or 2, passed " + player_nr + " instead."
        };
}

Player::Player(const Player &)
{
    
}

Player::~Player()
{
    
}

Player& Player::operator=(const Player& p)
{

}

Player::piece Player::operator()(int r, int c, int history_offset) const
{

}

void Player::load_board(const std::string& filename)
{
    
}

void Player::store_board(const std::string& filename, int history_offset) const
{

}

void Player::init_board(const std::string& filename) const
{

}

void Player::move()
{

}

bool Player::valid_move() const
{

}


void Player::pop()
{

}

bool Player::wins(int player_nr) const
{

}

bool Player::wins() const
{

}

bool Player::loses(int player_nr) const
{

}

bool Player::loses() const
{

}

int Player::recurrence() const
{

}