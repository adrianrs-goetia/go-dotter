#pragma once

template <typename T>
class CircularBuffer {
private:
    int pos = 0;
    int _size;
    T* _buffer;

public:
    CircularBuffer(int size): _size{size}, pos{_size-1} {
        _buffer = new T[_size];
    }

    CircularBuffer(int size, T fill): _size{size}, pos{_size-1} {
        _buffer = new T[_size];
        LOG(DEBUG, "size=%s", _size);
        for (int i=0; i<_size; i++) {
            _buffer[i]=fill;
        }
    }

    ~CircularBuffer() {
        delete[] _buffer;
    }

    T get(int index) {
        return _buffer[(pos-(index%_size)+_size)%_size];
    }

    T get_absolute(int index) {
        return _buffer[index];
    }

    void push(T element) {
        pos=(pos+1)%_size;
        _buffer[pos]=element;
    }

    T pop() {
        T tmp=_buffer[pos];
        pos=(pos-1+_size)%_size;
        return tmp;
    }

    struct Iterator 
    {
        using iterator_category = std::forward_iterator_tag;
        using difference_type   = std::ptrdiff_t;
        using value_type        = T;
        using pointer           = T*;
        using reference         = T&;
        pointer leftmost = _buffer;

        Iterator(pointer ptr) : m_ptr(ptr) {}

        reference operator*() const { return *m_ptr; }
        pointer operator->() { return m_ptr; }
        Iterator& operator++() { m_ptr=leftmost+(m_ptr-leftmost-1+_size)%_size; return *this; }  
        Iterator operator++(int) { Iterator tmp = *this; m_ptr=leftmost+(m_ptr-leftmost-1+_size)%_size; return tmp; }
        friend bool operator== (const Iterator& a, const Iterator& b) { return a.m_ptr == b.m_ptr; };
        friend bool operator!= (const Iterator& a, const Iterator& b) { return a.m_ptr != b.m_ptr; } 

    private:
        pointer m_ptr;
    };

    Iterator begin() { return Iterator(&_buffer[pos]); }
    Iterator end()   { return Iterator(&_buffer[_size+1]); }
};

