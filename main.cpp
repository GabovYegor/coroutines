#include <iostream>
#include <thread>
#include <chrono>

#include "resumable_no_own.h"
#include "resumable_continuable_coroutine.h"
#include "co_return_value_coroutine.h"
#include "resumable_coroutine.h"
#include "iteratable_generator.h"
#include "event_awaiter.h"
#include "task_awaiter_t.h"

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

resumable_continuable coro_suspend_continue() {
    for(size_t i = 0; i < 10; ++i) {
        std::cout << i << std::endl;
        co_await std::suspend_always{};
    }
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

void print_example_header() {
    static size_t example_counter = 1;
    std::cout << std::endl
              << "----------------- EXAMPLE " << example_counter++
              << " -----------------" << std::endl;
}

void print_hello() {
    std::this_thread::sleep_for(std::chrono::seconds(3));
    std::cout << "Hello world!" << std::endl;
}

resumable_no_own coro_task() {
    std::cout << "coro_task() started" << std::endl;
    auto result = co_await task_awaiter_t{ print_hello };
    std::cout << "coro_task() result : " << result << std::endl;
    std::cout << "coro_task() finished" << std::endl;
}

int main() {
    print_example_header(); // 1
    common_function();

    print_example_header(); // 2
    auto sum_accessor = coro_sum(5, 9);
    std::cout << "sum_accessor.get_value() result: " << sum_accessor.get_value() << std::endl;

    print_example_header(); // 3
    auto coro_suspend_accessor = coro_suspend();
    std::cout << "coro_suspend() is about to be resumed" << std::endl;
    coro_suspend_accessor.resume();
    std::cout << "coro_suspend() is about to be resumed again" << std::endl;
    coro_suspend_accessor.resume();

    print_example_header(); // 4
    auto coro_suspend_continue_accessor = coro_suspend_continue();
    coro_suspend_continue_accessor.resume();
    coro_suspend_continue_accessor.resume();
    coro_suspend_continue_accessor.resume();
    std::cout << "continue() was called" << std::endl;
    coro_suspend_continue_accessor.continue_execution();

    print_example_header(); // 5
    std::cout << "sequence result: ";
    for(auto i : sequence(10, 20, 2))
        std::cout << i << " ";
    std::cout << std::endl;

    print_example_header(); // 6
    consumer1();
    consumer2();
    producer();
    producer();

    print_example_header(); // 7

    coro_task();
    return 0;
}