#pragma once

#include <coroutine>

template <class T>
struct Iterator {
    Iterator() = default;

    Iterator(Generator<T>* generator) : generator_ { generator } {}

    bool operator!=(const Iterator& other) {
        return generator_ != other.generator_;
    }

    Iterator operator++() {
        auto done = generator_->move_next();
        if(done)
            generator_ = nullptr;
        return *this;
    }

    T operator*() {
        return generator_->current_value();
    }

private:
    Generator<T> * generator_;
};

template <class T>
struct IteratableGenerator {

    struct promise_type;
    using coro_handle = std::coroutine_handle<promise_type>;

    struct promise_type {
        auto get_return_object() noexcept {
            return coro_handle::from_promise(*this);
        }

        auto initial_suspend() noexcept {
            return std::suspend_never{};
        }

        auto final_suspend() noexcept {
            return std::suspend_always{};
        }

        void return_void() {}

        auto yield_value(T value) {
            data_ = value;
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

    IteratableGenerator(coro_handle handle) : handle_{ handle } {}

    auto begin() {
        return Iterator<T>{ this };
    }

    auto end() {
        return Iterator<T>{ nullptr };
    }

    bool move_next() {
        if(!handle_.done())
            handle_.resume();
        return handle_.done();
    }

    T current_value() {
        return handle_.promise().get_data();
    }

    ~IteratableGenerator() {
        handle_.destroy();
    }

private:
    coro_handle handle_;
};