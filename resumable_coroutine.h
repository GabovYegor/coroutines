#pragma once

#include <coroutine>

namespace resumable_coroutine {
    struct Promise;

    using coro_handle = std::coroutine_handle<Promise>;

    struct Resumable {
        using promise_type = Promise;

        Resumable(coro_handle handle) : handle_{ handle } {}

        bool resume() {
            if(!handle_.done())
                handle_.resume();
            return !handle_.done();
        }

        ~Resumable() {
            handle_.destroy();
        }

    private:
        coro_handle handle_;
    };

    struct Promise {
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

    Resumable coroutine() {
        std::cout << "Hello " << std::endl;
        co_await std::suspend_always{};
        std::cout << "world!" << std::endl;
    }
}