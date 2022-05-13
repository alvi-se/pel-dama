/**
 * @file player.cpp
 * @author Alvise Favero (888851@stud.unive.it)
 * @date 14/06/2022
 * @version 1
 */

#include "player.hpp"

using std::endl;
using std::ifstream;
using std::istream;
using std::ofstream;
using std::ostream;
using std::string;

#pragma region DATA STRUCTURES

template <typename T>
struct Cell
{
    T data;
    Cell* next;
};

template <typename T>
class List
{
public:
    using Pcell = Cell<T>*;
    class iterator
    {
    public:
        using iterator_category = std::forward_iterator_tag;

        iterator(Pcell cell)
        {
            current = cell;
        }

        iterator(const iterator& it)
        {
            current = it.current;
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

        bool operator==(const iterator& it) const
        {
            return current == it.current;
        }

        bool operator!=(const iterator& it) const
        {
            return current != it.current;
        }

    private:
        Pcell current;
    };

    List()
    {
        head = nullptr;
    }

    List(const List<T>& l)
    {
        head = copy(l->head);
    }

    List(List<T>&& l)
    {
        head = l->head;
        l->head = nullptr;
    }

    ~List()
    {
        destroy(head);
    }

    void push_front(T element)
    {
        Pcell c = new Cell<T>;
        c->next = head;
        head = c;
        c->data = element;
    }

    void push_back(T element)
    {
        Pcell c = new Cell<T>;
        c->data = element;
        c->next = nullptr;
        if (head == nullptr)
        {
            head = c;
        }
        else
        {
            Pcell pc = head;
            while (pc->next != nullptr)
                pc = pc->next;
            pc->next = c;
        }
    }

    T& front()
    {
        if (head == nullptr)
            throw new player_exception{ player_exception::index_out_of_bounds, "No elements in stack" };
        return head->data;
    }

    T& front() const
    {
        if (head == nullptr)
            throw new player_exception{ player_exception::index_out_of_bounds, "No elements in stack" };
        return head->data;
    }

    T& back()
    {
        if (head == nullptr)
            throw new player_exception{ player_exception::index_out_of_bounds, "No elements in stack" };
        return lastCell()->data;
    }

    T& back() const
    {
        if (head == nullptr)
            throw new player_exception{ player_exception::index_out_of_bounds, "No elements in stack" };
        return lastCell()->data;
    }

    void pop_front()
    {
        if (head == nullptr)
            throw player_exception{ player_exception::index_out_of_bounds, "Tried to pop but stack is empty." };
        Pcell c = head;
        head = head->next;
        delete c;
    }

    T& at(int pos)
    {
        Pcell pc = head;
        while (pos > 0)
        {
            if (pc == nullptr)
                throw player_exception{ player_exception::index_out_of_bounds, "Out of stack bounds" };
            pc = pc->next;
            --pos;
        }
        if (pc == nullptr)
            throw player_exception{ player_exception::index_out_of_bounds, "Out of stack bounds" };
        return pc->data;
    }

    T& at(int pos) const
    {
        Pcell pc = head;
        while (pos > 0)
        {
            if (pc == nullptr)
                throw player_exception{ player_exception::index_out_of_bounds, "Out of stack bounds" };
            pc = pc->next;
            --pos;
        }
        if (pc == nullptr)
            throw player_exception{ player_exception::index_out_of_bounds, "Out of stack bounds" };
        return pc->data;
    }

    void remove(int pos)
    {
        if (pos == 0)
        {
            if (head == nullptr)
                throw player_exception{ player_exception::index_out_of_bounds, "Out of stack bounds" };
            Pcell toremove = head;
            head = toremove->next;
            delete toremove;
        }
        else
        {
            remove_rec(pos, head);
        }
    }

    size_t size() const
    {
        return size_rec(head);
    }

    List& operator=(const List& l)
    {
        if (this != &l)
        {
            destroy(head);
            head = copy(l.head);
        }
        return *this;
    }

    List& operator=(List&& l)
    {
        if (this != &l)
        {
            head = l.head;
            l.head = nullptr;
        }
        return *this;
    }

    iterator begin()
    {
        return iterator(head);
    }

    iterator end()
    {
        return iterator(nullptr);
    }

private:
    Pcell head;

    Pcell& lastCell()
    {
        Pcell pc = head;
        while (pc != nullptr && pc->next != nullptr)
            pc = pc->next;
        return pc;
    }

    Pcell copy(Pcell pc)
    {
        Pcell newcell = nullptr;
        if (pc != nullptr)
        {
            newcell = new Cell<T>;
            newcell->data = pc->data;
            newcell->next = copy(pc->next);
        }

        return newcell;
    }

    void remove_rec(int pos, Pcell pc)
    {
        if (pc == nullptr || pc->next == nullptr)
            throw player_exception{ player_exception::index_out_of_bounds, "Out of stack bounds" };
        if (pos > 1)
        {
            remove_rec(--pos, pc->next);
        }
        else
        {
            Pcell toremove = pc->next;
            pc->next = toremove->next;
            delete toremove;
        }
    }

    size_t size_rec(Pcell pc) const
    {
        if (pc == nullptr)
            return 0;
        else
            return 1 + size_rec(pc->next);
    }

    void destroy(Pcell pc)
    {
        if (pc != nullptr)
        {
            if (pc->next != nullptr)
                destroy(pc->next);
            delete pc;
        }
    }
};

template <typename T>
class Vector
{
public:
    Vector(size_t size)
    {
        data = new T[size];
        _size = size;
    }

    Vector(const Vector& v)
    {
        data = new T[v._size];
        _size = v._size;
        for (size_t i = 0; i < _size; ++i)
        {
            data[i] = v.data[i];
        }
    }

    T& at(size_t index)
    {
        if (index >= _size)
            throw player_exception{ player_exception::index_out_of_bounds, "Out of vector bounds" };
        return data[index];
    }

    size_t size() const
    {
        return _size;
    }

    ~Vector()
    {
        delete[] data;
    }

private:
    T* data;
    size_t _size;

    Vector();
};

#pragma endregion

#pragma region GAME ELEMENTS AND FUNCTIONS

char piece_to_char(Player::piece p)
{
    switch (p)
    {
    case Player::e: return ' ';
    case Player::o: return 'o';
    case Player::x: return 'x';
    case Player::O: return 'O';
    case Player::X: return 'X';
    default:
        throw player_exception{ player_exception::invalid_board, "Invalid piece" };
    }
}

Player::piece char_to_piece(char c)
{
    switch (c)
    {
    case ' ': return Player::e;
    case 'o': return Player::o;
    case 'x': return Player::x;
    case 'O': return Player::O;
    case 'X': return Player::X;
    default:
        throw player_exception{ player_exception::invalid_board, "Invalid piece" };
    }
}

/**
 * @brief Contiene i dati della mossa di un pezzo.
 * Se
 */
struct Move
{
    Player::piece p;
    int from[2];
    int to[2];
    int eats[2];
};

/**
 * @brief Rappresenta una scacchiera.
 * È formata da una matrice 8 x 8 di player::piece.
 * Per comodità, la matrice viene salvata in ordine
 * dal basso verso l'alto, da sinistra a destra.
 * Quindi l'angolo in basso a sinistra della scacchiera
 * sarà posizione (0, 0), quello in alto a destra sarà (7, 7)
 */
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
                if ((i + j) % 2 != 0)
                {
                    // Zona neri
                    if (i <= 2)
                        pieces[i][j] = Player::x;
                    // Zona bianchi
                    else if (i >= 5)
                        pieces[i][j] = Player::o;
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
                throw player_exception{ player_exception::invalid_board, "Board row size not equal to 15" };
            for (size_t j = 0; j < s.length(); ++j)
            {
                // Le celle dispari sono tutti spazi
                if (j % 2 != 0 && s.at(j) != ' ')
                    throw player_exception{ player_exception::invalid_board, "Expected space" };
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
                            throw player_exception{ player_exception::invalid_board, "Occupied white cell" };
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
            j < 0 || j >= 8)
            throw player_exception{ player_exception::index_out_of_bounds, "Invalid position" };
        return pieces[i][j];
    }

    const Player::piece& at(int i, int j) const
    {
        if (
            i < 0 || i >= 8 ||
            j < 0 || j >= 8)
            throw player_exception{ player_exception::index_out_of_bounds, "Invalid position" };
        return pieces[i][j];
    }

    void print(ostream& output)
    {
        for (int i = 7; i >= 0; --i)
        {
            for (int j = 0; j < 8; ++j)
            {
                char c = piece_to_char(pieces[i][j]);
                output << c;
                if (j < 7)
                    output << ' ';
            }
            if (i > 0)
                output << endl;
        }
    }
    /**
     * @brief Controlla se una pedina si può muovere nella board attuale.
     * La possibilità di mangiare una pedina avversaria viene inclusa.
     *
     * @param row La riga della pedina.
     * @param col La colonna della pedina.
     * @return True se la pedina si può muovere, false altrimenti.
     */
    bool canMove(int row, int col) const
    {
        if (
            row < 0 || row > 7 ||
            col < 0 || col > 7)
            throw player_exception{ player_exception::index_out_of_bounds, "Out of board bounds" };

        switch (at(row, col))
        {
        case Player::piece::x:
            // Verso l'alto
            if (row < 7)
            {
                // Lato sinistro
                if (col > 0 && at(row + 1, col - 1) == Player::piece::e)
                    return true;
                // Lato destro
                if (col < 7 && at(row + 1, col + 1) == Player::piece::e)
                    return true;
            }
            break;
        case Player::piece::o:
            // Verso il basso
            if (row > 0)
            {
                // Lato sinistro
                if (col > 0 && at(row - 1, col - 1) == Player::piece::e)
                    return true;
                // Lato destro
                if (col < 7 && at(row - 1, col + 1) == Player::piece::e)
                    return true;
            }
            break;
        case Player::piece::X:
        case Player::piece::O:
            // Verso l'alto
            if (row < 7)
            {
                // Lato sinistro
                if (col > 0 && at(row + 1, col - 1) == Player::piece::e)
                    return true;
                // Lato destro
                if (col < 7 && at(row + 1, col + 1) == Player::piece::e)
                    return true;
            }
            // Verso il basso
            if (row > 0)
            {
                // Lato sinistro
                if (col > 0 && at(row - 1, col - 1) == Player::piece::e)
                    return true;
                // Lato destro
                if (col < 7 && at(row - 1, col + 1) == Player::piece::e)
                    return true;
            }
            break;
        }
        return canEat(row, col);
    }

    /**
     * @brief Controlla se la pedina ha la possibilità di mangiare pedine avversarie.
     *
     * @param row La riga della pedina.
     * @param col La colonna della pedina.
     * @return True se può mangiare almeno una pedina, false altrimen 
     */
    bool canEat(int row, int col) const
    {
        if (
            row < 0 || row > 7 ||
            col < 0 || col > 7)
            throw player_exception{ player_exception::index_out_of_bounds, "Out of board bounds" };

        // TODO finire
        switch (at(row, col))
        {
        case Player::piece::x:
            if (row < 6)
            {
                // Lato sinistro
                if (
                    col > 1 &&
                    at(row + 1, col - 1) == Player::piece::o &&
                    at(row + 2, col - 2) == Player::piece::e
                    )
                    return true;
                // Lato destro
                if (
                    col > 1 &&
                    at(row + 1, col + 1) == Player::piece::o &&
                    at(row + 2, col + 2) == Player::piece::e
                    )
                    return true;
            }
            break;
        case Player::piece::o:
            break;
        case Player::piece::X:
            break;
        case Player::piece::O:
            break;
        }
        return false;
    }

    bool promotion(int row, int col) const
    {
        switch (at(row, col))
        {
            // Una dama non può essere promossa!
        case Player::piece::X: return false;
        case Player::piece::O: return false;
        case Player::piece::x:
            if (row != 6)
                return false;
            // Lato sinistro
            else if (
                col > 0 &&
                at(7, col - 1) == Player::piece::e
                )
                return true;
            // Lato destro
            else if (
                col < 7 &&
                at(7, col + 1) == Player::piece::e
                )
                return true;
            break;
        case Player::piece::o:
            if (row != 1)
                return false;
            // Lato sinistro
            else if (
                col > 0 &&
                at(0, col - 1) == Player::piece::e
                )
                return true;
            // Lato destro
            else if (
                col < 7 &&
                at(0, col + 1) == Player::piece::e
                )
                return true;
            break;
        }
        return false;
    }

    Move getMove(const Board& b)
    {
        // TODO
    }

    bool operator==(const Board& b) const
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

#pragma endregion

struct Player::Impl
{
    int player_nr;
    List<Board> history;

    bool canMove(const Board& b, int row, int col)
    {

    }
};

Player::Player(int player_nr)
{
    if (player_nr < 1 || player_nr > 2)
        throw player_exception{
            player_exception::index_out_of_bounds,
            "Player number not valid. Must be 1 or 2" };
    pimpl = new Impl;
    pimpl->player_nr = player_nr;
}

Player::Player(const Player& p)
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

    pimpl->history.push_front(b);
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

}

bool Player::valid_move() const
{
    try
    {
        Move m = pimpl->history.at(0).getMove(pimpl->history.at(1));
    }
    catch (const player_exception e)
    {
        return false;
    }
    return true;

}

void Player::pop()
{
    pimpl->history.pop_front();
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
    Board top = pimpl->history.front();
    int count = 0;
    for (const Board b : pimpl->history)
    {
        if (b == top)
            ++count;
    }
    return count;
}
