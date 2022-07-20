#pragma once

#include <coroutine>

struct suspend_customizable {
    suspend_customizable(bool is_always_ready)
        : is_ready_{is_always_ready } {}

    constexpr bool await_ready() const noexcept { return is_ready_; }

    constexpr void await_suspend(std::coroutine_handle<>) const noexcept {}

    constexpr void await_resume() const noexcept {}

private:
    bool is_ready_ = true;
};

struct resumable_continuable {

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

        suspend_customizable await_transform(std::suspend_always) {
            if(is_continue)
                return suspend_customizable{ true };
            return suspend_customizable{ false };
        }

        bool is_continue = false;
    };

    resumable_continuable(coro_handle handle) : handle_{handle } {}

    bool resume() {
        if(!handle_.done()) {
            handle_.resume();
        }
        return !handle_.done();
    }

    void continue_execution() {
        handle_.promise().is_continue = true;
        resume();
    }

    ~resumable_continuable() {
        handle_.destroy();
    }

private:
    coro_handle handle_;
};