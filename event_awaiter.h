#pragma once

#include <coroutine>
#include <list>
#include <thread>
#include <chrono>

using coro_type = std::coroutine_handle<>;

struct evt_awaiter_t {
    struct awaiter {
        awaiter(evt_awaiter_t& event) : event_ { event } {}

        bool await_ready() const noexcept { return event_.is_set(); }

        void await_suspend(coro_type handle) noexcept {
            handle_ = handle;
            event_.push_awaiter(*this);
        }

        void await_resume() noexcept {
            event_.reset();
        }

        evt_awaiter_t& event_;
        coro_type handle_ = nullptr;
    };

    bool is_set() const noexcept { return set_; }

    void set() noexcept {
        set_ = true;

        std::list<awaiter> awaiters_to_resume;
        awaiters_to_resume.splice(awaiters_to_resume.begin(), lst_);
        for(auto awaiter : awaiters_to_resume)
            awaiter.handle_.resume();
    }

    void reset() noexcept { set_ = false; }

    void push_awaiter(awaiter awaiter) { lst_.push_back(awaiter); }

    awaiter operator co_await() noexcept { return awaiter{ *this }; }

private:
    std::list<awaiter> lst_;
    bool set_ = false;
};
