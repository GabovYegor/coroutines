#pragma once

#include <coroutine>
#include <list>
#include <functional>

struct task_awaiter_t {
    using task_t = std::function<void()>;

    task_awaiter_t(task_t&& task) : task_{ task } {}

    struct awaiter {
        awaiter(task_t&& task) : task_{ task } {}

        bool await_ready() const noexcept { return false; }

        void await_suspend(std::coroutine_handle<> coro) noexcept {
            task_();
            coro.resume();
        }

        std::string await_resume() noexcept {
            return "Hello coroutine!";
        }
    private:
        task_t task_;
    };

    awaiter operator co_await() noexcept { return awaiter{ std::move(task_) }; }

private:
    task_t task_;
};

