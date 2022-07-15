#pragma once

#include <coroutine>

struct resumable_no_own {
    struct promise_type {
        auto get_return_object() noexcept {
            return resumable_no_own{};
        }

        auto initial_suspend() noexcept {
            return std::suspend_never{};
        }

        auto final_suspend() noexcept {
            return std::suspend_never{};
        }

        void return_void() {}

        void unhandled_exception() {}
    };
};