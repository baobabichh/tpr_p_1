#include "Semaphore.h"

Semaphore::Semaphore(size_t counter) : _counter{counter}
{
}

inline void Semaphore::wait()
{
    std::unique_lock<std::mutex> lk(_m);
    _cv.wait(lk, [this] { return _counter > 0; });
    --_counter;
}

inline void Semaphore::post()
{
    std::unique_lock<std::mutex> lk(_m);
    ++_counter;
    _cv.notify_one();
}

inline size_t Semaphore::available() const
{
    return _counter;
}
