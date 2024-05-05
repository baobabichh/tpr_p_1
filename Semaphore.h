#pragma once
#include <mutex>
#include <condition_variable>


class Semaphore
{
public:
    explicit Semaphore(size_t counter = 0);
    inline void wait()
    {
        std::unique_lock<std::mutex> lk(_m);
        _cv.wait(lk, [this] { return _counter > 0; });
        --_counter;
    }
    inline void post()
    {
        std::unique_lock<std::mutex> lk(_m);
        ++_counter;
        _cv.notify_one();
    }
    inline size_t available() const
    {
        return _counter;
    }

private:
    size_t _counter{};
    std::mutex _m{};
    std::condition_variable _cv{};
};