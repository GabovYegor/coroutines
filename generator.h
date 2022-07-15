#pragma once

#include <coroutine>

namespace generator {
    template <class T>
    struct Promise;

    template <class T>
    using coro_handle = std::coroutine_handle<Promise<T>>;

    template <class T>
    struct Generator {
        using promise_type = Promise<T>;

        Generator(coro_handle<T> handle) : handle_{ handle } {}

        bool move_next() {
            if(!handle_.done())
                handle_.resume();
            return !handle_.done();
        }

        T current_value() {
            return handle_.promise().get_data();
        }

        ~Generator() {
            handle_.destroy();
        }

    private:
        coro_handle<T> handle_;
    };

    template <class T>
    struct Promise {
        auto get_return_object() noexcept {
            return coro_handle<T>::from_promise(*this);
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

    Generator<int> coroutine() {
        int num = 0;
        while (true) {
            co_yield num++;
        }
    }
}