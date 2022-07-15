#pragma once

#include <coroutine>

template <class T>
struct generator {
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

        auto yield_value(T&& value) {
            data_ = std::forward<T>(value);
            return std::suspend_always{};
        }

        void unhandled_exception() {
            std::terminate();
        }

        auto get_data() {
            return data_;
        }

    private:
        T data_ = {};
    };

    generator(coro_handle handle) : handle_{handle } {}

    bool move_next() {
        if(!handle_.done())
            handle_.resume();
        return !handle_.done();
    }

    T current_value() {
        return handle_.promise().get_data();
    }

    ~generator() {
        handle_.destroy();
    }

private:
    coro_handle handle_;
};