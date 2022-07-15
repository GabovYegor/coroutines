#pragma once

#include <coroutine>


template <class T>
struct returnable_any {
    struct promise_type {
        auto get_return_object() noexcept {
            return this;
        }

        auto initial_suspend() noexcept {
            return std::suspend_never{};
        }

        auto final_suspend() noexcept {
            return std::suspend_always{};
        }

        void return_value(T&& value) {
            value_to_return = std::forward<T>(value);
        }

        void unhandled_exception() {}

        T value_to_return;
    };

    returnable_any(promise_type* promise) : _promise{ promise } {}

    T get_value() {
        return _promise->value_to_return;
    }

    promise_type* _promise;
};

