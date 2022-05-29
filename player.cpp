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

struct Position
{
    int row;
    int col;

    void print(ostream& output)
    {
        output << '[' << row << ", " << col << ']';
    }
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
        head = copy(l.head);
    }

    List(List<T>&& l)
    {
        head = l.head;
        l.head = nullptr;
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
            throw new player_exception{ player_exception::index_out_of_bounds, "No elements in list" };
        return head->data;
    }

    T& front() const
    {
        if (head == nullptr)
            throw new player_exception{ player_exception::index_out_of_bounds, "No elements in list" };
        return head->data;
    }

    T& back()
    {
        if (head == nullptr)
            throw new player_exception{ player_exception::index_out_of_bounds, "No elements in list" };
        return lastCell()->data;
    }

    T& back() const
    {
        if (head == nullptr)
            throw new player_exception{ player_exception::index_out_of_bounds, "No elements in list" };
        return lastCell()->data;
    }

    void pop_front()
    {
        if (head == nullptr)
            throw player_exception{ player_exception::index_out_of_bounds, "Tried to pop but list is empty" };
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
                throw player_exception{ player_exception::index_out_of_bounds, "Out of list bounds" };
            pc = pc->next;
            --pos;
        }
        if (pc == nullptr)
            throw player_exception{ player_exception::index_out_of_bounds, "Out of list bounds" };
        return pc->data;
    }

    T& at(int pos) const
    {
        Pcell pc = head;
        while (pos > 0)
        {
            if (pc == nullptr)
                throw player_exception{ player_exception::index_out_of_bounds, "Out of list bounds" };
            pc = pc->next;
            --pos;
        }
        if (pc == nullptr)
            throw player_exception{ player_exception::index_out_of_bounds, "Out of list bounds" };
        return pc->data;
    }

    bool isEmpty() const
    {
        return head == nullptr;
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

    List& operator=(const List<T>& l)
    {
        if (this != &l)
        {
            destroy(head);
            head = copy(l.head);
        }
        return *this;
    }

    List& operator=(List<T>&& l)
    {
        if (this != &l)
        {
            head = l.head;
            l.head = nullptr;
        }
        return *this;
    }

    List& operator+(const List<T>& l) const
    {
        List<T> l1{ *this };
        Pcell l2 = copy(l.head);
        l1.lastCell()->next = l2;
        return l1;
    }

    List& operator+=(const List<T>& l)
    {
        Pcell l2 = copy(l.head);
        if (head == nullptr)
            head = l2;
        else
        {
            Pcell last = lastCell();
            last->next = l2;

        }
        return *this;
    }

    iterator begin()
    {
        return iterator(head);
    }

    const iterator begin() const
    {
        return iterator(head);
    }

    iterator end()
    {
        return iterator(nullptr);
    }

    const iterator end() const
    {
        return iterator(nullptr);
    }

private:
    Pcell head;

    Pcell lastCell()
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
    Vector()
    {
        // 10 come capacità è abbastanza, considerato
        // che il vettore verrà utilizzato per righe/colonne
        //della board (quindi di dimensione 8)
        _capacity = 10;
        data = new T[_capacity];
        _size = 0;
    }

    Vector(size_t size)
    {
        data = new T[size];
        _size = size;
        _capacity = size;
    }

    Vector(size_t size, const T& el)
    {
        data = new T[size];
        _size = size;
        _capacity = size;
        for (size_t i = 0; i < size; ++i)
        {
            data[i] = el;
        }
    }

    Vector(const Vector<T>& v)
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

    const T& at(size_t index) const
    {
        if (index >= _size)
            throw player_exception{ player_exception::index_out_of_bounds, "Out of vector bounds" };
        return data[index];
    }

    T& operator[](size_t index)
    {
        if (index >= _size)
            throw player_exception{ player_exception::index_out_of_bounds, "Out of vector bounds" };
        return data[index];
    }

    const T& operator[](size_t index) const
    {
        if (index >= _size)
            throw player_exception{ player_exception::index_out_of_bounds, "Out of vector bounds" };
        return data[index];
    }

    void push_back(T el)
    {
        if (_size >= _capacity)
        {
            _capacity *= 2;
            data = static_cast<T*>(realloc(data, _capacity));
        }
        data[_size++] = el;
    }

    Vector<T>& operator=(const Vector<T>& v)
    {
        if (this != &v)
        {
            delete[] data;
            data = new T[v._size];
            _size = v._size;
            for (size_t i = 0; i < _size; ++i)
            {
                data[i] = v.data[i];
            }
        }
        return *this;
    }

    size_t size() const
    {
        return _size;
    }

    size_t capacity() const
    {
        return _capacity;
    }

    void print(ostream& output)
    {
        output << "[ ";
        for (size_t i = 0; i < _size; ++i)
            output << at(i) << ' ';
        output << ']';
    }

    ~Vector()
    {
        delete[] data;
    }

private:
    T* data;
    size_t _size;
    size_t _capacity;
};

#pragma endregion

#pragma region GAME ELEMENTS AND FUNCTIONS

/**
 * @brief Converte una pedina nel suo corrispettivo carattere.
 *
 * @param p La pedina.
 * @return Il carattere.
 */
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

/**
 * @brief Converte un carattere all'enum Piece.
 *
 * @param c Il carattere.
 * @return La pedina.
 */
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

int get_opponent(int player_nr)
{
    if (player_nr == 1) return 2;
    if (player_nr == 2) return 1;
    throw player_exception{ player_exception::index_out_of_bounds, "Invalid player number" };
}

void assert_position(Position pos)
{
    if (
        pos.row < 0 ||
        pos.col < 0 ||
        pos.row > 7 ||
        pos.col > 7
        ) throw player_exception{ player_exception::index_out_of_bounds, "Position not valid." };
}

/**
 * @brief Contiene i dati della mossa di una pedina.
 */
struct Move
{
    Position from;
    Position to;

    /**
     * @brief Controlla se la mossa mangia un'altra pedina,
     * limitandosi a controllare la distanza di spostamento.
     * ATTENZIONE: dà per scontato che la mossa sia valida!
     * @return true se la mossa mangia una pedina, false altrimenti.
     */
    bool jumps() const
    {
        return std::abs(to.row - from.row) == 2;
    }
};

/**
 * @brief Specifica la direzione in cui si deve muovere la pedina.
 * Si poteva fare anche con un int, ma con un enum è più chiaro.
 */
enum MoveDirection
{
    U_L,
    U_R,
    D_L,
    D_R
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
     * @brief Istanzia una board iniziale.
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
     * dallo stream passato per parametro.
     * @param input Lo stream di input.
     */
    Board(istream& input)
    {
        for (int i = 7; i >= 0; --i)
        {
            string s;
            std::getline(input, s);
            if (s.length() != 15)
                throw player_exception{ player_exception::invalid_board, "Board row size not equal to 15" };
            for (size_t j = 0; j < s.length(); ++j)
            {
                // Le celle dispari sono tutti spazi
                if (j % 2 != 0)
                {
                    if (s.at(j) != ' ')
                        throw player_exception{ player_exception::invalid_board, "Expected space" };
                }
                // Le celle pari sono le caselle della scacchiera
                else
                {
                    // Si può sapere se una cella è bianca se la somma tra gli indici
                    // di riga e colonna è dispari, analogamente pari per le celle nere
                    // Bisogna però dividere per due il numero della colonna, dato che le
                    // celle sono tutte spaziate
                    if ((i + j / 2) % 2 == 0)
                    {
                        if (s.at(j) != ' ')
                            throw player_exception{ player_exception::invalid_board, "Occupied white cell" };
                        pieces[i][j / 2] = Player::piece::e;
                    }
                    else
                    {
                        pieces[i][j / 2] = char_to_piece(s.at(j));
                    }
                }
            }
        }
    }

    /**
     * @brief Copy constructor.
     * @param b La board da copiare.
     */
    Board(const Board& b)
    {
        for (size_t i = 0; i < 8; ++i)
        {
            for (size_t j = 0; j < 8; ++j)
            {
                pieces[i][j] = b.pieces[i][j];
            }
        }
    }

    /**
     * @brief Restituisce la pedina alle specificate.
     * @param i Riga della pedina.
     * @param j Colonna della pedina.
     * @return La pedina.
     * @exception player_exception Lanciata se si esce dai bordi della board.
     */
    Player::piece& at(int i, int j)
    {
        if (
            i < 0 || i >= 8 ||
            j < 0 || j >= 8)
            throw player_exception{ player_exception::index_out_of_bounds, "Invalid position" };
        return pieces[i][j];
    }

    Player::piece& at(Position pos)
    {
        return at(pos.row, pos.col);
    }

    const Player::piece& at(int i, int j) const
    {
        if (
            i < 0 || i >= 8 ||
            j < 0 || j >= 8)
            throw player_exception{ player_exception::index_out_of_bounds, "Invalid position" };
        return pieces[i][j];
    }

    const Player::piece& at(Position pos) const
    {
        return at(pos.row, pos.col);
    }

    /**
     * @brief Stampa la board in uno stream.
     *
     * @param output Lo stream di output.
     */
    void print(ostream& output) const
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

    List<Move> getPossibleMoves(Position pos) const
    {
        List<Move> moves;
        assert_position(pos);

        switch (at(pos))
        {
        case Player::piece::x:
            // Verso l'alto
            if (pos.row < 7)
            {
                // Lato sinistro
                if (pos.col > 0 && at(pos.row + 1, pos.col - 1) == Player::piece::e)
                    moves.push_back(Move{
                        pos,
                        Position{pos.row + 1, pos.col - 1}
                        });
                // Lato destro
                if (pos.col < 7 && at(pos.row + 1, pos.col + 1) == Player::piece::e)
                    moves.push_back(Move{
                        pos,
                        Position{pos.row + 1, pos.col + 1}
                        });
            }
            break;
        case Player::piece::o:
            // Verso il basso
            if (pos.row > 0)
            {
                // Lato sinistro
                if (pos.col > 0 && at(pos.row - 1, pos.col - 1) == Player::piece::e)
                    moves.push_back(Move{
                        pos,
                        Position{pos.row - 1, pos.col - 1}
                        });
                // Lato destro
                if (pos.col < 7 && at(pos.row - 1, pos.col + 1) == Player::piece::e)
                    moves.push_back(Move{
                        pos,
                        Position{pos.row - 1, pos.col + 1}
                        });
            }
            break;
        case Player::piece::X:
        case Player::piece::O:
            // Verso l'alto
            if (pos.row < 7)
            {
                // Lato sinistro
                if (pos.col > 0 && at(pos.row + 1, pos.col - 1) == Player::piece::e)
                    moves.push_back(Move{
                        pos,
                        Position{pos.row + 1, pos.col - 1}
                        });
                // Lato destro
                if (pos.col < 7 && at(pos.row + 1, pos.col + 1) == Player::piece::e)
                    moves.push_back(Move{
                        pos,
                        Position{pos.row + 1, pos.col + 1}
                        });
            }
            // Verso il basso
            if (pos.row > 0)
            {
                // Lato sinistro
                if (pos.col > 0 && at(pos.row - 1, pos.col - 1) == Player::piece::e)
                    moves.push_back(Move{
                        pos,
                        Position{pos.row - 1, pos.col - 1}
                        });
                // Lato destro
                if (pos.col < 7 && at(pos.row - 1, pos.col + 1) == Player::piece::e)
                    moves.push_back(Move{
                        pos,
                        Position{pos.row - 1, pos.col + 1}
                        });
            }
            break;
        }
        moves += getPossibleJumps(pos);
        return moves;
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
        // Se la pedina non può muoversi nelle celle adiacenti,
        // vede se può mangiare alcune pedine.
        return canJump(row, col);
    }

    List<Move> getPossibleJumps(Position pos) const
    {
        List<Move> jumps;
        assert_position(pos);
        switch (at(pos.row, pos.col))
        {
        case Player::piece::x:
            if (pos.row < 6)
            {
                // Lato sinistro
                if (
                    pos.col > 1 &&
                    at(pos.row + 1, pos.col - 1) == Player::piece::o &&
                    at(pos.row + 2, pos.col - 2) == Player::piece::e
                    )
                    jumps.push_back(Move{
                        pos,
                        Position{pos.row + 2, pos.col - 2}
                        });
                // Lato destro
                if (
                    pos.col < 6 &&
                    at(pos.row + 1, pos.col + 1) == Player::piece::o &&
                    at(pos.row + 2, pos.col + 2) == Player::piece::e
                    )
                    jumps.push_back(Move{
                        pos,
                        Position{pos.row + 2, pos.col + 2}
                        });
            }
            break;
        case Player::piece::o:
            if (pos.row > 1)
            {
                // Lato sinistro
                if (
                    pos.col > 1 &&
                    at(pos.row - 1, pos.col - 1) == Player::piece::x &&
                    at(pos.row - 2, pos.col - 2) == Player::piece::e
                    )
                    jumps.push_back(Move{
                        pos,
                        Position{pos.row - 2, pos.col - 2}
                        });
                // Lato destro
                if (
                    pos.col < 6 &&
                    at(pos.row - 1, pos.col + 1) == Player::piece::x &&
                    at(pos.row - 2, pos.col + 2) == Player::piece::e
                    )
                    jumps.push_back(Move{
                        pos,
                        Position{pos.row - 2, pos.col + 2}
                        });
            }
            break;
        case Player::piece::X:
            // Parte sopra
            if (pos.row < 6)
            {
                // Lato sinistro
                if (
                    pos.col > 1 &&
                    (at(pos.row + 1, pos.col - 1) == Player::piece::o ||
                        at(pos.row + 1, pos.col - 1) == Player::piece::O) &&
                    at(pos.row + 2, pos.col - 2) == Player::piece::e
                    )
                    jumps.push_back(Move{
                    pos,
                    Position{pos.row + 2, pos.col - 2}
                        });
                // Lato destro
                if (
                    pos.col < 6 &&
                    (at(pos.row + 1, pos.col + 1) == Player::piece::o ||
                        at(pos.row + 1, pos.col + 1) == Player::piece::O) &&
                    at(pos.row + 2, pos.col + 2) == Player::piece::e
                    )
                    jumps.push_back(Move{
                        pos,
                        Position{pos.row + 2, pos.col + 2}
                        });
            }
            // Parte sotto
            if (pos.row > 1)
            {
                // Lato sinistro
                if (
                    pos.col > 1 &&
                    (at(pos.row - 1, pos.col - 1) == Player::piece::o ||
                        at(pos.row - 1, pos.col - 1) == Player::piece::O) &&
                    at(pos.row - 2, pos.col - 2) == Player::piece::e
                    )
                    jumps.push_back(Move{
                        pos,
                        Position{pos.row - 2, pos.col - 2}
                        });
                // Lato destro
                if (
                    pos.col < 6 &&
                    (at(pos.row - 1, pos.col + 1) == Player::piece::o ||
                        at(pos.row - 1, pos.col + 1) == Player::piece::O) &&
                    at(pos.row - 2, pos.col + 2) == Player::piece::e
                    )
                    jumps.push_back(Move{
                        pos,
                        Position{pos.row - 2, pos.col + 2}
                        });
            }
            break;
        case Player::piece::O:
            // Parte sopra
            if (pos.row < 6)
            {
                // Lato sinistro
                if (
                    pos.col > 1 &&
                    (at(pos.row + 1, pos.col - 1) == Player::piece::x ||
                        at(pos.row + 1, pos.col - 1) == Player::piece::X) &&
                    at(pos.row + 2, pos.col - 2) == Player::piece::e
                    )
                    jumps.push_back(Move{
                        pos,
                        Position{pos.row + 2, pos.col - 2}
                        });
                // Lato destro
                if (
                    pos.col < 6 &&
                    (at(pos.row + 1, pos.col + 1) == Player::piece::x ||
                        at(pos.row + 1, pos.col + 1) == Player::piece::X) &&
                    at(pos.row + 2, pos.col + 2) == Player::piece::e
                    )
                    jumps.push_back(Move{
                        pos,
                        Position{pos.row + 2, pos.col + 2}
                        });
            }
            // Parte sotto
            if (pos.row > 1)
            {
                // Lato sinistro
                if (
                    pos.col > 1 &&
                    (at(pos.row - 1, pos.col - 1) == Player::piece::x ||
                        at(pos.row - 1, pos.col - 1) == Player::piece::X) &&
                    at(pos.row - 2, pos.col - 2) == Player::piece::e
                    )
                    jumps.push_back(Move{
                        pos,
                        Position{pos.row - 2, pos.col - 2}
                        });
                // Lato destro
                if (
                    pos.col < 6 &&
                    (at(pos.row - 1, pos.col + 1) == Player::piece::x ||
                        at(pos.row - 1, pos.col + 1) == Player::piece::X) &&
                    at(pos.row - 2, pos.col + 2) == Player::piece::e
                    )
                    jumps.push_back(Move{
                        pos,
                        Position{pos.row - 2, pos.col + 2}
                        });
            }
            break;
        }
        return jumps;
    }

    /**
     * @brief Controlla se la pedina ha la possibilità di mangiare pedine avversarie.
     *
     * @param row La riga della pedina.
     * @param col La colonna della pedina.
     * @return True se può mangiare almeno una pedina, false altrimenti.
     */
    bool canJump(int row, int col) const
    {
        if (
            row < 0 || row > 7 ||
            col < 0 || col > 7)
            throw player_exception{ player_exception::index_out_of_bounds, "Out of board bounds" };

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
                    col < 6 &&
                    at(row + 1, col + 1) == Player::piece::o &&
                    at(row + 2, col + 2) == Player::piece::e
                    )
                    return true;
            }
            break;
        case Player::piece::o:
            if (row > 1)
            {
                // Lato sinistro
                if (
                    col > 1 &&
                    at(row - 1, col - 1) == Player::piece::x &&
                    at(row - 2, col - 2) == Player::piece::e
                    )
                    return true;
                // Lato destro
                if (
                    col < 6 &&
                    at(row - 1, col + 1) == Player::piece::x &&
                    at(row - 2, col + 2) == Player::piece::e
                    )
                    return true;
            }
            break;
        case Player::piece::X:
            // Parte sopra
            if (row < 6)
            {
                // Lato sinistro
                if (
                    col > 1 &&
                    (at(row + 1, col - 1) == Player::piece::o ||
                        at(row + 1, col - 1) == Player::piece::O) &&
                    at(row + 2, col - 2) == Player::piece::e
                    )
                    return true;
                // Lato destro
                if (
                    col < 6 &&
                    (at(row + 1, col + 1) == Player::piece::o ||
                        at(row + 1, col + 1) == Player::piece::O) &&
                    at(row + 2, col + 2) == Player::piece::e
                    )
                    return true;
            }
            // Parte sotto
            if (row > 1)
            {
                // Lato sinistro
                if (
                    col > 1 &&
                    (at(row - 1, col - 1) == Player::piece::o ||
                        at(row - 1, col - 1) == Player::piece::O) &&
                    at(row - 2, col - 2) == Player::piece::e
                    )
                    return true;
                // Lato destro
                if (
                    col < 6 &&
                    (at(row - 1, col + 1) == Player::piece::o ||
                        at(row - 1, col + 1) == Player::piece::O) &&
                    at(row - 2, col + 2) == Player::piece::e
                    )
                    return true;
            }
            break;
        case Player::piece::O:
            // Parte sopra
            if (row < 6)
            {
                // Lato sinistro
                if (
                    col > 1 &&
                    (at(row + 1, col - 1) == Player::piece::x ||
                        at(row + 1, col - 1) == Player::piece::X) &&
                    at(row + 2, col - 2) == Player::piece::e
                    )
                    return true;
                // Lato destro
                if (
                    col < 6 &&
                    (at(row + 1, col + 1) == Player::piece::x ||
                        at(row + 1, col + 1) == Player::piece::X) &&
                    at(row + 2, col + 2) == Player::piece::e
                    )
                    return true;
            }
            // Parte sotto
            if (row > 1)
            {
                // Lato sinistro
                if (
                    col > 1 &&
                    (at(row - 1, col - 1) == Player::piece::x ||
                        at(row - 1, col - 1) == Player::piece::X) &&
                    at(row - 2, col - 2) == Player::piece::e
                    )
                    return true;
                // Lato destro
                if (
                    col < 6 &&
                    (at(row - 1, col + 1) == Player::piece::x ||
                        at(row - 1, col + 1) == Player::piece::X) &&
                    at(row - 2, col + 2) == Player::piece::e
                    )
                    return true;
            }
            break;
        }
        return false;
    }

    /**
     * @brief Controlla se la pedina può essere promossa a dama.
     *
     * @param row La riga della pedina.
     * @param col La colonna della pedina.
     * @return True se può essere promossa a dama, false altriment 
     */
    bool canBePromoted(int row, int col) const
    {
        switch (at(row, col))
        {
            // Una dama non può essere promossa!
        case Player::piece::X: return false;
        case Player::piece::O: return false;
        case Player::piece::x:
            if (row == 6)
            {
                // Lato sinistro
                if (
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
            }
            else if (row == 5 && canJump(row, col))
                return true;
            break;
        case Player::piece::o:
            if (row == 1)
            {
                // Lato sinistro
                if (
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
            }
            else if (row == 2 && canJump(row, col))
                return true;
            break;
        }
        return false;
    }

    /**
     * @brief Controlla se la pedina è minacciata da un'altra avversaria.
     * @param pos La posizione della pedina.
     * @return True se è minacciata, false altriment 
     */
    bool isThreatened(Position pos) const
    {
        // Una pedina ai bordi non può mai essere minacciata!
        if (
            pos.row <= 0 ||
            pos.row >= 7 ||
            pos.col <= 0 ||
            pos.col >= 7
            ) return false;

        switch (at(pos))
        {
        case Player::piece::o:
            if (
                (at(pos.row - 1, pos.col - 1) == Player::piece::x ||
                    at(pos.row - 1, pos.col - 1) == Player::piece::X) &&
                at(pos.row + 1, pos.col + 1) == Player::piece::e
                ) return true;
            if (
                (at(pos.row - 1, pos.col + 1) == Player::piece::x ||
                    at(pos.row - 1, pos.col + 1) == Player::piece::X) &&
                at(pos.row + 1, pos.col - 1) == Player::piece::e
                ) return true;
            break;
        case Player::piece::x:
            if (
                (at(pos.row + 1, pos.col - 1) == Player::piece::o ||
                    at(pos.row + 1, pos.col - 1) == Player::piece::O) &&
                at(pos.row - 1, pos.col + 1) == Player::piece::e
                ) return true;
            if (
                (at(pos.row + 1, pos.col + 1) == Player::piece::o ||
                    at(pos.row + 1, pos.col + 1) == Player::piece::O) &&
                at(pos.row - 1, pos.col - 1) == Player::piece::e
                ) return true;
            break;
        case Player::piece::O:
            if (
                at(pos.row - 1, pos.col - 1) == Player::piece::X &&
                at(pos.row + 1, pos.col + 1) == Player::piece::e
                ) return true;
            if (
                at(pos.row - 1, pos.col + 1) == Player::piece::X &&
                at(pos.row + 1, pos.col - 1) == Player::piece::e
                ) return true;
            break;
        case Player::piece::X:
            if (
                at(pos.row + 1, pos.col - 1) == Player::piece::O &&
                at(pos.row - 1, pos.col + 1) == Player::piece::e
                ) return true;
            if (
                at(pos.row + 1, pos.col + 1) == Player::piece::O &&
                at(pos.row - 1, pos.col - 1) == Player::piece::e
                ) return true;
            break;
        }
        return false;
    }

    /**
     * @brief Confronta le due board ed estrae la mossa compiuta.
     * La board attuale sarà quella "di arrivo", quella passata come parametro
     * invece è quella "di partenza".
     *
     * @param b La board di inizio della mossa.
     * @return La mossa estratta.
     * @exception player_exception Viene lanciata se la mossa non è valida.
     */
    Move getMove(const Board& b) const
    {
        // TODO
    }

    List<Position> getMovablePieces(int player_nr)
    {
        List<Position> movable;
        if (player_nr < 1 || player_nr > 2)
            throw player_exception{ player_exception::index_out_of_bounds, "Player number not valid" };
        for (int i = 0; i < 8; ++i)
        {
            for (int j = 0; j < 8; ++j)
            {
                if (player_nr == 1)
                {
                    if (
                        at(i, j) == Player::piece::x ||
                        at(i, j) == Player::piece::X
                        )
                        if (canMove(i, j))
                            movable.push_back(Position{ i, j });
                }
                else
                {
                    if (
                        at(i, j) == Player::piece::o ||
                        at(i, j) == Player::piece::O
                        )
                        if (canMove(i, j))
                            movable.push_back(Position{ i, j });
                }
            }
        }
        return movable;
    }

    /**
     * @brief Applica una mossa alla board corrente, restituendo una board nuova.
     *
     * @param m La mossa da applicare alla board.
     * @return La board con la mossa applicata.
     * @exception player_exception Viene lanciata se la mossa non è valida.
     */
    Board applyMove(const Move& m) const
    {
        int rowDelta = m.to.row - m.from.row;
        int colDelta = m.to.col - m.from.col;
        if (
            // Il movimento deve essere in diagonale
            std::abs(rowDelta) != std::abs(colDelta) ||
            // Deve muoversi almeno di una cella
            rowDelta < -2 ||
            // Non può muoversi più di due celle
            rowDelta > 2 ||
            // Non si può muovere una cella vuota
            at(m.from.row, m.from.col) == Player::piece::e ||
            // Non si può arrivare su una cella non vuota
            at(m.to.row, m.to.col) != Player::piece::e
            ) throw player_exception{ player_exception::invalid_board, "Invalid move" };

        Board b(*this);
        if (
            b.at(m.from.row, m.from.col) == Player::piece::o &&
            rowDelta > 0
            ) throw player_exception{ player_exception::invalid_board, "o piece can't go upwards." };
        else if (
            b.at(m.from.row, m.from.col) == Player::piece::x &&
            rowDelta < 0
            ) throw player_exception{ player_exception::invalid_board, "x piece can't go downwards." };

        if (std::abs(rowDelta) == 2)
        {
            // Controllo se la pedina in mezzo è "mangiabile"
            switch (b.at(m.from.row, m.from.col))
            {
            case Player::piece::o:
                if (
                    b.at(
                        m.from.row + (rowDelta / 2),
                        m.from.col + (colDelta / 2)
                    ) != Player::piece::x
                    ) throw player_exception{ player_exception::invalid_board, "Invalid move" };
                break;
            case Player::piece::x:
                if (
                    b.at(
                        m.from.row + (rowDelta / 2),
                        m.from.col + (colDelta / 2)
                    ) != Player::piece::o
                    ) throw player_exception{ player_exception::invalid_board, "Invalid move" };
                break;
            case Player::piece::O:
                if (
                    b.at(
                        m.from.row + (rowDelta / 2),
                        m.from.col + (colDelta / 2)
                    ) != Player::piece::x &&
                    b.at(
                        m.from.row + (rowDelta / 2),
                        m.from.col + (colDelta / 2)
                    ) != Player::piece::X
                    ) throw player_exception{ player_exception::invalid_board, "Invalid move" };
                break;
            case Player::piece::X:
                if (
                    b.at(
                        m.from.row + (rowDelta / 2),
                        m.from.col + (colDelta / 2)
                    ) != Player::piece::o &&
                    b.at(
                        m.from.row + (rowDelta / 2),
                        m.from.col + (colDelta / 2)
                    ) != Player::piece::O
                    ) throw player_exception{ player_exception::invalid_board, "Invalid move" };
                break;
            }
            b.at(m.from) = Player::piece::e;
            b.at(
                m.from.row + (rowDelta / 2),
                m.from.col + (colDelta / 2)
            ) = Player::piece::e;
        }

        b.at(m.from.row, m.from.col) = Player::piece::e;
        // ↓ Attenzione, è la board attuale, non quella nuova!
        b.at(m.to.row, m.to.col) = at(m.from.row, m.from.col);

        // Promozione delle pedine
        if (
            m.to.row == 0 &&
            at(m.from.row, m.from.col) == Player::piece::o
            ) b.promote(m.to.row, m.to.col);
        if (
            m.to.row == 7 &&
            at(m.from.row, m.from.col) == Player::piece::x
            ) b.promote(m.to.row, m.to.col);
        return b;
    }

    void promote(int row, int col)
    {
        switch (at(row, col))
        {
        case Player::piece::o:
            at(row, col) = Player::piece::O;
            break;
        case Player::piece::x:
            at(row, col) = Player::piece::X;
        }
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
    Vector<Vector<Player::piece>> pieces{ 8, Vector<Player::piece>{8} };
};

#pragma endregion

struct Player::Impl
{
    int player_nr;
    piece playerPiece;
    piece playerDama;
    List<Board> history;

    /**
     * @brief Data una lista di posizioni di pedine,
     * valuta la mossa migliore che si può fare.
     * Almeno un pezzo deve essere movibile.
     * 
     * @param pieces Le posizioni delle pedine.
     * @return La mossa migliore.
     */
    Move bestMove(const List<Position>& pieces) const
    {
        const Board& lastBoard = history.front();
        Move best;
        int best_points;
        List<Move> moves;
        for (const Position p : pieces)
        {
            moves += lastBoard.getPossibleMoves(p);
        }

        if (!moves.isEmpty())
        {
            best = moves.front();
            best_points = evaluateMove(moves.front(), lastBoard);
            moves.pop_front();
            for (const Move m : moves)
            {
                int val = evaluateMove(m, lastBoard);
                if (val > best_points)
                {
                    best = m;
                    best_points = val;
                }
            }
        }

        return best;
    }

    int evaluateMove(const Move& m, const Board& b) const
    {
        int points = 0;
        Board applied = b.applyMove(m);
        // La pedina mangia una pedina avversaria?
        if (m.jumps())
            points += 5;
        // La pedina è minacciata una volta mossa?
        if (applied.isThreatened(m.to))
        {
            points -= 5;
        }
        // Viene promossa?
        if (b.at(m.from) == playerPiece &&
            applied.at(m.to) == playerDama
            ) points += 10;
        // Vince la partita?
        if (applied.getMovablePieces(get_opponent(player_nr)).isEmpty())
            points += 100;
        return points;
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
    if (player_nr == 1)
    {
        pimpl->playerPiece = piece::x;
        pimpl->playerDama = piece::X;
    }
    else
    {
        pimpl->playerPiece = piece::o;
        pimpl->playerDama = piece::O;
    }
}

Player::Player(const Player& p)
{
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
    List<Position> movable = pimpl->history.front().getMovablePieces(pimpl->player_nr);
    if (movable.isEmpty())
    {
        pimpl->history.push_front(pimpl->history.front());
    }
    else
    {
        Move best = pimpl->bestMove(movable);
        Board newone = pimpl->history.front().applyMove(best);
        pimpl->history.push_front(newone);
    }
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
    return loses(get_opponent(player_nr));
}

bool Player::wins() const
{
    return wins(pimpl->player_nr);
}

bool Player::loses(int player_nr) const
{
    List<Position> movable{ pimpl->history.front().getMovablePieces(pimpl->player_nr) };
    return movable.isEmpty();
}

bool Player::loses() const
{
    return loses(pimpl->player_nr);
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
