#include "player.hpp"

using std::string;
using std::ifstream;
using std::ofstream;
using std::endl;

#pragma region DATA STRUCTURES

template<typename T>
struct Cell
{
    T data;
    Cell* next;
};

template<typename T>
class Stack
{
public:
    class iterator
    {

    public:
        iterator(Cell<T>* cell)
        {
            current = cell;
        }

        iterator& operator++()
        {
            // ++i
            current = current->next;
            return *this;
        }

        iterator operator++(int) // int è dummy, per differenziarlo da ++i
        {
            // i++
            iterator old(*this);
            current = current->next;
            return old;
        }

        T& operator*()
        {
            return current->data;
        }

    private:
        using iterator_category = std::forward_iterator_tag;
        Cell <T>* current;
    };

    Stack()
    {
        head = nullptr;
    }

    Stack(const Stack<T>& s)
    {
        head = copy(s->head);
    }

    ~Stack()
    {
        destroy(head);
    }

    void push(T element)
    {
        Cell<T>* c = new Cell<T>;
        c->next = nullptr;
        if (head == nullptr)
        {
            c->next = nullptr;
            head = c;
        }
        else
        {
            c->next = head;
            head = c;
        }
    }

    T& top()
    {
        return head->data;
    }

    T& top() const
    {
        return head->data;
    }

    void pop()
    {
        if (head == nullptr)
            throw player_exception{ player_exception::index_out_of_bounds, "Tried to pop but stack is empty." };
        Cell<T>* c = head;
        head = head->next;
        delete c;
    }

    T& at(int pos)
    {
        if (head == nullptr)
            throw player_exception { player_exception::index_out_of_bounds, "Out of stack bounds" };
        Cell<T>* pc = head;
        while (pos > 0)
        {
            pc = pc->next;
            if (pc == nullptr)
                throw player_exception { player_exception::index_out_of_bounds, "Out of stack bounds" };
            --pos;
        }
        return pc->data;
    }
    
    T& at(int pos) const
    {
        if (head == nullptr)
            throw player_exception { player_exception::index_out_of_bounds, "Out of stack bounds" };
        Cell<T>* pc = head;
        while (pos > 0)
        {
            pc = pc->next;
            if (pc == nullptr)
                throw player_exception { player_exception::index_out_of_bounds, "Out of stack bounds" };
            --pos;
        }
        return pc->data;
    }

    int size() const
    {
        Cell<T>* pc = head;
        int size = 0;
        while (pc != nullptr)
        {
            pc = pc->next;
            ++size;
        }
        return size;
    }

    Stack& operator=(const Stack& s)
    {
        if (this != &s)
        {
            destroy(head);
            head = copy(s.head);
        }
        return *this;
    }

private:
    Cell<T>* head;

    Cell<T>* copy(Cell<T>* pc)
    {
        Cell<T>* newcell = nullptr;
        if (pc != nullptr)
        {
            newcell = new Cell<T>;
            newcell->data = pc->data;
            newcell->next = copy(pc->next);
        }

        return newcell;
    }

    void destroy(Cell<T>* pc)
    {
        if (pc != nullptr)
        {
            if (pc->next != nullptr)
                destroy(pc->next);
            delete pc;
        }
    }
};

#pragma endregion

struct Board
{
    Player::piece pieces[8][8];
};


struct Player::Impl
{
    int player_nr;
    Stack<Board> history;
};

Player::Player(int player_nr)
{
    if (player_nr < 1 || player_nr > 2)
        throw player_exception
        {
            player_exception::index_out_of_bounds,
            "Player number not valid. Must be 1 or 2"
        };
    pimpl = new Impl;
    pimpl->player_nr = player_nr;
}

Player::Player(const Player &p)
{
    // TODO ad ogni modifica del pimpl va sistemato il copy constructor
    pimpl = new Impl;
    pimpl->player_nr = p.pimpl->player_nr;
    pimpl->history = p.pimpl->history;
}

Player::~Player()
{
    delete pimpl;
}

Player& Player::operator=(const Player& p)
{
    // TODO anche questo va sistemato pari passo con il pimpl e il copy constructor
    if (this != &p)
    {
        pimpl->player_nr = p.pimpl->player_nr;
        pimpl->history = p.pimpl->history;
    }
    return *this;
}

Player::piece Player::operator()(int r, int c, int history_offset) const
{
    // TODO
}

void Player::load_board(const std::string& filename)
{
    ifstream file(filename);
    if (!file.good())
        throw player_exception{ player_exception::missing_file, "Missing file: " + filename };

    Board b;
    for (int i = 0; i < 8; ++i)
    {
        string s;
        std::getline(file, s);
        if (s.length() != 15)
            throw player_exception { player_exception::invalid_board, "Board row size not equal to 15" };
        for (int j = 0; j < s.length(); ++j)
        {
            // Le celle dispari sono tutti spazi
            if (j % 2 != 0 && s.at(j) != ' ')
                throw player_exception { player_exception::invalid_board, "Expected space" };
            // Le celle pari sono le caselle della scacchiera
            else
            {
                // Si può sapere se una cella è bianca se la somma tra gli indici
                // di riga e colonna è dispari, analogamente pari per le celle nere
                if ((i + j) % 2 != 0)
                {
                    if (s.at(j) != ' ')
                        throw player_exception { player_exception::invalid_board, "Occupied white cell" };
                }
                switch(s.at(j))
                {
                    case ' ':
                        b.pieces[i][j / 2] = Player::e;
                        break;
                    case 'x':
                        b.pieces[i][j / 2] = Player::x;
                        break;
                    case 'X':
                        b.pieces[i][j / 2] = Player::X;
                        break;
                    case 'o':
                        b.pieces[i][j / 2] = Player::o;
                        break;
                    case 'O':
                        b.pieces[i][j / 2] = Player::O;
                        break;
                    default:
                        throw player_exception{ player_exception::invalid_board, "Piece not valid" };
                }
            }
        }
    }

    // FIXME non è detto che funzioni sempre a causa del getline, investigare
    if (!file.eof())
        throw player_exception{ player_exception::invalid_board, "New line at end of board " + filename };

    file.close();

    pimpl->history.push(b);
}

void Player::store_board(const std::string& filename, int history_offset) const
{
    ofstream file(filename);
    Board b = pimpl->history.at(history_offset);
    // TODO committo, buonanotte.
}

void Player::init_board(const std::string& filename) const
{
    ofstream file(filename);
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            // La cella è nera, si possono mettere le pedine
            if ((i + j) % 2 == 0)
            {
                // Zona bianchi
                if (i <= 2)
                    file << 'o';
                // Zona neri
                else if (i >= 5)
                    file << 'x';
                // Zona centrale
                else
                    file << ' ';
            }
            // Cella bianca
            else
                file << ' ';
            
            // Spazio tra le celle
            if (j < 7)
                file << ' ';
        }
        if (i < 7)
            file << endl;
    }
    file.close();
}

void Player::move()
{
    // TODO
}

bool Player::valid_move() const
{
    // TODO
}


void Player::pop()
{
    pimpl->history.pop();
}

bool Player::wins(int player_nr) const
{
    // TODO
}

bool Player::wins() const
{
    // TODO
}

bool Player::loses(int player_nr) const
{
    // TODO
}

bool Player::loses() const
{
    // TODO
}

int Player::recurrence() const
{
    
}
