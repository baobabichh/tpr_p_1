#pragma once
#include <mutex>
#include <condition_variable>
#include <iostream>


class Semaphore
{
public:
    explicit Semaphore(size_t counter = 0);
    void wait();
    void post();
    size_t available() const;

private:
    size_t _counter{};
    std::mutex _m{};
    std::condition_variable _cv{};
};