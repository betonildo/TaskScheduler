#ifndef FIXED_QUEUE_H
#define FIXED_QUEUE_H

#ifndef UNSIGNED_SIZE 
typedef unsigned long long unsigned_size_t; 
#endif /*UNSIGNED_SIZE*/

template<class T, unsigned_size_t size>
class fixed_queue {

private:
    T m_queue[size];
    bool m_used[size];
    unsigned_size_t m_index;

public:

    fixed_queue() {
        m_index = 0;
    }

    void enqueue(T element) {
        m_used[m_index] = true;
        m_queue[m_index] = element;
        m_index = (++m_index) % size;        
    }

    T dequeue() {
        m_used[m_index] = false;
        T element = m_queue[m_index];
        m_index = (++m_index) % size;
        return element;
    }

    bool isEmpty() {
        for(unsigned_size_t i = 0; i < size; i++) {
            if (m_used[i]) return false;
        }
    }

    bool isFull() {
        for(unsigned_size_t i = 0; i < size; i++) {
            if (!m_used[i]) return false;
        }
    }
};

#endif /*FIXED_QUEUE_H*/
