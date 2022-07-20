#pragma once

#include <coroutine>
#include <list>
#include <functional>
#include <future>

struct task_awaiter_t {
    using task_t = std::function<void()>;

    task_awaiter_t(task_t&& task) : task_{ std::move(task) } {}

    struct awaiter {
        awaiter(task_t&& task) : task_{ std::move(task) } {}

        bool await_ready() const noexcept { return false; }

        void await_suspend(std::coroutine_handle<> coro) noexcept {
            std::thread([coro, this] {
                task_();
                coro();
            }).detach();
        }

        void await_resume() noexcept {}
    private:
        task_t task_;
    };

    awaiter operator co_await() noexcept { return awaiter{ std::move(task_) }; }

private:
    task_t task_;
};

