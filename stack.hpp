#include <string>

struct player_exception{
    enum err_type {index_out_of_bounds,missing_file,invalid_board};
    err_type t;
    std::string msg;
};

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
    using Pcell = Cell<T>*;
    class iterator
    {
    public:
        iterator(Pcell cell);

        iterator& operator++();

        iterator operator++(int);

        T& operator*();

    private:
        using iterator_category = std::forward_iterator_tag;
        Cell <T>* current;
    };

    Stack();

    Stack(const Stack<T>& s);

    ~Stack();

    void push(T element);

    T& top();

    T& top() const;

    void pop();

    T& at(int pos);
    
    T& at(int pos) const;

    void remove(int pos);

    int size() const;

    Stack& operator=(const Stack& s);

private:
    Pcell head;

    Pcell copy(Pcell pc);

    void remove_rec(int pos, Pcell pc);

    void destroy(Pcell pc);
};
