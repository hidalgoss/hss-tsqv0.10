#ifndef HSS_IQUEUE_HPP
#define HSS_IQUEUE_HPP

#include <iostream>

namespace hss {

template <typename T>
class IQueue {
public:
    void push(T & new_value);
    void pop(T &v);
    bool try_push(const T &v);
    bool try_pop(T &value);
    bool empty() const;

};// end class IQueue.
}// end namespace hss.

#endif // HSS_IQUEUE_HPP
