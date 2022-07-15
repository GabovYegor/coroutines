#pragma once

#include <coroutine>

namespace co_return_value_coroutine {
    template <class T>
    struct Promise;

    template <class T>
    struct CoroReturnType {
        using promise_type = Promise<T>;

        CoroReturnType(promise_type* promise) : _promise{ promise } {}

        T get_value() {
            return _promise->data;
        }

        promise_type* _promise;
    };

    template <class T>
    struct Promise {
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
            data = std::forward<T>(value);
        }

        void unhandled_exception() {}

        T data;
    };

    CoroReturnType<int> coroutine() {
        co_return 42;
    }
}