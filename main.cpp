#include <iostream>

#include "resumable_no_own.h"
#include "co_return_value_coroutine.h"
#include "resumable_coroutine.h"
#include "generator.h"
#include "iteratable_generator.h"
#include "event_awaiter.h"

// That's basically common function. Common functions are subset of coroutines
resumable_no_own common_function() {
    co_return;
}

template <class T>
returnable_any<T> coro_sum(const T lhs, const T rhs)
{
    co_return lhs + rhs;
}

resumable coro_suspend() {
    std::cout << "coro_suspend() point 1" << std::endl;
    co_await std::suspend_always{};
    std::cout << "coro_suspend() point 2" << std::endl;
}

iteratable_generator<size_t> sequence(size_t start, size_t finish, size_t step = 1) {
    for(size_t i = start; i <= finish; i += step)
        co_yield i;
}

evt_awaiter_t g_event;

resumable_no_own consumer1() {
    std::cout << "consumer1 point 1" << std::endl;
    co_await g_event;
    std::cout << "consumer1 point 2" << std::endl;
    co_await g_event;
    std::cout << "consumer1 point 3" << std::endl;
}

resumable_no_own consumer2() {
    std::cout << "consumer2 point 1" << std::endl;
    co_await g_event;
    std::cout << "consumer2 point 2" << std::endl;
    co_await g_event;
    std::cout << "consumer2 point 3" << std::endl;
}

void producer() {
    std::cout << "producer() is working ..." << std::endl;
    std::cout << "Data has been prepared for consumers!" << std::endl;
    g_event.set();
}

int main() {
    common_function();

    auto sum_accessor = coro_sum(5, 9);
    std::cout << "sum_accessor.get_value() result: " << sum_accessor.get_value() << std::endl;

    auto coro_suspend_accessor = coro_suspend();
    std::cout << "coro_suspend() is about to be resumed" << std::endl;
    coro_suspend_accessor.resume();
    std::cout << "coro_suspend() is about to be resumed again" << std::endl;
    coro_suspend_accessor.resume();

    std::cout << "sequence result: ";
    for(auto i : sequence(10, 20, 2))
        std::cout << i << " ";
    std::cout << std::endl;

    consumer1();
    consumer2();
    producer();
    producer();
    return 0;
}