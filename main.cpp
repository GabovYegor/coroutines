// To avoid build issues on MSVC
#define __cpp_lib_coroutine

#include <iostream>
#include <thread>
#include <chrono>

#include <cppcoro/task.hpp>
#include <cppcoro/generator.hpp>
#include <cppcoro/static_thread_pool.hpp>
#include <cppcoro/sync_wait.hpp>

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
    std::cout << "Hello coroutine" << std::endl;
}

resumable_no_own coro_task() {
    std::cout << "coro_task() started" << std::endl;
    co_await task_awaiter_t{ [] { print_hello(); } };
    std::cout << "coro_task() finished" << std::endl;
}

cppcoro::generator<const std::uint64_t> fibonacci()
{
    std::uint64_t a = 0, b = 1;
    while (true)
    {
        co_yield b;
        auto tmp = a;
        a = b;
        b += tmp;
    }
}

cppcoro::task<void> do_something_on_threadpool(cppcoro::static_thread_pool& tp)
{
    // First schedule the coroutine onto the threadpool.
    co_await tp.schedule();

    // When it resumes, this coroutine is now running on the threadpool.
    print_hello();
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
    std::cout << "Started complex task in main" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(4));
    std::cout << "Finished complex task in main" << std::endl;

    print_example_header(); // 8
    int counter = 0;
    for (auto fib_val : fibonacci()) {
        if(counter++ == 9) {
            std::cout << fib_val << std::endl;
            break;
        }
    }

    print_example_header(); // 9
    cppcoro::static_thread_pool tp;
    auto do_smth_coro = do_something_on_threadpool(tp);

    std::cout << "Started complex task_2 in main" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout << "Finished complex task_2 in main" << std::endl;

    cppcoro::sync_wait(do_smth_coro);
    return 0;
}