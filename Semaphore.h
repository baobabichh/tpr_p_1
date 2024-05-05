#pragma once
#include <mutex>
#include <condition_variable>


class Semaphore
{
private:
    size_t _counter{};
    std::mutex _m{};
    std::condition_variable _cv{};

public:
    explicit Semaphore(size_t counter = 0);
    void wait();
    void post();
    size_t available() const;
};