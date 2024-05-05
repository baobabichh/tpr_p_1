#include "Semaphore.h"

Semaphore::Semaphore(size_t counter) : _counter{counter}
{
}

void Semaphore::wait()
{
    std::unique_lock<std::mutex> lk(_m);
    _cv.wait(lk, [this] { return _counter > 0; });
    --_counter;
}

void Semaphore::post()
{
    ++_counter;
    _cv.notify_one();
}

size_t Semaphore::available() const
{
    return _counter;
}
