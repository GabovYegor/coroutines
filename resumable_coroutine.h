#pragma once

#include <coroutine>

struct resumable {

    struct promise_type;
    using coro_handle = std::coroutine_handle<promise_type>;

    struct promise_type {
        auto get_return_object() noexcept {
            return coro_handle::from_promise(*this);
        }

        auto initial_suspend() noexcept {
            return std::suspend_always{};
        }

        auto final_suspend() noexcept {
            return std::suspend_always{};
        }

        void return_void() {}

        void unhandled_exception() {
            std::terminate();
        }
    };

    resumable(coro_handle handle) : handle_{handle } {}

    bool resume() {
        if(!handle_.done())
            handle_.resume();
        return !handle_.done();
    }

    ~resumable() {
        handle_.destroy();
    }

private:
    coro_handle handle_;
};