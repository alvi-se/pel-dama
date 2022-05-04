#include "player.hpp"

using std::string;
using std::ifstream;
using std::ofstream;
using std::ostream;
using std::istream;
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
            if (pc == nullptr)
                throw player_exception { player_exception::index_out_of_bounds, "Out of stack bounds" };
            pc = pc->next;
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
            if (pc == nullptr)
                throw player_exception { player_exception::index_out_of_bounds, "Out of stack bounds" };
            pc = pc->next;
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

char piece_to_char(Player::piece p)
{
    switch (p)
    {
        case Player::e : return ' ';
        case Player::o : return 'o';
        case Player::x : return 'x';
        case Player::O : return 'O';
        case Player::X : return 'X';
        default:
            throw player_exception { player_exception::invalid_board, "Invalid piece" };
    }
}

Player::piece char_to_piece(char c)
{
    switch (c)
    {
        case ' ' : return Player::e;
        case 'o' : return Player::o;
        case 'x' : return Player::x;
        case 'O' : return Player::O;
        case 'X' : return Player::X;
        default:
            throw player_exception { player_exception::invalid_board, "Invalid piece"};
    }
}

class Board
{
public:
    /**
     * @brief Istanzia una board iniziale 
     */
    Board()
    {
        for (size_t i = 0; i < 8; ++i)
        {
            for (size_t j = 0; j < 8; ++j)
            {
            // La cella è nera, si possono mettere le pedine
                if ((i + j) % 2 == 0)
                {
                    // Zona bianchi
                    if (i <= 2)
                        pieces[i][j] = Player::o;
                    // Zona neri
                    else if (i >= 5)
                        pieces[i][j] = Player::x;
                    // Zona centrale
                    else
                        pieces[i][j] = Player::e;
                }
                // Cella bianca
                else
                    pieces[i][j] = Player::e;
            }
        }
    }

    /**
     * @brief Istanzia la board leggendo la posizione delle celle
     * dallo stream passato per parametro
     * @param input Lo stream di input
     */
    Board(istream& input)
    {
        for (int i = 0; i < 8; ++i)
        {
            string s;
            std::getline(input, s);
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
                    // Bisogna però dividere per due il numero della colonna, dato che le
                    // celle sono tutte spaziate
                    if ((i + j / 2) % 2 != 0)
                    {
                        if (s.at(j) != ' ')
                            throw player_exception { player_exception::invalid_board, "Occupied white cell" };
                    }
                    else
                    {
                        pieces[i][j / 2] = char_to_piece(s.at(j));
                    }
                }
            }
        }
    }

    Player::piece& at(int i, int j)
    {
        if (
            i < 0 || i >= 8 ||
            j < 0 || j >= 8
        )
            throw player_exception { player_exception::index_out_of_bounds, "Invalid position" };
        return pieces[i][j];
    }

    void print(ostream& output)
    {
        for (size_t i = 0; i < 8; ++i)
        {
            for (size_t j = 0; j < 8; ++j)
            {
                char c = piece_to_char(pieces[i][j]);
                output << c;
                if (j < 7)
                    output << ' ';
            }
            if (i < 7)
                output << endl;
        }
    }

    bool operator==(const Board& b)
    {
        bool equal = true;
        for (size_t i = 0; i < 8; ++i)
        {
            for (size_t j = 0; j < 8; ++j)
            {
                if (pieces[i][j] != b.pieces[i][j])
                    equal = false;
            }
        }
        return equal;
    }
    
private:
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
    pimpl->history.at(history_offset).at(r, c);
}

void Player::load_board(const std::string& filename)
{
    ifstream file(filename);
    if (!file.good())
        throw player_exception{ player_exception::missing_file, "Missing file: " + filename };

    Board b(file);

    if (!file.eof())
        throw player_exception{ player_exception::invalid_board, "New line at end of board " + filename };

    file.close();

    pimpl->history.push(b);
}

void Player::store_board(const std::string& filename, int history_offset) const
{
    ofstream file(filename);
    Board& b = pimpl->history.at(history_offset);
    b.print(file);
    file.close();
    
}

void Player::init_board(const std::string& filename) const
{
    ofstream file(filename);
    Board().print(file);
    file.close();
}

void Player::move()
{
    // TODO
}

bool Player::valid_move() const
{
    if (pimpl->history.at(0) == pimpl->history.at(1))
        return false;
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
    // TODO
}
